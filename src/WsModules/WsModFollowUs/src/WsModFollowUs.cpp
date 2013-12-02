/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/filesystem.hpp>

#include <Wt/WLogger>
#include <Wt/WText>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>
#include <User/WsUser.h>

#include <Main/WsApplication.h>

#include "WsModules/WsModAnchor/src/WsAnchor.h"

#include "WsModFollowUs.h"

extern "C" {
  void WsModFollowUsInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

// Class WsFollowUs
// ==================
WsFollowUs::WsFollowUs(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsFollowUs");
}

WsFollowUs::~WsFollowUs()
{
  LOG(DEBUG) << "WsFollowUs::~WsFollowUs() !";
}

void WsFollowUs::load()
{
  WContainerWidget::load();
  WsUser* pUser = WsApplication::wsInstance()->wsUser();
  std::string rootPath = pUser->getRootPath();
  std::string rootUrl  = rootPath;
  boost::replace_all(rootUrl, wApp->docRoot(), "");
  addWidget(new WText("Follow us"));
  WsAnchor* pAnchorRss = new WsAnchor();
  pAnchorRss->addStyleClass("WsRss");
  pAnchorRss->setOption("debug",    std::string("true"));
  pAnchorRss->setOption("linkType", std::string("InternalPath"));
  pAnchorRss->setOption("link",     std::string(rootUrl + "/ws.res/Eurofer.rss"));
  pAnchorRss->setOption("text",     std::string("RSS"));
  pAnchorRss->setOption("icon",     std::string(rootUrl + "/ws.res/icones/RSS.png"));
  pAnchorRss->setOption("iconText", std::string("Eurofer RSS"));
  pAnchorRss->setOption("target",   std::string("TargetNewWindow"));
  addWidget(pAnchorRss);
  WsAnchor* pAnchorTwitter = new WsAnchor();
  pAnchorTwitter->addStyleClass("WsTwitter");
  pAnchorTwitter->setOption("debug", std::string("true"));
  pAnchorTwitter->setOption("linkType", std::string("Url"));
  pAnchorTwitter->setOption("link", std::string("https://twitter.com/Eurofer_eu"));
  pAnchorTwitter->setOption("text", std::string("Follow us"));
  pAnchorTwitter->setOption("icon", std::string(rootUrl + "/ws.res/icones/twitter.png"));
  pAnchorTwitter->setOption("iconText", std::string("Eurofer on twitter"));
  pAnchorTwitter->setOption("target", std::string("TargetNewWindow"));
  addWidget(pAnchorTwitter);
}


// Class WsModFollowUs
// =====================
WsModFollowUs::WsModFollowUs()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModFollowUs !";
}

WsModFollowUs::~WsModFollowUs()
{
  //  WApplication::instance()->log("notice") << "end dtor of WsModFollowUs !";
}

WWidget* WsModFollowUs::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModFollowUs::createContents(WContainerWidget* parent) const
{
  WsFollowUs* pFU = new WsFollowUs(parent);
  wApp->log("notice") <<  "WsModFollowUs::createContents() : prepare options ";
  pFU->setOptions(options());
  pFU->outOptions("WsModFollowUs::createContents()");
  return pFU;
}

WsEditorWidget* WsModFollowUs::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModFollowUs::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModFollowUs::description() const
{
  return "WsModFollowUs wittyShare module";
}

