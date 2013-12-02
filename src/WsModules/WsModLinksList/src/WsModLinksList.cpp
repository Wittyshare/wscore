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
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WLink>
#include <Wt/WString>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>
#include <User/WsUser.h>

#include <Main/WsApplication.h>
// TODO : pas top je sais
#include "WsModules/WsModAnchor/src/WsAnchor.h"

#include "WsModLinksList.h"

extern "C" {
  void WsModLinksListInit(void)
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

// Class WsLinksList
// ==================
WsLinksList::WsLinksList(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsLinksList");
}

WsLinksList::~WsLinksList()
{
  if ( asString(option("debug")) == "true" )
    LOG(DEBUG) << "WsLinksList::~WsLinksList() !";
}

void WsLinksList::load()
{
  Wt::WContainerWidget::load();
  if ( asString(option("debug")) == "true" )
    Wt::WApplication::instance()->log("notice") << "WsLinksList::load()";
  if ( asString(option("debug")) == "true" )
    outOptions("WsLinksList::load()");
  std::string sClass = asString(option("class")).toUTF8();
  if ( sClass.size() > 0 )
    addStyleClass(sClass);
  std::string title = asString(option("title")).toUTF8();
  if ( title.size() > 0 ) {
    WText* pText = new WText(title);
    pText->addStyleClass("WsTitle");
    addWidget(pText);
  }
  std::string linkType = asString(option("linkType")).toUTF8(); // Url, Resource, InternalPath
  std::string link     = asString(option("link")).toUTF8();
  std::string text     = asString(option("text")).toUTF8();
  std::string icon     = asString(option("icon")).toUTF8();
  std::string iconText = asString(option("iconText")).toUTF8();
  std::string target   = asString(option("target")).toUTF8(); // TargetSelf, TargetThisWindow, TargetNewWindow
  if ( linkType.size() < 1 ) linkType = "InternalPath";
  WsUser* pUser = WsApplication::wsInstance()->wsUser();
  std::string fullRootPath = pUser->getRootPath();
  std::string rootUrl  = fullRootPath;
  boost::replace_all(rootUrl, wApp->docRoot(), "");
  std::string path = WsApplication::wsInstance()->internalPath();
  std::string sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(path);
  std::string rootPath = asString(option("rootPath")).narrow();
  boost::algorithm::replace_all(rootPath, "&amp;",  "&");
  if ( rootPath.size() < 1 ) rootPath = path;
  NodePtr root = pUser->getAccessRoot();
  if ( !root ) {
    LOG(DEBUG) << "WsLinksList::load() cannot get root !";
    return;
  }
  NodePtr startNode = root->eatPath(rootPath);
  if (startNode.get() == 0) {
    LOG(DEBUG) << "WsLinksList::load() cannot eatPath !" << rootPath;
    return;
  }
  LOG(DEBUG) << "WsLinksList::load() start on rootPath = " << rootPath;
  std::vector<NodePtr> dirNode          = startNode.get()->getFiles();
  for (std::vector<NodePtr>::iterator it = dirNode.begin(); it != dirNode.end(); ++it) {
    NodePtr curNode = *it;
    boost::filesystem::path path(curNode.get()->getPath());
    if ( ! ( path.extension() == ".jpeg"
             || path.extension() == ".jpg"
             || path.extension() == ".png"
           ) ) continue;
    if ( curNode.get()->isDirectory() )        continue;
    std::string pngPath =  curNode.get()->getPath().string();
    LOG(DEBUG) << "WsLinksList::load() node !" << pngPath;
    boost::filesystem::path pdfFile = pngPath;
    pdfFile.replace_extension(".pdf");
    std::string sFileObject = pdfFile.string();
    if ( sFileObject.size() < 1 ) continue;
    if ( asString(option("debug")) == "true" )
      wApp->log("notice") << "sLinksList::load() pdf/zip file = " << sFileObject;
    std::string     linkType = "InternalPath";
    std::string     sUrl;
    NodePtr pNode       = root->eatPath(sFileObject);
    if ( !pNode.get() ) {
      pdfFile.replace_extension(".zip");
      sFileObject = pdfFile.string();
      pNode       = root->eatPath(sFileObject);
      if ( !pNode.get() ) {
        pdfFile.replace_extension(".url");
        sFileObject = pdfFile.string();
        pNode       = root->eatPath(sFileObject);
        if ( !pNode )
          continue;
        std::string conct = fullRootPath + sFileObject;
        std::ifstream f(conct.c_str());
        std::getline(f, sUrl); // url without CR
        boost::algorithm::replace_all(sUrl, "link=", "");
        sFileObject = sUrl;
        linkType = "Url";
      }
    }
    std::string        sShortDesc = pNode.get()->getProperties().get()->get("global", "short_description", "");
    WsAnchor* pAnchor = dynamic_cast<WsAnchor*>(WsApp->WsModules().module("WsModAnchor")->createContents());
    pAnchor->setOptions(options());
    pAnchor->setOption("linkType", linkType);
    pAnchor->setOption("link",     sFileObject);
    bool bUseExt = false;
    if ( asString(option("useExt")) == "true" ) bUseExt = true;
    if ( asString(option("useText")) == "false" )
      pAnchor->setOption("text",  std::string());
    else
      pAnchor->setOption("text",     pNode.get()->getDisplayName(bUseExt));
    pAnchor->setOption("icon",     std::string(rootUrl + pngPath));
    if ( asString(option("useText")) == "false" )
      pAnchor->setOption("iconText", std::string());
    else
      pAnchor->setOption("iconText", pNode.get()->getDisplayName(bUseExt));
    pAnchor->setOption("target",   std::string("TargetNewWindow"));
    if ( asString(option("useShortDesc")) == "false" )
      pAnchor->setOption("title",  std::string());
    else
      pAnchor->setOption("title",    sShortDesc);
    //    pAnchor->load();
    addWidget(pAnchor);
  }
}



// Class WsModLinksList
// =====================
WsModLinksList::WsModLinksList()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModLinksList !";
}

WsModLinksList::~WsModLinksList()
{
  if ( asString(option("debug")) == "true" ) // Todo test is now crash if WsOptions is deleted before ?
    LOG(DEBUG) << "WsModLinksList::~WsModLinksList() !";
}

WWidget* WsModLinksList::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModLinksList::createContents(WContainerWidget* parent) const
{
  WsLinksList* pLinksList = new WsLinksList(parent);
  pLinksList->setOptions(options());
  if ( asString(option("debug")) == "true" )
    pLinksList->outOptions("WsModLinksList::createContents()");
  return pLinksList;
}

WsEditorWidget* WsModLinksList::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModLinksList::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModLinksList::saveEditor() const
{
  return true;
}

std::string WsModLinksList::description() const
{
  return "WsModLinksList wittyShare module";
}

