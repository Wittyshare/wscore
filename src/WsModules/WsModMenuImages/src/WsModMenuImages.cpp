/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

#include <Wt/WLogger>
#include <Wt/WText>

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

// TODO : pas top je sais
#include "WsModules/WsModImages2/src/WsImages2.h"

#include "WsModMenuImages.h"

extern "C" {
  void WsModMenuImagesInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

using namespace Wt;

// Class WsMenuImages
// ==================
WsMenuImages::WsMenuImages(WContainerWidget* parent)
  : WContainerWidget(parent), m_nStartImage(0), m_nMaxImages(3), m_pTimer(0)
{
  addStyleClass("WsMenuImages");
  LOG(DEBUG) << "WsMenuImages::WsMenuImages() !";
}

WsMenuImages::~WsMenuImages()
{
  LOG(DEBUG) << "WsMenuImages::~WsMenuImages() !";
}

void WsMenuImages::load()
{
  int nMaxImages = asNumber(option("maxImages"));
  if ( nMaxImages > 0 )
    setMaxImages(nMaxImages);
  WContainerWidget::load();
  buildVector();
  buildMenu();
  if ( m_vDirectories.size() > m_nMaxImages ) {
    // On ajoute le timer au widget, les objets de ce type ne sont pas détruits par un clear();
    m_pTimer = new WTimer(this);
    m_pTimer->setInterval(5000);
    m_pTimer->timeout().connect(SLOT(this, WsMenuImages::doTimeout));
    m_pTimer->start();
  }
  WApplication::instance()->log("notice") << "WsMenuImages::load : end !";
}

// TODO : voir avec Koen si pas moyen de stocker l'objet, actu 2 x la construction de pImg il faudrait qu'il ne soit pas detruit lors d'1 clear()
void WsMenuImages::buildVector()
{
  std::string                   endPath(GlobalConfig::PathToImages); // "/ws.res/images
  WsUser*                       pUser            = WsApp->wsUser();
  std::string                   sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(WsApp->internalPath());
  WApplication::instance()->log("ERROR") << "WsMenuImages::buildVector : sWithoutPrefix!" << sWithoutPrefix;
  NodePtr               startNode        = pUser->getAccessRoot();
  if ( !startNode.get() ) {
    WApplication::instance()->log("ERROR") << "WsMenuImages::buildVector : startNode is NULL !";
    return;
  }
  NodePtr               selNode        = startNode.get()->eatPath(sWithoutPrefix);
  if ( !selNode.get() ) {
    WApplication::instance()->log("ERROR") << "WsMenuImages::buildVector : selNode is NULL !";
    return;
  }
  if ( selNode.get()->isRegularFile() ) {
    selNode = selNode.get()->getParent();
    if ( !selNode.get() ) {
      WApplication::instance()->log("ERROR") << "WsMenuImages::buildVector : selected node parent is NULL !" << sWithoutPrefix;
      return;
    }
  }
  WApplication::instance()->log("notice") << "WsMenuImages::buildVector : startDir = " << startNode.get()->getPath().string() << " and endPath = " << endPath;
  std::vector<NodePtr> dirNode          = startNode.get()->getDirectories();
  WApplication::instance()->log("notice") << "WsMenuImages::buildVector : directory size : " << dirNode.size();
  for (std::vector<NodePtr>::iterator it = dirNode.begin(); it != dirNode.end(); ++it) {
    NodePtr curNode = *it;
    //    TODO : voir avec Ben
    //    if ( curNode.get()->getProperties().get()->get("global", "in_menu"Images, DefaultProperties::InMenuImages) != "true" ) continue;
    std::string curDir = curNode.get()->getFullPath().string() + endPath;
    //    WApplication::instance()->log("notice") << "WsMenuImages::buildVector : test curNode fullPath = " << curDir << " title = " << curNode.get()->getDisplayName();
    if ( !boost::filesystem::exists(curDir) ) continue;
    WApplication::instance()->log("notice") << "WsMenuImages::buildVector : curNode match images path = " << curDir << " title = " << curNode.get()->getDisplayName();
    // TODO : necessaire de passer par le module ? (pour les options ?)
    WsImages2* pImg = new WsImages2(); //dynamic_cast<WsImages2*>(WsApp->WsModules().module("WsModImages2")->createContents());
    pImg->setOptions(options());
    if ( asString(option("debug")) == "true" )
      pImg->outOptions("WsMenuImages::buildVector()");
    WApplication::instance()->log("notice") << "WsMenuImages::buildVector : WsImages2 set images path = " << curNode.get()->getPath().string();
    std::string curPathUrl = curNode.get()->getFullPath().string();
    boost::algorithm::replace_first(curPathUrl, wApp->docRoot(), "");
    pImg->setOption("imagesPath", std::string(curPathUrl + endPath));
    WApplication::instance()->log("notice") << "WsMenuImages::buildVector : WsImages2 set path = " << std::string(curPathUrl + endPath);
    pImg->build();
    if ( !pImg->count() ) {
      delete pImg;
      WApplication::instance()->log("notice") << "WsMenuImages::buildVector : WsImages2 has no images path = " << curNode.get()->getPath().string();
      continue;
    }
    m_vDirectories.push_back(curPathUrl + endPath);
    //    m_vDisplayNames.push_back(curNode.get()->getDisplayName());
    delete pImg;
  }
}

void WsMenuImages::doTimeout()
{
  if ( (m_nStartImage + 1) >= (m_vDirectories.size() - m_nMaxImages) )   m_nStartImage = 0;
  else                                                                  ++m_nStartImage;
  clear();
  buildMenu();
}

void WsMenuImages::buildMenu()
{
  // TODO : check 2 img or 4 with m_nMaxImages 3
  // TODO : add a internalPathChanged to reload the menu with the correct images for the new path
  if ( m_pTimer )
    m_pTimer->stop();
  int maxImg = m_nMaxImages;
  if ( maxImg > m_vDirectories.size() ) maxImg = m_vDirectories.size();
  for (int countDir = m_nStartImage; countDir < maxImg; ++countDir) {
    if ( countDir > m_vDirectories.size() ) {
      WApplication::instance()->log("notice") << "WsMenuImages::buildMenu : OH OH anormal break !!!!";
      break;
    }
    WsImages2* pImg = new WsImages2(); //dynamic_cast<WsImages2*>(WsApp->WsModules().module("WsModImages2")->createContents());
    pImg->setOptions(options());
    pImg->setOption("imagesPath", m_vDirectories[countDir]);
    pImg->setOption("useImageTitle", std::string("true"));
    if ( asString(option("debug")) == "true" )
      pImg->outOptions("WsMenuImages::buildVector()");
    pImg->build();
    //    WContainerWidget* cwImage = new WContainerWidget();
    //    std::string title = m_vDisplayNames[countDir];
    //    boost::algorithm::replace_all(title, "&", "&amp;");
    //    WText* pTextTitle = new WText(title);
    //    pTextTitle->addStyleClass("WsMenuImagesTitle");
    //    cwImage->addWidget(pTextTitle);
    //    cwImage->addWidget(pImg);
    //    addWidget(cwImage);
    addWidget(pImg);
  }
  if ( m_pTimer )
    m_pTimer->start();
}

void WsMenuImages::setMaxImages(int nMaxImages)
{
  m_nMaxImages = nMaxImages;
}



// Class WsModMenuImages
// =====================
WsModMenuImages::WsModMenuImages()
  : WsModule()
{
  LOG(DEBUG) << "WsModMenuImages::WsModMenuImages() !";
}

WsModMenuImages::~WsModMenuImages()
{
  LOG(DEBUG) << "WsModMenuImages::~WsModMenuImages() !";
}

WWidget* WsModMenuImages::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMenuImages::createContents(WContainerWidget* parent) const
{
  WsMenuImages* pMenuImg = new WsMenuImages(parent);
  pMenuImg->setOptions(options());
  return pMenuImg;
}

WsEditorWidget* WsModMenuImages::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMenuImages::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModMenuImages::saveEditor() const
{
  return true;
}

std::string WsModMenuImages::description() const
{
  return "WsModMenuImages wittyShare module";
}

