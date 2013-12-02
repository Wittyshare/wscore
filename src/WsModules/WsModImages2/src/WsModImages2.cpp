/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <Wt/WLogger>
#include <Wt/WTheme>

#include <Logger/WsLogger.h>
#include <Main/WsApplication.h>

#include "WsModImages2.h"
#include "WsImages2.h"

extern "C" {
  void WsModImages2Init(void)
  {
    fprintf(stderr, "WsModImages2Init : preparing required libraries ==> libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "WsModImages2Init : cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

WsModImages2::WsModImages2()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModImages2/Transl/WsModImages2");
  LOG(DEBUG) << "WsModImages2::WsModImages2 end ctor!";
}

WsModImages2::~WsModImages2()
{
  LOG(DEBUG) << "WsModImages2::~WsModImages2 : dtor";
}

WWidget* WsModImages2::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModImages2::createContents(WContainerWidget* parent) const
{
  WsImages2* pImg  = new WsImages2(parent);
  pImg->setOptions(options());
  return pImg;
}

WsEditorWidget* WsModImages2::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModImages2::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModImages2::description() const
{
  return "WsModImages2 wittyShare module";
}

