/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WHBoxLayout>

#include "WsLogo.h"
#include "WsSiteMap.h"
#include "WsSearchObject.h"
#include <Wt/WTheme>

#include "WsTopBanner.h"

#include <ConfigManager/WsLayoutProperties.h>
#include <Include/WsGlobalConfig.h>

using namespace Wt;

WsTopBanner::WsTopBanner(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  wApp->log("notice") << "WsTopBanner::WsTopBanner CTOR";
  //  if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsTopBanner.css");
  addStyleClass("WsopBanner");
  WHBoxLayout* hbox = new WHBoxLayout();
  hbox->setContentsMargins(0, 0, 0, 0);
  if ( WsLayoutProperties::instance()->get("global", "topbanner_use_logo", "true") == "true" ) {
    WsLogo* pLogo = new WsLogo();
    WContainerWidget* cw = new WContainerWidget();
    cw->addWidget(pLogo);
    hbox->addWidget(cw, 1, AlignLeft);
  }
  if ( WsLayoutProperties::instance()->get("global", "topbanner_use_sitemap", "true") == "true" ) {
    WsSiteMap* pSiteMap = new WsSiteMap();
    hbox->addWidget(pSiteMap, 0, AlignRight);
  }
  if ( WsLayoutProperties::instance()->get("global", "topbanner_use_search", "true") == "true" ) {
    WsSearchObject* pSearch = new WsSearchObject();
    hbox->addWidget(pSearch, 0, Wt::AlignTop | Wt::AlignRight);
  }
  setLayout(hbox);
}

WsTopBanner::~WsTopBanner()
{ }

