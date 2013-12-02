/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

#include <Wt/WLogger>
#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

#include "WsModImagesGallery.h"

extern "C" {
  void WsModImagesGalleryInit(void)
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

// Class WsImagesGallery
// ==================
WsImagesGallery::WsImagesGallery(WContainerWidget* parent)
  : WContainerWidget(parent), m_nStartImage(0), m_nMaxImages(3), m_pTimerThumb(0), m_pTimerImage(0), m_nCurrentImage(0), m_pImages2(0), m_pImages2Text(0), m_cwThumb(0)
{
  addStyleClass("WsImagesGallery");
  LOG(DEBUG) << "WsImagesGallery::WsImagesGallery() !";
}

WsImagesGallery::~WsImagesGallery()
{
  LOG(DEBUG) << "WsImagesGallery::~WsImagesGallery() !";
}

void WsImagesGallery::load()
{
  WContainerWidget::load();
  WsUser*                       pUser            = WsApp->wsUser();
  std::string                   sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(WsApp->internalPath());
  wApp->log("notice") << "WsImagesGallery::load() sWithoutPrefix = " << sWithoutPrefix;
  NodePtr   startNode = pUser->getAccessRoot();
  if ( !startNode ) return;
  NodePtr curNode    = startNode.get()->eatPath(sWithoutPrefix);
  std::string title = curNode.get()->getDisplayName(true);
  WText* pTitle = new WText(title);
  pTitle->addStyleClass("WsTitle");
  addWidget(pTitle);
  int nMaxImages = asNumber(option("maxImages"));
  if ( nMaxImages > 0 )
    setMaxImages(nMaxImages);
  buildVector();
  buildThumbnails();
  renderButtons();
  renderThumb();
  renderImage();
  m_pTimerImage = new WTimer(this);
  m_pTimerImage->setInterval(asNumber(option("image_delay")));
  m_pTimerImage->timeout().connect(SLOT(this, WsImagesGallery::doTimeoutImage));
  if ( m_vImages.size() > m_nMaxImages && asString(option("useThumbTimer")).toUTF8() != "false" ) {
    m_pTimerThumb = new WTimer(this);
    m_pTimerThumb->setInterval(asNumber(option("image_delay")));
    m_pTimerThumb->timeout().connect(SLOT(this, WsImagesGallery::doTimeoutThumb));
    m_pTimerThumb->start();
  }
  WApplication::instance()->log("notice") << "WsImagesGallery::load : end !";
}

void WsImagesGallery::buildVector()
{
  std::string                   imgPath          = asString(option("imagesPath")).toUTF8();
  boost::algorithm::replace_all(imgPath, "&amp;", "&");
  if ( imgPath.size() < 1 ) return;
  WsUser*                       pUser            = WsApp->wsUser();
  std::string                   sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(WsApp->internalPath());
  wApp->log("notice") << "WsImagesGallery::buildVector() imgPath = " << imgPath << "sWithoutPrefix = " << sWithoutPrefix;
  boost::filesystem::directory_iterator end_itr;
  try {
    for (boost::filesystem::directory_iterator itr_dir(pUser->getRootPath() + imgPath); itr_dir != end_itr; ++itr_dir) {
      if ( ! ( itr_dir->path().extension() == ".jpeg"
               || itr_dir->path().extension() == ".jpg"
               || itr_dir->path().extension() == ".png"
             ) ) continue;
      if ( gdcore_isPathHidden(itr_dir->path()) )                      continue;
      if ( gdcore_isPathSymLink(itr_dir->path()) )                     continue;
      if ( boost::filesystem::is_directory(itr_dir->status()) )        continue;
      m_vImages.push_back(itr_dir->path().string());
    }
  } catch (boost::filesystem::filesystem_error& e) {
    wApp->log("ERROR") << "WsImagesGallery::buildVector() bad image path =" << imgPath << " error = " << e.what();
    return;
  }
}

void WsImagesGallery::buildThumbnails()
{
  for (int curImg = 0; curImg < m_vImages.size(); ++curImg) {
    std::string urlSized     = m_vImages[curImg];
    std::string sResize      = asString(option("resize")).toUTF8();
    if ( sResize.size() < 1 ) sResize = "100*75";
    //    boost::algorithm::replace_last(urlSized, "/images/",  "/thumbnails/"); // TODO : Improve by path.parent_path
    Magick::Blob rBlob;
    int status = gdImage_resizeImage(m_vImages[curImg], rBlob, sResize);
    //    int status = gdImage_resizeImage(m_vImages[curImg], urlSized, sResize); // TODO : Use Blob
    if ( status ) {
      wApp->log("notice") << "WsImagesGallery::buildThumbnails() : cannot resize to Blob = " << urlSized << " status = " << status << " url source " << m_vImages[curImg];
      //wApp->log("notice") << "WsImagesGallery::buildThumbnails() : cannot resize to urlSized = " << urlSized << " status = " << status << " url source " << m_vImages[curImg];
      continue;
    }
    m_vThumbnails.push_back(rBlob);
    // m_vThumbnails.push_back(urlSized);
  }
}

void WsImagesGallery::doTimeoutThumb()
{
  if ( (m_nStartImage + 1) >= (m_vImages.size() - m_nMaxImages) )   m_nStartImage = 0;
  else                                                           ++m_nStartImage;
  renderThumb();
}

void WsImagesGallery::doTimeoutImage()
{
  if ( (m_nCurrentImage + 1) >= m_vImages.size() )   m_nCurrentImage = 0;
  else                                            ++m_nCurrentImage;
  renderImage();
}

void WsImagesGallery::renderButtons()
{
  WContainerWidget* cw = new WContainerWidget();
  cw->addStyleClass("WsPlayer");
  WPushButton* button = new WPushButton("", cw);
  button->setIcon(WLink(WLink::Url, wApp->theme()->resourcesUrl() + "wittyshare/Icons/previous.png"));
  button->clicked().connect(boost::bind(&WsImagesGallery::doPrevious, this, button));
  button = new WPushButton("", cw);
  button->setIcon(WLink(WLink::Url, wApp->theme()->resourcesUrl() + "wittyshare/Icons/play.png"));
  button->clicked().connect(boost::bind(&WsImagesGallery::doPlay, this, button));
  button = new WPushButton("", cw);
  button->setIcon(WLink(WLink::Url, wApp->theme()->resourcesUrl() + "wittyshare/Icons/next.png"));
  button->clicked().connect(boost::bind(&WsImagesGallery::doNext, this, button));
  addWidget(cw);
}

void WsImagesGallery::renderThumb()
{
  if ( m_pTimerThumb )
    m_pTimerThumb->stop();
  bool bNewThumb = false;
  if ( !m_cwThumb ) {
    bNewThumb = true;
    m_cwThumb = new WContainerWidget();
    m_cwThumb->addStyleClass("WsThumbnails");
  } else
    m_cwThumb->clear();
  int maxImg = m_nMaxImages;
  if ( (m_nStartImage + maxImg) > m_vImages.size() )
    maxImg = m_vImages.size() - m_nStartImage;
  for (int countDir = m_nStartImage; countDir < (m_nStartImage + maxImg); ++countDir) {
    if ( countDir > m_vImages.size() ) {
      WApplication::instance()->log("notice") << "WsImagesGallery::renderThumb : OH OH anormal break !!!!";
      break;
    }
    WImage*      pImage      = new WImage();
    pImage->addStyleClass("thumbnails");
    if ( countDir == m_nCurrentImage ) {
      pImage->addStyleClass("WsSelected");
    }
    pImage->addStyleClass("WsClickable");
    std::string       imglnk = "data:image/jpeg;base64," + m_vThumbnails[countDir].base64();
    pImage->setImageLink(imglnk);
    pImage->clicked().connect(boost::bind(&WsImagesGallery::doThumbClicked, this, countDir));
    m_cwThumb->addWidget(pImage);
  }
  if ( bNewThumb )
    addWidget(m_cwThumb);
  if ( m_pTimerThumb )
    m_pTimerThumb->start();
}

void WsImagesGallery::renderImage()
{
  std::string imgPath = m_vImages[m_nCurrentImage];
  boost::algorithm::replace_first(imgPath, wApp->docRoot(), "");
  int idxImg2 = -1;
  if ( m_pImages2 ) {
    idxImg2 = indexOf(m_pImages2);
    removeWidget(m_pImages2);
    removeWidget(m_pImages2Text);
    delete m_pImages2;
    delete m_pImages2Text;
    m_pImages2     = 0;
    m_pImages2Text = 0;
  }
  m_pImages2 = new WsImages2();
  m_pImages2->setOptions(options());
  m_pImages2->setOption("useImageTitle", std::string("false"));
  m_pImages2->setOption("imagesPath", imgPath);
  m_pImages2->setOption("textWidget", std::string("false"));
  if ( asString(option("debug")) == "true" )
    m_pImages2->outOptions("WsImagesGallery::renderImage())");
  m_pImages2->build();
  m_pImages2Text = new WText(m_pImages2->text());
  m_pImages2Text->addStyleClass("WsImagesGalleryText");
  //  if ( idxImg2 != -1 ) { // si thumbnail en dessous de l'image et du texte
  if ( false ) {
    insertBefore(m_pImages2    , m_cwThumb);
    insertBefore(m_pImages2Text, m_cwThumb);
  } else {
    addWidget(m_pImages2);
    addWidget(m_pImages2Text);
  }
  // TODO : WImage vector + removeStyleClass WsSelected + addStyleClass WsSelected and remove next line
  renderThumb();
}

void WsImagesGallery::setMaxImages(int nMaxImages)
{
  m_nMaxImages = nMaxImages;
}

void WsImagesGallery::doThumbClicked(int nImageId)
{
  m_pTimerImage->stop();
  m_nCurrentImage = nImageId;
  renderImage();
}

void WsImagesGallery::doPrevious(Wt::WPushButton* pButton)
{
  m_pTimerImage->stop();
  if ( (m_nCurrentImage - 1) < 0 )    m_nCurrentImage = m_vImages.size() - 1;
  else                             --m_nCurrentImage;
  renderImage();
}

void WsImagesGallery::doPlay(Wt::WPushButton* pButton)
{
  std::string state =  pButton->icon().url();
  if ( state ==  wApp->theme()->resourcesUrl() + "wittyshare/Icons/play.png") {
    pButton->setIcon(WLink(WLink::Url, wApp->theme()->resourcesUrl() + "wittyshare/Icons/pause.png"));
    m_pTimerImage->start();
  } else {
    pButton->setIcon(WLink(WLink::Url, wApp->theme()->resourcesUrl() + "wittyshare/Icons/play.png"));
    m_pTimerImage->stop();
  }
}

void WsImagesGallery::doNext(Wt::WPushButton* pButton)
{
  m_pTimerImage->stop();
  if ( (m_nCurrentImage + 1) >= m_vImages.size() )   m_nCurrentImage = 0;
  else                                            ++m_nCurrentImage;
  renderImage();
}


// Class WsModImagesGallery
// =====================
WsModImagesGallery::WsModImagesGallery()
  : WsModule()
{
  LOG(DEBUG) << "WsModImagesGallery::WsModImagesGallery() !";
}

WsModImagesGallery::~WsModImagesGallery()
{
  LOG(DEBUG) << "WsModImagesGallery::~WsModImagesGallery() !";
}

WWidget* WsModImagesGallery::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModImagesGallery::createContents(WContainerWidget* parent) const
{
  WsImagesGallery* pImgGal = new WsImagesGallery(parent);
  pImgGal->setOptions(options());
  return pImgGal;
}

WsEditorWidget* WsModImagesGallery::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModImagesGallery::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

bool WsModImagesGallery::saveEditor() const
{
  return true;
}

std::string WsModImagesGallery::description() const
{
  return "WsModImagesGallery wittyShare module";
}

