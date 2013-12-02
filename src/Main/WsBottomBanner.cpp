/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include "WsApplication.h"
#include "WsBottomBanner.h"

using namespace Wt;

WsBottomBanner::WsBottomBanner(const std::string& rText = std::string(), WContainerWidget* parent)
  : WContainerWidget(parent), m_pWText(0), m_bDebug(false)
{
  //if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsBottomBanner.css");
  addStyleClass("WsBottomBanner");
  m_pWText = new WText("", this);
  setText(rText);
}

WsBottomBanner::~WsBottomBanner()
{ }

const std::string& WsBottomBanner::text() const
{
  return m_pString;
}

void WsBottomBanner::setText(const std::string& rText)
{
  m_pString = rText;
  WsUser*         pUser           = WsApp->wsUser();
  std::string     m_sDocumentRoot = pUser->getRootPath(); // /var/www/demo_site
  std::string     sFile           = m_sDocumentRoot + rText;
  std::string     fileContent;
  if ( m_bDebug )
    wApp->log("notice") << " WsBottomBanner::setText sFile = " << sFile;
  if ( !gdcore_file_content2string(sFile.c_str(), fileContent) ) {
    wApp->log("notice") << " WsBottomBanner::setText : cannot open : " << sFile;
    return;
  }
  m_pWText->setText(fileContent);
}

