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

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

#include "WsModBreadCrumb.h"
#include "WsBreadCrumb.h"

extern "C" {
  void WsModBreadCrumbInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

// Class WsModBreadCrumb
// =====================
WsModBreadCrumb::WsModBreadCrumb()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModBreadCrumb !";
}

WsModBreadCrumb::~WsModBreadCrumb()
{
  if ( asString(option("debug")) == "true" ) // Todo test is now crash if WsOptions is deleted before ?
    LOG(DEBUG) << "WsModBreadCrumb::~WsModBreadCrumb() !";
}

WWidget* WsModBreadCrumb::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModBreadCrumb::createContents(WContainerWidget* parent) const
{
  WsBreadCrumb* pWsBreadCrumb = new WsBreadCrumb(parent);
  pWsBreadCrumb->setOptions(options());
  if ( asString(option("debug")) == "true" )
    pWsBreadCrumb->outOptions("WsModBreadCrumb::createContents()");
  return pWsBreadCrumb;
}

WsEditorWidget* WsModBreadCrumb::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModBreadCrumb::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModBreadCrumb::description() const
{
  return "WsModBreadCrumb wittyShare module";
}

