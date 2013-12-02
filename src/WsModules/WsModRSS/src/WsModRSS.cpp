/*
 *
 *       Filename:  WsModRSS.cpp
 *
 *    Description:  Zip Wittyshare plugin
 *
 *        Created:  03/04/2013 06:14:15
 *
 *         Author: Guy Deleeuw, Guy.Geleeuw@gmail.com
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
#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <ConfigManager/WsGlobalProperties.h>

#include <WsModule/WsModule.h>

#include "WsModRSS.h"
#include "WsRSS.h"

using namespace Wt;

extern "C" {
  void WsModRSSInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so!\n");
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load wt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

WsModRSS::WsModRSS()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModRSS/Transl/WsModRSS");
  LOG(DEBUG) << "WsModRSS :: end ctor of WsModRSS !";
}

WsModRSS::~WsModRSS()
{
  LOG(DEBUG) << "WsModRSS :: end dtor of WsModRSS !";
}

WWidget* WsModRSS::createContentsMenuBar(WContainerWidget* parent) const
{
  WsModRSS* self =  const_cast<WsModRSS*>(this);
  self->buildMenuBar(parent);
  return 0;
}

WWidget* WsModRSS::createContents(WContainerWidget* parent) const
{
  return 0;
}

WsEditorWidget* WsModRSS::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModRSS::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModRSS::description() const
{
  return "WsModRSS wittyShare module";
}

void WsModRSS::buildMenuBar(WContainerWidget* parent)
{
  LOG(DEBUG) << "WsModRSS:buildMenuBar: ";
  if ( !contentButtonsBar() ) return;
  LOG(DEBUG) << "WsModRSS::buildMenuBar has a menu bar ";
  WsUser*           pUser   = WsApp->wsUser();
  if ( pUser->isAdministrator() || pUser->isEditor() )  {
    WsRSS* m_pRSS = new WsRSS(parent);
    m_pRSS->setOptions(options());
    gdToolbarItem*    pTbItem = contentButtonsBar()->addToolbarItem(wApp->theme()->resourcesUrl() + "wittyshare/Icons/rss.png", "", "Build the RSS file");
    pTbItem->clicked().connect(SLOT(m_pRSS, WsRSS::buildRSS));
  }
}

