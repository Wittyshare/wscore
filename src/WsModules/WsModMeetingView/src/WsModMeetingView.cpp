/*
 *
 *       Filename:  WsModMeetingView.cpp
 *
 *    Description:  WsModMeetingView : a module to render Asterisk running conference call
 *
 *        Created:  16/01/2013 11:07:15
 *
 *         Author: Guy Deleeuw : guy.deleeuw@gmail.com
 *
 */

#include <boost/filesystem.hpp>

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <gdcore/bdAsterisk/ConnectionHandler.h>
#include <gdcore/bdAsterisk/MeetMeList.h>
#include <gdcore/bdAsterisk/MeetMeListAnswer.h>
#include <gdcore/bdAsterisk/MeetMeMuteUnMute.h>

#include <Wt/WApplication>
#include <Wt/WBoostAny>

#include <gdwtcore/WAsterisk/gdMeetingView.h>

#include <Logger/WsLogger.h>

#include "WsModMeetingView.h"

using namespace Wt;

extern "C" {
  void WsModMeetingViewInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so!\n");
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "WsModMeetingViewInit : cannot load wt.so shared library! %s\n", dlerror());
      return;
    }
  }
}




WsMeetingView::WsMeetingView(Wt::WContainerWidget* parent)
  : gdMeetingView(parent)
{
  LOG(DEBUG) << "WsMeetingView::WsMeetingView() !";
}

WsMeetingView::~WsMeetingView()
{
  LOG(DEBUG) << "WsMeetingView::~WsMeetingView() !";
}

void WsMeetingView::load()
{
  LOG(DEBUG) << "WsMeetingView::~WsMeetingView() !";
  long rowHeight = asNumber(option("rowHeight"));
  if ( rowHeight > 0 )
    setRowHeight(rowHeight);
  std::string  sHideHeader = asString(option("hideHeader")).narrow();
  if ( sHideHeader == "true" )
    hideHeader(true);
  hideColumns(asString(option("hideColumns")).narrow());
  setServer(asString(option("server")).narrow());
  long lPort = asNumber(option("port"));
  if ( lPort > 0 )
    setRowHeight(lPort);
  setRootNumber(asString(option("rootNumber")).narrow());
  setUser(asString(option("user")).narrow());
  setPassword(asString(option("password")).narrow());
  long lConf = asNumber(option("conferenceID"));
  if ( lConf > 0 )
    setConferenceID(lConf);
  gdMeetingView::load();
}



WsModMeetingView::WsModMeetingView()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModMeetingView/Transl/WsModMeetingView");
  LOG(DEBUG) << "WsModMeetingView::WsModMeetingView() !";
}

WsModMeetingView::~WsModMeetingView()
{
  LOG(DEBUG) << "WsModMeetingView::~WsModMeetingView() !";
}

WWidget* WsModMeetingView::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMeetingView::createContents(WContainerWidget* parent) const
{
  wApp->log("notice") <<  "WsModMeetingView::createContents : start";
  WsMeetingView* dirView = new WsMeetingView(parent);
  dirView->setOptions(options());
  return dirView;
}

WsEditorWidget* WsModMeetingView::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModMeetingView::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModMeetingView::saveEditor() const
{
  return true;
}

std::string WsModMeetingView::description() const
{
  return "WsModMeetingView wittyShare module";
}

