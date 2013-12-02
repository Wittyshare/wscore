/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WTheme>

#include <gdcore/gdImage.h>

#include <ConfigManager/WsLayoutProperties.h>

#include "WsLogo.h"

using namespace Wt;

WsLogo::WsLogo(WContainerWidget* parent)
  : WImage(parent)
{
  wApp->log("notice") << "WsLogo::WsLogo CTOR";
  std::string curImgUrl(wApp->theme()->resourcesUrl() + "wittyshare/Images/logo.png");
  setImageRef(curImgUrl);
  gdImageProperties imgProp = gdImage_size(wApp->docRoot() + curImgUrl);
  setWidth(WLength(imgProp.width, WLength::Pixel));
  setHeight(WLength(imgProp.height, WLength::Pixel));
  wApp->log("notice") << "WsLogo::WsLogo() logo = " << imageRef();
  clicked().connect(SLOT(this, WsLogo::doLogoClicked));
}

WsLogo::~WsLogo()
{ }

void WsLogo::doLogoClicked()
{
  if ( WsLayoutProperties::instance()->get("global", "use_content_directory_view", "false") == "false" ) return wApp->setInternalPath("/", true);
  wApp->setInternalPath("/Logo", true);
}

