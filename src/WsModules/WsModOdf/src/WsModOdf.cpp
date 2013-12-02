/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WText>

#include <Logger/WsLogger.h>

#include "WsModOdf.h"

extern "C" {
  void WsModOdfInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so %s!\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

WsOdf::WsOdf(WContainerWidget* parent)
  : WContainerWidget(parent)
{ }

WsOdf::~WsOdf()
{ }

void WsOdf::setDiffPath(const std::string& diffPath)
{
  m_sDiffPath = diffPath;
}

void WsOdf::load()
{
  WContainerWidget::load();
  setId("odf");
  resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
  setOverflow(WContainerWidget::OverflowAuto);
  std::string  p1 = Wt::WApplication::instance()->internalPath();
  WApplication::instance()->log("notice") << "WsOdf::load() internalPath : " << p1; // /about/wt.odt
  //std::string  p = "/demo_site/about/wt.odt";
  // TODO : check security level
  std::string  p(m_sDiffPath + p1);
  WApplication::instance()->log("notice") << "diffPath : " << p;
  if ( p.size() > 0 ) {
    std::string javaScript =  "var odfelement = document.getElementById('odf'); odfcanvas = new odf.OdfCanvas(odfelement); odfcanvas.load('";
    javaScript += p;
    javaScript += "')";
    WApplication::instance()->log("notice") << "WsOdf::load() execute this javascript : " << javaScript;
    WApplication::instance()->doJavaScript(javaScript);
  }
}








WsModOdf::WsModOdf()
  : WsModule()
{
  // Add the required css
  WApplication::instance()->useStyleSheet(WApplication::instance()->resourcesUrl() + "webodf/webodf.css");
  // Add the required javascript file
  WApplication::instance()->require(WApplication::instance()->resourcesUrl() + "webodf/webodf.js", "webodf");
  LOG(DEBUG) << "WsModOdf::WsModOdf() !";
}

WsModOdf::~WsModOdf()
{
  LOG(DEBUG) << "WsModOdf::~WsModOdf() !";
}

WWidget* WsModOdf::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModOdf::createContents(WContainerWidget* parent) const
{
  WsOdf* pcw = new WsOdf(parent);
  pcw->setDiffPath(diffPath());
  pcw->setOptions(options());
  return pcw;
}

WsEditorWidget* WsModOdf::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModOdf::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModOdf::saveEditor() const
{
  return true;
}

std::string WsModOdf::description() const
{
  return "WsModOdf wittyShare module";
}

