/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/filesystem/operations.hpp>

#include <Include/WsGlobalConfig.h>

#include <Wt/WLogger>
#include <Wt/WTheme>
#include <Wt/WText>

#include <Main/WsApplication.h>

// TODO : pas top je sais
#include "WsModules/WsModImages2/src/WsImages2.h"

#include "WsBanners.h"

WsBanners::WsBanners(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsBanners");
}

void WsBanners::load()
{
  WContainerWidget::load();
  if ( Wt::asString(option("debug")) == "true" )
    wApp->log("notice") << "WsBanners::load() start";
  if ( Wt::asString(option("useInternalPath")) == "true" ) {
    wApp->internalPathChanged().connect(SLOT(this, WsBanners::doPathChanged));
    if ( Wt::asString(option("debug")) == "true" )
      wApp->log("notice") << "WsBanners::load() using internalPath";
  }
  doPathChanged(wApp->internalPath());
  if ( Wt::asString(option("debug")) == "true" )
    wApp->log("notice") << "WsBanners::load() end";
}

void WsBanners::doPathChanged(std::string newPath)
{
  if ( Wt::asString(option("debug")) == "true" )
    wApp->log("notice") << "WsBanners::doPathChanged - path = " << newPath;
  clear();
  std::string             sPathWithoutPrefix = WsApp->WsModules().pathWithoutPrefix(newPath); // ex. /SiteMap
  WsUser*                 pUser         = WsApp->wsUser();
  boost::filesystem::path root          = pUser->getRootPath();
  std::string             sCurrent      = root.string() + sPathWithoutPrefix; // root.string() retourne un / a la fin ?
  NodePtr         startNode     = pUser->getAccessRoot();
  std::string imgPath = Wt::asString(option("imagesPath")).toUTF8();
  boost::algorithm::replace_all(imgPath, "&amp;", "&");
  if ( imgPath.size() > 0 ) {
    wApp->log("notice") << "WsBanners::doPathChanged - forcing imagesPath = " << imgPath;
    WsImages2* pImg  = new WsImages2(this);
    pImg->setOptions(options());
    std::string s = root.string() + imgPath;
    boost::replace_all(s, wApp->docRoot(), "");
    pImg->setOption("imagesPath", s); // TODO : move this to WsImages2
    pImg->build();
    pImg->load();
    addWidget(pImg);
    return;
  }
  boost::replace_all(sCurrent, "//", "/");
  boost::filesystem::path current(sCurrent);
  // Check that the path is a directory, if not take the parent
  if ( !boost::filesystem::is_directory(current) )
    current = current.parent_path();
  if ( Wt::asString(option("debug")) == "true" )
    wApp->log("notice") << "WsBanners::doPathChanged() start " << current.string();
  while (1) {
    sCurrent = current.string() + "/ws.res/images/WsBanners";
    boost::replace_all(sCurrent, "//", "/");
    boost::filesystem::path banners(sCurrent);
    // Check if the current directory has a sub directory ws.res/images/WsBanners
    if ( boost::filesystem::is_directory(banners) ) {
      if ( Wt::asString(option("debug")) == "true" )
        wApp->log("notice") << "WsBanners::doPathChanged() exist " << banners.string();
      boost::filesystem::directory_iterator end_itr;
      for (boost::filesystem::directory_iterator itr_dir(banners); itr_dir != end_itr; ++itr_dir) {
        if (  (  itr_dir->path().extension() == ".jpeg"
                 || itr_dir->path().extension() == ".jpg"
                 || itr_dir->path().extension() == ".png"
              ) ) {
          string str_bann = banners.string();
          boost::replace_all(str_bann, "//", "/");
          boost::replace_all(str_bann, WsApp->docRoot(), "");
          if ( Wt::asString(option("debug")) == "true" )
            wApp->log("notice") << "WsBanners::doPathChanged() Image(s) found : setOption " << str_bann;
          WsImages2* pImg  = new WsImages2(this);
          pImg->setOptions(options());
          pImg->setOption("imagesPath", str_bann);
          pImg->build();
          pImg->load();
          addWidget(pImg);
          return;
        }
      }
    }
    if (current == root) break;
    //
    // Check if the current directory allow up to parrent on the banners
    std::string noRoot = current.string();
    boost::replace_all(noRoot, root.string(), "");
    if ( Wt::asString(option("debug")) == "true" )
      wApp->log("notice") << "WsBanners::doPathChanged() noRoot " << noRoot;
    NodePtr currentNode = startNode.get()->eatPath(noRoot);
    if ( currentNode )
      if ( currentNode.get()->getProperties().get()->get("global", "banners_up", "true") == "false" ) break;
    current = current.parent_path();
  }
  wApp->log("notice") << "WsBanners::doPathChanged() NO Image(s) found";
}

