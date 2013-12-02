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

#include "WsModBanners.h"
#include "WsBanners.h"

extern "C" {
  void WsModBannersInit(void)
  {
    fprintf(stderr, "WsModBannersInit : preparing required libraries ==> libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "WsModBannersInit : cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

WsModBanners::WsModBanners()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModBanners/Transl/WsModBanners");
  LOG(DEBUG) << "WsModBanners::WsModBanners end ctor!";
}

WsModBanners::~WsModBanners()
{
  LOG(DEBUG) << "WsModBanners::~WsModBanners : dtor";
}

WWidget* WsModBanners::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModBanners::createContents(WContainerWidget* parent) const
{
  WsBanners* pBan = new WsBanners(parent);
  pBan->setOptions(options());
  return pBan;
}

WsEditorWidget* WsModBanners::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModBanners::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModBanners::description() const
{
  return "WsModBanners wittyShare module";
}

