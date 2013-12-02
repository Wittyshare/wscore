/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WTheme>

#include <gdwtcore/gdToolbar.h>

#include "WsSiteMap.h"

#include <ConfigManager/WsLayoutProperties.h>
#include <Include/WsGlobalConfig.h>

using namespace Wt;

WsSiteMap::WsSiteMap(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  wApp->log("notice") << "WsSiteMap::WsSiteMap CTOR";
  //if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsSiteMap.css");
  addStyleClass("WsSiteMap");
  gdToolbarItem* pTb = gdToolbar::newToolbarItem("", "Site Map", "");
  pTb->clicked().connect(SLOT(this, WsSiteMap::doSiteMapClicked));
  addWidget(pTb);
}

WsSiteMap::~WsSiteMap()
{ }

void WsSiteMap::doSiteMapClicked()
{
  wApp->setInternalPath("/SiteMap", true);
}

