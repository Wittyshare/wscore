/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WHBoxLayout>
#include <Wt/WTheme>

#include <Main/WsApplication.h>
#include "Main/WsContentButtonsBar.h"

#include "Main/WsFunctionnalities.h"

using namespace Wt;

WsFunctionnalities::WsFunctionnalities(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  //  if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsFunctionnalities.css");
  addStyleClass("WsFunctionnalities");
  WHBoxLayout*  hbox = new WHBoxLayout();
  hbox->setContentsMargins(0, 0, 0, 0);
  WWidget* w = WsApp->WsModules().module("WsBreadCrumb")->createContents();
  WsOptions* wOpt = dynamic_cast<WsOptions*>(w);
  //    wOpt->setOption("rootPath", path);
  hbox->addWidget(w, 1);
  WsContentButtonsBar* pBB = new WsContentButtonsBar();
  hbox->addWidget(pBB, 0);
  setLayout(hbox);
  //    resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
}

WsFunctionnalities::~WsFunctionnalities()
{ }

