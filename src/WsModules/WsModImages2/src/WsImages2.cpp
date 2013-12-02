/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <gdcore/gdCore.h>

#include <Wt/WLogger>
#include <Wt/WImage>
#include <Wt/WBoostAny>
#include <Wt/WText>
#include <Wt/WTheme>

#include <gdcore/gdImage.h>

#include <Main/WsApplication.h>
#include <User/WsUser.h>

#include "WsImages2.h"

using namespace Wt;

wsi2Anchor::wsi2Anchor(const Wt::WLink& link, const Wt::WString& text, Wt::WContainerWidget* parent)
  : WAnchor(link, text, parent), m_nImageIndex(0)
{
  clicked().connect(SLOT(this, wsi2Anchor::doClicked));
}

void wsi2Anchor::setImageIndex(int imageIndex)
{
  m_nImageIndex = imageIndex;
}

void wsi2Anchor::doClicked(Wt::WMouseEvent mEv)
{
  m_sigImageSelected.emit(m_nImageIndex);
}

Wt::Signal<int>& wsi2Anchor::imageSelected()
{
  return m_sigImageSelected;
};







WsImages2::WsImages2(WContainerWidget* parent)
  : WContainerWidget(parent), m_bDebug(false), m_lDelay(5000), m_nCurrentImage(0), m_pTimer(0), m_bLoaded(false), m_bOnLoad(false)
{
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsImages2.css");
  addStyleClass("WsImages2Widget");
}

WsImages2::~WsImages2()
{
  if ( m_pTimer ) {
    m_pTimer->stop();
    delete m_pTimer;
  }
}

void WsImages2::load()
{
  WContainerWidget::load();
  if ( !m_bLoaded )
    build();
  if ( m_bReactWhenPathChange )
    wApp->internalPathChanged().connect(SLOT(this, WsImages2::doPathChanged));
  loadImage();
  if ( m_bReactWhenPathChange ) return;
  if ( !m_lDelay ) return;
  if ( m_sImagesVect.size() < 2 ) return;
  // On ajoute le timer au widget, les objets de ce type ne sont pas détruits par un clear();
  m_pTimer = new WTimer();
  m_pTimer->setInterval(m_lDelay);
  m_pTimer->start();
  m_pTimer->timeout().connect(SLOT(this, WsImages2::doTimeout));
}

void WsImages2::build()
{
  wApp->log("notice") << "WsImages2::load() start ";
  if ( asString(option("debug")) == "true" )
    m_bDebug = true;
  long lDelay = asNumber(option("image_delay"));
  if ( lDelay > 0 )
    m_lDelay = lDelay;
  if ( asString(option("reactWhenPathChange")) == "false" )
    m_bReactWhenPathChange = false;
  std::string sImagesPath = asString(option("imagesPath")).toUTF8();
  wApp->log("notice") << "WsImages2::load imagesPath = " << sImagesPath;
  if ( sImagesPath.size() < 1 )
    sImagesPath = wApp->theme()->resourcesUrl() + "wittyshare/Images/WsImages2";
  if ( m_bDebug )
    wApp->log("notice") << "WsImages2::load imagesPath = " << sImagesPath;
  if (  boost::filesystem::is_directory(WsApp->docRoot() + sImagesPath) ) {
    if ( m_bDebug )
      wApp->log("notice") << "WsImages2::build() start";
    boost::filesystem::directory_iterator end_itr;
    try {
      for (boost::filesystem::directory_iterator itr_dir(WsApp->docRoot() + sImagesPath); itr_dir != end_itr; ++itr_dir) {
        wApp->log("notice") << "WsImages2::In loop " << sImagesPath;
        if ( ! ( itr_dir->path().extension() == ".jpeg"
                 || itr_dir->path().extension() == ".jpg"
                 || itr_dir->path().extension() == ".png"
               ) ) continue;
        if ( gdcore_isPathHidden(itr_dir->path()) )                      continue;
        if ( gdcore_isPathSymLink(itr_dir->path()) )                     continue;
        if ( boost::filesystem::is_directory(itr_dir->status()) )        continue;
        boost::filesystem::path newPath = itr_dir->path();
        m_sImagesVect.push_back(newPath.string());
        if ( m_bDebug )
          wApp->log("notice") << "WsImages2::build() new image : path =" << newPath.string();
      }
    } catch (boost::filesystem::filesystem_error& e) {
      wApp->log("ERROR") << "WsImages2::build() bad image path =" << sImagesPath << " error = " << e.what();
      return;
    }
  } else
    m_sImagesVect.push_back(WsApp->docRoot() + sImagesPath);
  // TODO : the images in vect is img3 img2 img1, not sure that the sort apply on all cases
  std::sort(m_sImagesVect.begin(), m_sImagesVect.end());
  for (int iP = 0; iP < m_sImagesVect.size(); ++iP) {
    // Load the internalPath from a file with an extension url, the name of the file does be the same that the image.
    std::string sLink;
    std::string sLinkType;
    std::string sTitle;
    std::string sShortDescription;
    boost::filesystem::path urlFile(m_sImagesVect[iP]);
    urlFile.replace_extension(".url");
    if ( boost::filesystem::exists(urlFile) ) {
      std::ifstream f(urlFile.string().c_str());
      std::string sLine;
      // std::getline(f, sLine) Line without cr
      while ( std::getline(f, sLine) ) {
        if ( sLine.compare(0,  5, "link=")             == 0 )   sLink             = sLine;
        if ( sLine.compare(0,  9, "linkType=")         == 0 )   sLinkType         = sLine;
        if ( sLine.compare(0,  6, "title=")            == 0 )   sTitle            = sLine;
        if ( sLine.compare(0, 17, "shortDescription=") == 0 )   sShortDescription = sLine;
      }
      boost::algorithm::replace_all(sLink,             "link=",             "");
      boost::algorithm::replace_all(sLinkType,         "linkType=",         "");
      boost::algorithm::replace_all(sTitle,            "title=",            "");
      boost::algorithm::replace_all(sShortDescription, "shortDescription=", "");
    }
    m_sImagesLink.push_back(sLink);
    m_sImagesLinkType.push_back(sLinkType);
    if ( m_bDebug )
      wApp->log("notice") << "WsImages2::build() url image = " << sLink;
    // Load the short description of the internalPath loaded from the url file.
    if ( sShortDescription.size() < 1 )
      if ( sLink.size() > 0 ) {
        WsUser* pUser       = WsApp->wsUser();
        NodePtr pNode       = pUser->getAccessRoot().get()->eatPath(sLink);
        if ( pNode ) {
          sShortDescription = pNode.get()->getProperties().get()->get("global", "short_description", "");
          if ( sTitle.size() < 1 )
            sTitle = pNode.get()->getDisplayName(true);
        }
      }
    m_sImagesTitle.push_back(sTitle);
    m_sImagesText.push_back(sShortDescription);
  }
  m_bLoaded = true;
  if ( m_bDebug )
    wApp->log("notice") << "WsImages2::build() end";
}

int WsImages2::count()
{
  return m_sImagesVect.size();
}

void WsImages2::doTimeout()
{
  if ( m_bOnLoad ) return;
  //  if ( m_bDebug )
  //     wApp->log("notice") << "WsImages2::doTimeout() m_nCurrentImage = " << m_nCurrentImage;
  if ( (m_nCurrentImage + 1) >= m_sImagesVect.size() )   m_nCurrentImage = 0;
  else                                                ++m_nCurrentImage;
  loadImage();
}

void WsImages2::doPathChanged()
{
  doTimeout();
}

void WsImages2::loadImage()
{
  if ( m_bOnLoad ) return;
  m_bOnLoad = true;
  if ( m_bDebug )
    WApplication::instance()->log("notice") << "WsImages2::loadImage()";
  if ( !m_sImagesVect.size() ) {
    WApplication::instance()->log("notice") << "WsImages2::loadImage() : no images !";
    m_bOnLoad = false;
    return;
  }
  clear();
  if ( asString(option("useImageTitle")) == "true" ) {
    std::string curImgTitle(m_sImagesTitle[m_nCurrentImage]);
    if ( curImgTitle.size() > 0 ) {
      boost::algorithm::replace_all(curImgTitle, "&", "&amp;");
      WText* pTextTitle = new WText(curImgTitle);
      pTextTitle->addStyleClass("WsImage2Title");
      addWidget(pTextTitle);
    }
  }
  WContainerWidget* cwImg = new WContainerWidget();
  cwImg->addStyleClass("WsImages2");
  if (  m_sImagesVect.size() > 1 && asString(option("useCountImages")) != "false") {
    WContainerWidget* cw = new WContainerWidget();
    cw->addStyleClass("countImages");
    for (int countImage = 0; countImage < m_sImagesVect.size(); ++countImage) {
      wsi2Anchor* pAnchor = 0;
      if ( asString(option("setImagesCount")) != "true" )
        pAnchor = new wsi2Anchor(WLink(), "&nbsp;");
      else
        pAnchor = new wsi2Anchor(WLink(), asString(countImage + 1));
      pAnchor->imageSelected().connect(SLOT(this, WsImages2::doCounterClicked));
      pAnchor->setImageIndex(countImage);
      if ( countImage == m_nCurrentImage )
        pAnchor->addStyleClass("selected");
      cw->addWidget(pAnchor);
    }
    cwImg->addWidget(cw);
  }
  std::string curImgUrl(m_sImagesVect[m_nCurrentImage]);
  gdImageProperties imgProp = gdImage_size(curImgUrl);
  if ( m_bDebug )
    wApp->log("notice") << "WsImages2::loadImage set image width to " << imgProp.width << " and height = " << imgProp.height;
  boost::algorithm::replace_first(curImgUrl, wApp->docRoot(), "");
  WLink myLink(WLink::Url, curImgUrl);
  if ( m_bDebug )
    wApp->log("notice") << "WsImages2::loadImage WLink = " << curImgUrl;
  if ( asString(option("imageInBackground")) == "true" ) {
    cwImg->decorationStyle().setBackgroundImage(myLink, Wt::WCssDecorationStyle::NoRepeat);
    if ( asString(option("textWidget")) == "true" ) {
      if ( m_sImagesText[m_nCurrentImage].size() > 0 ) {
        WText* textImage = new WText(m_sImagesText[m_nCurrentImage]);
        textImage->addStyleClass("WsImages2Text");
        if ( m_sImagesLink[m_nCurrentImage].size() > 1 ) {
          boost::filesystem::path pUrl(m_sImagesLink[m_nCurrentImage]);
          if ( pUrl.extension() != ".nolink" ) {
            textImage->addStyleClass("WsClickable");
            textImage->clicked().connect(SLOT(this, WsImages2::doImageClicked));
          }
        }
        cwImg->addWidget(textImage);
      }
    }
  } else {
    WImage* pImage = new WImage();
    pImage->setImageLink(myLink);
    cwImg->addWidget(pImage);
  }
  if ( asString(option("plusWidget")) == "true" ) {
    std::string curImgUrl2(m_sImagesLink[m_nCurrentImage]);
    if ( m_bDebug )
      wApp->log("notice") << "WsImages2::loadImage curImgUrl =  " << curImgUrl2;
    WAnchor* pA = new WAnchor(WLink(WLink::InternalPath, curImgUrl2), "+");
    pA->addStyleClass("WsPlusWidget");
    cwImg->addWidget(pA);
  }
  cwImg->resize(WLength(imgProp.width, WLength::Pixel), WLength(imgProp.height, WLength::Pixel));
  addWidget(cwImg);
  // TODO : test if needed
  refresh();
  if ( m_bDebug )
    wApp->log("notice") << "WsImages2::loadImage " << curImgUrl;
  m_bOnLoad = false;
}

const std::string& WsImages2::text()
{
  return m_sImagesText[m_nCurrentImage];
}

void WsImages2::doImageClicked()
{
  wApp->log("notice") << "WsImages2::doImageClicked path = " << m_sImagesLink[m_nCurrentImage];
  wApp->setInternalPath(m_sImagesLink[m_nCurrentImage], true);
}

void WsImages2::doCounterClicked(int index)
{
  if ( m_bOnLoad ) return;
  wApp->log("notice") << "WsImages2::doCounterClicked index = " << index;
  m_nCurrentImage = index;
  loadImage();
}

