/*
 *
 *       Filename:  WsModLatestNews2.cpp
 *
 *    Description:  Zip Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include <boost/filesystem.hpp>

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <Wt/WDateTime>
#include <Wt/WStandardItemModel>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <ConfigManager/WsGlobalProperties.h>

#include <WsModule/WsModule.h>

#include "WsLatestNews.h"
#include "WsLatestNews2.h"
#include "WsModLatestNews2.h"

using namespace Wt;

extern "C" {
  void WsModLatestNews2Init(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so!\n");
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load wt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

WsModLatestNews2::WsModLatestNews2()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModLatestNews2/Transl/WsModLatestNews2");
  LOG(DEBUG) << "WsModLatestNews2 :: end ctor of WsModLatestNews2 !";
}

WsModLatestNews2::~WsModLatestNews2()
{
  LOG(DEBUG) << "WsModLatestNews2 :: end dtor of WsModLatestNews2 !";
}

WWidget* WsModLatestNews2::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModLatestNews2::createContents(WContainerWidget* parent) const
{
  WsLatestNews2* ln2 = new WsLatestNews2();
  ln2->addStyleClass("WsModLatestNews2");
  ln2->setOptions(options());
  return ln2;
}

WsEditorWidget* WsModLatestNews2::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModLatestNews2::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModLatestNews2::saveEditor() const
{
  return true;
}

std::string WsModLatestNews2::description() const
{
  return "WsModLatestNews2 wittyShare module";
}

