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
#include <Wt/WAnchor>
#include <Wt/WImage>
#include <Wt/WLink>
#include <Wt/WString>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

#include "WsAnchor.h"

using namespace Wt;

// Class WsAnchor
// ==================
WsAnchor::WsAnchor(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsAnchor");
}

WsAnchor::~WsAnchor()
{
  if ( asString(option("debug")) == "true" )
    LOG(DEBUG) << "WsAnchor::~WsAnchor() !";
}

void WsAnchor::load()
{
  Wt::WContainerWidget::load();
  if ( asString(option("debug")) == "true" )
    Wt::WApplication::instance()->log("notice") << "WsAnchor::load()";
  if ( asString(option("debug")) == "true" )
    outOptions("WsAnchor::load()");
  std::string linkType = asString(option("linkType")).toUTF8(); // Url, Resource, InternalPath
  std::string link     = asString(option("link")).toUTF8();
  std::string text     = asString(option("text")).toUTF8();
  std::string icon     = asString(option("icon")).toUTF8();
  std::string iconText = asString(option("iconText")).toUTF8();
  std::string target   = asString(option("target")).toUTF8(); // TargetSelf, TargetThisWindow, TargetNewWindow
  std::string title    = asString(option("title")).toUTF8();
  if ( linkType.size() < 1 ) linkType = "InternalPath";
  if ( title.size() > 0 ) {
    WText* ptext = new WText(title);
    ptext->addStyleClass("WsTitle");
    addWidget(new WText(title));
  }
  WLink  wLink;
  if ( linkType == "Url" )           wLink.setUrl(link);
  //  if ( linkType == "Resource" )      wLink.setResource(link);
  if ( linkType == "InternalPath" )  wLink.setInternalPath(link);
  WAnchor* pAnchor = new WAnchor(wLink, text);
  if ( icon.size() > 0 ) {
    WImage* pImage = new WImage(WLink(WLink::Url, icon), iconText, 0);
    pAnchor->setImage(pImage);
  }
  if ( target == "TargetNewWindow" )
    pAnchor->setTarget(Wt::TargetNewWindow);
  if ( target == "TargetSelf" )
    pAnchor->setTarget(Wt::TargetSelf);
  if ( target == "TargetThisWindow" )
    pAnchor->setTarget(Wt::TargetThisWindow);
  addWidget(pAnchor);
}

