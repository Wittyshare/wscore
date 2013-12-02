/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <Wt/WLogger>
#include <Wt/WText>
#include <Wt/WTheme>

#include <Main/WsApplication.h>
#include <Logger/WsLogger.h>

#include "WsModGalleria.h"

extern "C" {
  void WsModGalleriaInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so %s!\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

using namespace Wt;

WsGalleria::WsGalleria(WContainerWidget* parent)
  : WContainerWidget(parent)
{ }

WsGalleria::~WsGalleria()
{ }

void WsGalleria::load()
{
  WContainerWidget::load();
  setId("galleria");
  std::string  galleriaPath = WApplication::instance()->internalPath();
  wApp->log("notice") << "WsGalleria::load() internalPath : " << galleriaPath; // /about/galleria
  boost::algorithm::replace_last(galleriaPath, "galleria", "ws.res/galleria");
  wApp->log("notice") << "WsGalleria::load() galleriaPath : " << galleriaPath; // /about/ws.res/galleria
  WsUser* user = WsApplication::wsInstance()->wsUser();
  std::string fullGalleriaPath = user->getRootPath() + galleriaPath;
  wApp->log("notice") << "WsGalleria::load() fullGalleriaPath : " << fullGalleriaPath; // /var/www/demo_site/about/ws.res/galleria
  boost::filesystem::directory_iterator end_itr;
  for (boost::filesystem::directory_iterator itr_dir(fullGalleriaPath + "/thumbnails"); itr_dir != end_itr; ++itr_dir) {
    if (   itr_dir->path().extension() == ".jpeg"
           || itr_dir->path().extension() == ".jpg"
           || itr_dir->path().extension() == ".png"
       ) {
      string img      = itr_dir->path().string();
      boost::replace_first(img, WsApp->docRoot(), ""); // /demo_site/about/ws.res/galleria/thumbnails/img01.png
      boost::algorithm::replace_all(img, "&", "&amp;");
      string imgThumb = img;
      boost::replace_last(imgThumb, "thumbnails", "images"); // /demo_site/about/ws.res/galleria/images/img01.png
      std::string text = "<a href=\""
                         + img
                         + "\"><img src=\""
                         + imgThumb
                         + "\" data-title=\"My title\" data-description=\"My description\"/></a>";
      std::cout << text << std::endl;
      addWidget(new WText(text, Wt::PlainText));
    }
  }
  std::string javaScript  = "Galleria.loadTheme('";
  javaScript += wApp->resourcesUrl();
  javaScript += "galleria/themes/classic/galleria.classic.min.js'); \n";
  javaScript += "Galleria.run('#galleria');";
  WApplication::instance()->log("notice") << "WsGalleria::load() execute this javascript : " << javaScript;
  WApplication::instance()->doJavaScript(javaScript);
}








WsModGalleria::WsModGalleria()
  : WsModule()
{
  // Add the required css
  //  WApplication::instance()->useStyleSheet(WApplication::instance()->resourcesUrl() + "webodf/webodf.css");
  // Add the required javascript file
  WApplication::instance()->require("http://ajax.googleapis.com/ajax/libs/jquery/1/jquery.js");
  WApplication::instance()->require(WApplication::instance()->resourcesUrl() + "galleria/galleria-1.2.9.min.js", "galleria");
  LOG(DEBUG) << "WsModGalleria::WsModGalleria() !";
}

WsModGalleria::~WsModGalleria()
{
  LOG(DEBUG) << "WsModGalleria::~WsModGalleria() !";
}

WWidget* WsModGalleria::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModGalleria::createContents(WContainerWidget* parent) const
{
  WsGalleria* pcw = new WsGalleria(parent);
  pcw->setOptions(options());
  return pcw;
}

WsEditorWidget* WsModGalleria::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModGalleria::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModGalleria::description() const
{
  return "WsModGalleria wittyShare module";
}

