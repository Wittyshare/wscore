/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <Wt/WLogger>
#include <Wt/WMenu>
#include <Wt/WPopupMenu>
#include <Wt/WTheme>

#include <Main/WsApplication.h>

#include "WsMenu.h"
#include "WsModMenu.h"

extern "C" {
  void WsModMenuInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}


WsModMenu::WsModMenu()
  : WsModule()
{
  LOG(DEBUG) << "WsModMenu::WsModMenu() !";
}

WsModMenu::~WsModMenu()
{
  LOG(DEBUG) << "WsModMenu::~WsModMenu() !";
}

WWidget* WsModMenu::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMenu::createContents(WContainerWidget* parent) const
{
  WsMenu* pMenu = new WsMenu("/", parent);
  pMenu->setOptions(options());
  if ( asString(option("debug")) == "true" )
    pMenu->outOptions("WsModMenu::createContents()");
  return pMenu;
}

WsEditorWidget* WsModMenu::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMenu::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModMenu::saveEditor() const
{
  return true;
}

std::string WsModMenu::description() const
{
  return "WsModMenu wittyShare module";
}

