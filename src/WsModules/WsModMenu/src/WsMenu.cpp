/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/filesystem/operations.hpp>

#include <Include/WsGlobalConfig.h>

#include <Wt/WLogger>
#include <Wt/WTheme>
#include <Wt/WText>
#include <Wt/WLink>
#include <Wt/WResource>

#include <Main/WsApplication.h>

// TODO : pas top je sais
#include "WsModules/WsModImages2/src/WsImages2.h"

#include "WsMenu.h"

using namespace Wt;

WsMenu::WsMenu(WContainerWidget* parent)
  : WContainerWidget(parent), m_bDebug(false)
{
  m_sCurPath = WsApp->internalPath();
}

WsMenu::WsMenu(const std::string& path, WContainerWidget* parent)
  : WContainerWidget(parent), m_bDebug(false), m_sCurPath(path)
{
  LOG(DEBUG) << "WsMenu::WsMenu() ! " << this;
}

WsMenu::~WsMenu()
{
  LOG(DEBUG) << "WsMenu::~WsMenu() ! " << this;
}

void WsMenu::load()
{
  WContainerWidget::load();
  addStyleClass("WsMenu");
  wApp->log("notice") << "WsMenu::load() start";
  doLoadCurPath();
  wApp->internalPathChanged().connect(SLOT(this, WsMenu::doPathChanged));
  wApp->log("notice") << "WsMenu::load() end";
}

void WsMenu::doLoadCurPath()
{
  std::string rootPath = asString(option("rootPath")).toUTF8();
  boost::algorithm::replace_all(rootPath, "&amp;",  "&");
  // Si pas de root path on prend l'internal Path
  if ( rootPath.size() < 1 )
    m_sCurPath = WsApp->internalPath();
  else
    m_sCurPath = rootPath;
  std::string sWithoutPrefix = WsApp->WsModules().pathWithoutPrefix(m_sCurPath);
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::doLoadCurPath - internalPath = " << m_sCurPath << " without prefix : " << sWithoutPrefix;
  WsUser*         pUser     = WsApp->wsUser();
  NodePtr tmpNode = pUser->getAccessRoot().get()->eatPath(sWithoutPrefix);
  if ( !tmpNode.get() ) {
    wApp->log("notice") << "WsMenu::doLoadCurPath - eatPath return NULL ";
    return;
  }
  if ( tmpNode.get()->isRegularFile() ) {
    tmpNode = pUser->getAccessRoot().get()->eatPath(boost::filesystem::path(sWithoutPrefix).parent_path().string());
    if ( !tmpNode.get() ) {
      wApp->log("notice") << "WsMenu::doLoadCurPath - eatPath on parent return NULL ";
      return;
    }
  }
  NodePtr startNode = pUser->getMenuRoot();
  if ( !startNode.get() ) {
    wApp->log("notice") << "WsMenu::doLoadCurPath - startNode  = " << startNode;
    return;
  }
  NodePtr pNode = startNode.get()->eatPath(tmpNode.get()->getPath().string());
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::doLoadCurPath - pNode eathPath  = " << pNode.get();
  if ( !pNode.get() ) return;
  if ( asString(option("useTitle")) == "true" )   {
    std::string sTitle(pNode.get()->getDisplayName());
    boost::algorithm::replace_all(sTitle, "&", "&amp;");
    WText* title = new WText(sTitle);
    title->addStyleClass("WsMenuTitle");
    addWidget(title);
  }
  WMenu*          menuParent = 0;
  if ( asString(option("useButtons")) != "true" )
    menuParent = new WMenu(this);
  if ( asString(option("showRoot")) == "true" )
    if ( pNode.get()->getPath() == "/" ) {
      createMenu(pNode, menuParent);
    }
  loadMenu(pNode, menuParent);
  if ( asString(option("useImages")) == "true" )
    loadImage(pNode);
}

void WsMenu::loadMenu(NodePtr pNodeParent, WMenu* menuParent)
{
  std::vector<NodePtr> dirNode = pNodeParent.get()->getAll();
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::loadMenu- dirNode getAll size = " << dirNode.size() << " path " << pNodeParent.get()->getPath().string();
  for (std::vector<NodePtr>::iterator it = dirNode.begin(); it != dirNode.end(); ++it) {
    NodePtr curNode = *it;
    if ( asString(option("debug")) == "true" )
      wApp->log("notice") << "WsMenu::loadMenu - curNode path = " << curNode.get()->getPath().string();
    createMenu(curNode, menuParent);
  }
}

void WsMenu::createMenu(NodePtr curNode, WMenu* menuParent)
{
  std::string path2Icon;
  WsUser*     pUser   = WsApp->wsUser();
  std::string sIcon   = curNode.get()->getProperties().get()->get("global", "icon", "");
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::createMenu() icon properties = " << sIcon << " on path : " << curNode.get()->getPath().string();
  if ( sIcon.size() > 1 ) {
    NodePtr tmpNode = curNode;
    if ( tmpNode.get()->isRegularFile() )
      tmpNode   = curNode.get()->getParent();
    if ( tmpNode.get() ) {
      path2Icon = tmpNode.get()->getFullPath().string() + "/ws.res/icones/" + sIcon;
      if ( asString(option("debug")) == "true" )
        wApp->log("notice") << "WsMenu::createMenu() try if icon exists : " << path2Icon;
      if ( !boost::filesystem::exists(path2Icon) )
        path2Icon.clear();
      else {
        boost::algorithm::replace_first(path2Icon, WsApp->docRoot(), "");
        if ( asString(option("debug")) == "true" )
          wApp->log("notice") << "WsMenu::createMenu() match icon " << path2Icon; // /var/www/demo_site/ws.res/icones/house.png"
      }
    }
  }
  if ( asString(option("useButtons")) == "true" ) {
    if ( curNode.get()->getPath().string() != "/" )
      if ( asString(option("useSeparator")) == "true" ) {
        WText* pText = new WText("|", this);
        pText->addStyleClass("WsMenuSep");
      }
    WPushButton* button = new WPushButton(curNode.get()->getDisplayName(true), this);
    m_vPushButton.push_back(button);
    if ( path2Icon.size() > 1 ) {
      button->setIcon(WLink(WLink::Url, path2Icon));
      if ( curNode.get()->getProperties().get()->get("global", "button_text", "true") == "false" )
        button->setText("");
    }
    // TODO : Ameliorer cette fonction
    if ( (curNode.get()->isDirectory() && asString(option("directorySelectable")) == "true") ||
         pUser->isAdministrator() || pUser->isEditor() ||
         (asString(option("showRoot")) == "true" && curNode.get()->getPath() == "/")
       ) {
      button->setLink(WLink(WLink::InternalPath, curNode.get()->getPath().string()));
    }
    if ( curNode.get()->isRegularFile() ) {
      button->setLink(makeLink(curNode.get()->getPath().string(), false));
      if ( button->link().type() == WLink::Url )
        button->setLinkTarget(TargetNewWindow);
    }
    //     LOG(DEBUG)<<"WsMenu :: curnode is : "<<curNode.get()->getPath();
    //     LOG(DEBUG)<<"WsMenu :: curnode sub size : "<<curNode.get()->getDirectories().size();
    bool popupAllowed = (curNode.get()->getProperties().get()->get("global", "allow_popup", "true") == "true" ? true : false);
    if ( curNode.get()->isDirectory() && popupAllowed && asString(option("usePopupMenu")) == "true" ) {
      if ( !(asString(option("noRootPopup")) == "true" && curNode.get()->getPath() == "/") ) {
        WPopupMenu* pPopup = new WPopupMenu();
        pPopup->addStyleClass("wt-no-reparent");
        //           pPopup->resize(200, 150);
        //           pPopup->resize(WLength::Auto, 150);
        //           pPopup->setMaximumSize(WLength::Auto, 150);
        //           pPopup->setMaximumSize(300, 150);
        //           pPopup->setHeight(150);
        loadPopupMenu(curNode, pPopup);
        button->setMenu(pPopup);
        pPopup->setAutoHide(true);
        button->mouseWentOver().connect(boost::bind(&WsMenu::onMouseWentOver, this, button));
        button->setMouseOverDelay(50);
      }
    }
  } else { // No buttons, standard menu
    if ( curNode.get()->getPath().string() != "/" )
      menuParent->addSeparator();
    WMenuItem* pItem = menuParent->addItem(curNode.get()->getDisplayName(true));
    pItem->setLink(WLink(WLink::InternalPath, curNode.get()->getPath().string()));
    if ( path2Icon.size() > 1 )
      pItem->setIcon(path2Icon);
    if ( curNode.get()->isDirectory() && asString(option("usePopupMenu")) == "true" )
      if ( curNode.get()->getDirectories().size() ) {
        WPopupMenu* pPopup = new WPopupMenu();
        pPopup->addStyleClass("wt-no-reparent");
        loadPopupMenu(curNode, pPopup);
        pItem->setMenu(pPopup);
      }
  }
}

void WsMenu::onMouseWentOver(WPushButton* pButton)
{
  if ( !pButton ) return;
  if ( !pButton->menu() ) return;
  WPopupMenu* pPopup = dynamic_cast<WPopupMenu*>(pButton->menu());
  if ( asString(option("align")) == "right" )
    pPopup->popup(pButton, Wt::Horizontal);
  else
    pPopup->popup(pButton, Wt::Vertical);
}

void WsMenu::loadPopupMenu(NodePtr pNodeParent, Wt::WPopupMenu* menuParent)
{
  std::vector<NodePtr> dirNode = pNodeParent->getAll();
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::loadPopupMenu- dirNode getDirectories size = " << dirNode.size() << " path " << pNodeParent->getPath().string();
  for (std::vector<NodePtr>::iterator it = dirNode.begin(); it != dirNode.end(); ++it) {
    NodePtr curNode = *it;
    if ( asString(option("debug")) == "true" )
      wApp->log("notice") << "WsMenu::loadPopupMenu - curNode path = " << curNode.get()->getPath().string();
    bool popupAllowed = (curNode.get()->getProperties().get()->get("global", "allow_popup", "true") == "true" ? true : false);
    if ( curNode.get()->isDirectory() && popupAllowed ) {
      WPopupMenu* pSubPopupMenu = new WPopupMenu();
      loadPopupMenu(curNode, pSubPopupMenu);
      WMenuItem*   pMenuItem = menuParent->addMenu(curNode.get()->getDisplayName(true), pSubPopupMenu);
      if ( asString(option("directorySelectable")) == "true" )
        pMenuItem->setLink(WLink(WLink::InternalPath, curNode.get()->getPath().string()));;
    } else {
      WPopupMenuItem* pPopupItem = menuParent->addItem(curNode.get()->getDisplayName(true));
      pPopupItem->setLink(makeLink(curNode.get()->getPath().string(), true));
      if ( pPopupItem->link().type() == WLink::Url )
        pPopupItem->setLinkTarget(TargetNewWindow);
    }
  }
}

WLink WsMenu::makeLink(const std::string& path, bool bUseIcon)
{
  WsUser*     pUser                   = WsApp->wsUser();
  std::string m_sDocumentRoot         = pUser->getRootPath(); // /var/www/demo_site
  std::string m_httpDocumentRoot      = gdWApp->getParameter("DOCUMENT_ROOT", "/var/www");
  std::string m_sRelativeDocumentRoot = m_sDocumentRoot;
  boost::algorithm::replace_first(m_sRelativeDocumentRoot, m_httpDocumentRoot, ""); // /demo_site
  WLink  lnk;
  std::string     strExt(boost::filesystem::extension(path));
  if ( strExt == "" || strExt == ".fhtml" || strExt == ".rpg" || strExt == ".itpl" || strExt == ".form" || strExt == ".youtube" )
    lnk.setInternalPath(path);
  else {
    if ( strExt == ".url" ) {
      boost::filesystem::path urlFile(m_sDocumentRoot + path);
      wApp->log("notice") << "WsMenu::makeLink path = " << urlFile.string();
      std::string sUrl;
      std::ifstream f(urlFile.string().c_str());
      std::getline(f, sUrl); // url without CR
      boost::algorithm::replace_all(sUrl, "link=", "");
      lnk.setUrl(sUrl);
    } else
      lnk.setUrl(m_sRelativeDocumentRoot + path);
  }
  return lnk;
}

void WsMenu::loadImage(NodePtr pNodeParent)
{
  std::string    endPath(GlobalConfig::PathToImages);
  std::string    curDir = pNodeParent->getFullPath().string() + endPath;
  WApplication::instance()->log("notice") << "WsMenu::loadImage : path = " << curDir;
  if ( !boost::filesystem::exists(curDir) ) return;
  WApplication::instance()->log("notice") << "WsMenu::loadImage : path = " << curDir << " has an image directory";
  WsImages2* pImg = new WsImages2(); //dynamic_cast<WsImages2*>(WsApp->WsModules().module("WsModImages2")->createContents());
  pImg->setOptions(options());
  long delay = asNumber(option("image_delay"));
  if ( delay != -1 )
    pImg->setOption("image_delay", delay);
  pImg->setOption("imageInBackground", true);
  pImg->setOption("reactWhenPathChange", false);
  pImg->setOption("plusWidget", false);
  pImg->setOption("textWidget", false);
  std::string curPathUrl = curDir;
  boost::algorithm::replace_first(curPathUrl, wApp->docRoot(), "");
  WApplication::instance()->log("notice") << "WsMenu::loadImage : setImagesPath to : " << curPathUrl;
  pImg->setOption("imagesPath", std::string(curPathUrl));
  pImg->build();
  if ( !pImg->count() ) {
    delete pImg;
    WApplication::instance()->log("notice") << "WsMenu::load : has no images path = " << pNodeParent->getPath().string();
    return;
  }
  addWidget(pImg);
}

void WsMenu::doPathChanged(std::string newPath)
{
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsMenu::doPathChanged - path = " << newPath;
  if ( asString(option("useInternalPath")) == "true" ) {
    clear();
    doLoadCurPath();
  }
  doSelectedMenu(newPath);
}

void WsMenu::doSelectedMenu(std::string newPath)
{
  return;
  for (int iBut = 0; iBut < m_vPushButton.size(); ++iBut) {
    m_vPushButton[iBut]->removeStyleClass("WsSelected");
    const WLink& lnk = m_vPushButton[iBut]->link();
    std::string curPath;
    if ( lnk.type() == Wt::WLink::InternalPath ) curPath = lnk.internalPath().toUTF8();
    //    if ( lnk.type() == Wt::WLink::Url )          curPath = lnk.url();
    //    if ( lnk.type() == Wt::WLink::Resource )     curPath = lnk.resource()->internalPath(); // TODO : Wich # between internalPath & url ?
    if ( curPath.compare(0, curPath.size(), newPath) != 0 ) continue;
    m_vPushButton[iBut]->addStyleClass("WsSelected");
    wApp->log("notice") << "WsMenu::doSelectedMenu - match in button curPath = " << curPath << " newPath = " << newPath;
    WPopupMenu* pPopup = m_vPushButton[iBut]->menu();
    //    if ( m_vPushButton[iBut]->
  }
}

