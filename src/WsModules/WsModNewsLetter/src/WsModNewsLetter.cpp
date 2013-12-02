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
#include <Wt/WRegExpValidator>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

#include "WsModNewsLetter.h"
#include "WsNewsLetter.h"

extern "C" {
  void WsModNewsLetterInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

// Class WsModNewsLetter
// =====================
WsModNewsLetter::WsModNewsLetter()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModNewsLetter !";
}

WsModNewsLetter::~WsModNewsLetter()
{
  if ( asString(option("debug")) == "true" ) // Todo test is now crash if WsOptions is deleted before ?
    LOG(DEBUG) << "WsModNewsLetter::~WsModNewsLetter() !";
}

WWidget* WsModNewsLetter::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModNewsLetter::createContents(WContainerWidget* parent) const
{
  WsNewsLetter* pWsNewsLetter = new WsNewsLetter(parent);
  pWsNewsLetter->setOptions(options());
  if ( asString(option("debug")) == "true" )
    pWsNewsLetter->outOptions("WsModNewsLetter::createContents()");
  return pWsNewsLetter;
}

WsEditorWidget* WsModNewsLetter::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModNewsLetter::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModNewsLetter::saveEditor() const
{
  return true;
}

std::string WsModNewsLetter::description() const
{
  return "WsModNewsLetter wittyShare module";
}

std::string WsModNewsLetter::checkPath(const std::string& currentPath)
{
  std::string newPath = WsModule::checkPath(currentPath);
  WsNewsLetter* pWsNewsLetter = new WsNewsLetter();
  pWsNewsLetter->setOptions(options());
  newPath = pWsNewsLetter->checkPath(currentPath);
  delete pWsNewsLetter;
  return newPath;
}

