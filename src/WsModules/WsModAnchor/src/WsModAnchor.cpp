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

#include "WsModAnchor.h"
#include "WsAnchor.h"

extern "C" {
  void WsModAnchorInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

using namespace Wt;

// Class WsModAnchor
// =====================
WsModAnchor::WsModAnchor()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModAnchor !";
}

WsModAnchor::~WsModAnchor()
{
  if ( asString(option("debug")) == "true" ) // Todo test is now crash if WsOptions is deleted before ?
    LOG(DEBUG) << "WsModAnchor::~WsModAnchor() !";
}

WWidget* WsModAnchor::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModAnchor::createContents(WContainerWidget* parent) const
{
  WsAnchor* pAnchor = new WsAnchor(parent);
  pAnchor->setOptions(options());
  if ( asString(option("debug")) == "true" )
    pAnchor->outOptions("WsModAnchor::createContents()");
  return pAnchor;
}

WsEditorWidget* WsModAnchor::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModAnchor::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModAnchor::description() const
{
  return "WsModAnchor wittyShare module";
}

