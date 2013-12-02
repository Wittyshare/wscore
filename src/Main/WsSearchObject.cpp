/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WHBoxLayout>
#include <Wt/WTheme>

#include "WsLogo.h"
#include "WsSiteMap.h"

#include "WsSearchObject.h"

#include <ConfigManager/WsLayoutProperties.h>
#include <Include/WsGlobalConfig.h>

using namespace Wt;

WsSearchObject::WsSearchObject(WContainerWidget* parent)
  : gdWSearch(parent)
{
  wApp->log("notice") << "WsSearchObject::WsSearchObject CTOR";
  m_searchPath = WsLayoutProperties::instance()->get("global", "search_path", "/Search");
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsSearchObject.css");
  addStyleClass("WsSearchObject");
  setText("Search");
  searchValidated().connect(SLOT(this, WsSearchObject::doSearch));
  searchFocussed().connect(SLOT(this, WsSearchObject::doFocussed));
  searchBlurred().connect(SLOT(this, WsSearchObject::doBlurred));
}

WsSearchObject::~WsSearchObject()
{ }

void WsSearchObject::doSearch(WString sSearch)
{
  std::string  mySearch(sSearch.toUTF8());
  if ( mySearch == "Search" ) return;
  if ( mySearch.size() <  1 ) return;
  wApp->setInternalPath(m_searchPath + "/" + mySearch, true);
  wApp->log("notice") << "WsSearchObject::doSearch - : " << mySearch;
}

void WsSearchObject::doFocussed(Wt::WString sSearch)
{
  //  std::string  mySearch(sSearch.toUTF8());
  //  if ( mySearch != "Search" ) return;
  if ( sSearch != "Search" ) return;
  setText("");
}

void WsSearchObject::doBlurred(Wt::WString sSearch)
{
  setText("Search");
}
