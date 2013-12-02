/*
 *
 *       Filename:  WsModTwitter.cpp
 *
 *    Description:  Twitter Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include <curl/curl.h>

#include <Wt/WTheme>
#include <Wt/WLogger>
#include <Wt/Utils>

#include <gdwtcore/gdToolbar.h>

#include <Logger/WsLogger.h>

#include "WsModTwitter.h"

extern "C" {
  void WsModTwitterInit(void)
  {
    fprintf(stderr, "preparing required libraries : libarchive.so!\n");
    void* hndl = dlopen("libarchive.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libarchive.so shared library! %s\n", dlerror());
      return;
    }
  }
}


using namespace Wt;

WsTwitterEditorWidget::WsTwitterEditorWidget(WContainerWidget* parent)
  : WsEditorWidget(parent), m_cb(0), m_cbState(true)
{ }

WsTwitterEditorWidget::~WsTwitterEditorWidget()
{ }

void WsTwitterEditorWidget::load()
{
  WsEditorWidget::load();
  WsUser* user = WsApplication::wsInstance()->wsUser();
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsTwitterEditorWidget::load() " << path;
  std::string prop(user->getProperty(path, "WsModTwitter", "twitter_enable", "true"));
  m_cb = new WCheckBox("Enable twitter ?");
  if ( prop == "true" ) {
    m_cbState = true;
    m_cb->setCheckState(Wt::Checked);
  } else {
    m_cbState = false;
    m_cb->setCheckState(Wt::Unchecked);
  }
  addWidget(m_cb);
}

bool WsTwitterEditorWidget::saveEditor()
{
  WsUser* user = WsApplication::wsInstance()->wsUser();
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsTwitterEditorWidget::saveEditor " << path;
  if ( m_cb->checkState() != m_cbState )
    if ( user->saveProperty(path, "WsModTwitter", "twitter_enable", m_cb->checkState() ? "true" : "false") == FAILURE ) return false;
  return true;
}




WsModTwitter::WsModTwitter() : WsModule(), m_buttonReTweet(0)
{
  LOG(DEBUG) << "WsModTwitter::WsModTwitter() end ctor of WsModTwitter !";
}

WsModTwitter::~WsModTwitter()
{
  LOG(DEBUG) << "WsModTwitter::WsModTwitter() end dtor of WsModTwitter !";
}

WWidget* WsModTwitter::createContentsMenuBar(WContainerWidget* parent) const
{
  bool b = true;
  if ( asString(option("inContentMenuBar")) == "false" )
    b = false;
  LOG(DEBUG) << "WsModTwitter::createContentsMenuBar inContentMenuBar = " << b;
  if ( !b ) return 0;
  WsModTwitter* self =  const_cast<WsModTwitter*>(this);
  self->buildMenuBar();
  return 0;
}

void WsModTwitter::destroyContentsMenuBar()
{
  m_buttonReTweet = 0;
  WsModule::destroyContentsMenuBar();
}

WWidget* WsModTwitter::createContents(WContainerWidget* parent) const
{
  return 0;
}

WsEditorWidget* WsModTwitter::createEditor(WContainerWidget* parent) const
{
  WsTwitterEditorWidget* editor = new WsTwitterEditorWidget(parent);
  return editor;
}

WWidget* WsModTwitter::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModTwitter::buildMenuBar()
{
  if ( !contentButtonsBar() ) return 0;
  gdToolbarItem*    pTbItem = contentButtonsBar()->addToolbarItem("", " ", "");
  //  pTbItem->clicked().connect(SLOT(t 0his, WsModTwitter::handleTwitterClick));
  m_buttonReTweet = pTbItem->button();
  m_buttonReTweet->setIcon(wApp->theme()->resourcesUrl() + "wittyshare/Icons/Twitter_Retweet-16x16.png");
  m_buttonReTweet->setLinkTarget(Wt::TargetNewWindow);
  wApp->internalPathChanged().connect(SLOT(this, WsModTwitter::handleTwitterClick));
  return 0;
}

void WsModTwitter::handleTwitterClick()
{
  wApp->log("notice") <<  "WsModTwitter::handleTwitterClick()";
  if ( !m_buttonReTweet ) return;
  wApp->log("notice") <<  "WsModTwitter::handleTwitterClick() button ok";
  WsUser*     pUser   = WsApp->wsUser();
  std::string curPath = wApp->internalPath();
  boost::algorithm::replace_all(curPath, "&amp;",  "&");
  std::string tFile   = pUser->getRootPath() + curPath;
  if ( boost::filesystem::is_directory(tFile) ) return;
  NodePtr pRootNode    = pUser->getAccessRoot();
  if ( !pRootNode ) {
    wApp->log("notice") <<  "WsModTwitter::handleTwitterClick() : Path = " << curPath << ", getAccessRoot() is null";
    return;
  }
  NodePtr pNode        = pRootNode.get()->eatPath(curPath);
  if ( !pNode ) {
    wApp->log("notice") <<  "WsModTwitter::handleTwitterClick() : Path = " << curPath << ", getAccessRoot() eatPath is null";
    return;
  }
  std::string sText       = pNode.get()->getDisplayName(true);
  std::string sServerUrl  = WsGlobalProperties::instance()->get("global", "serverUrl", "http://localhost");
  std::string sFullUrl    = sServerUrl + curPath;
  char* pCurl = curl_escape(sFullUrl.c_str(), sFullUrl.size());
  std::string sCurl(pCurl);
  curl_free(pCurl);
  boost::algorithm::replace_all(sCurl, "/",  "%2F");
  boost::algorithm::replace_all(sCurl, " ",  "%20");
  pCurl = curl_escape(sText.c_str(), sText.size());
  std::string sTextEnc(pCurl);
  curl_free(pCurl);
  boost::algorithm::replace_all(sTextEnc, "/",  "%2F");
  boost::algorithm::replace_all(sTextEnc, " ",  "%20");
  std::string twitterUrl;
  twitterUrl += "https://twitter.com/intent/tweet?original_referer=";
  //  twitterUrl += "http%3A%2F%2Fwww.eurofer.eu%2Findex.php%2Feng%2FNews-Media%2FPress-Releases%2FENVI-Committee-backs-life-cycle-approach-to-vehicle-emissions";
  //  twitterUrl += Wt::Utils::urlEncode(sFullUrl);
  twitterUrl += sCurl;
  twitterUrl += "&text=";
  //  twitterUrl += "ENVI%20Committee%20backs%20life%20cycle%20approach%20to%20vehicle%20emissions";
  //  twitterUrl += Wt::Utils::urlEncode(sText);
  twitterUrl += sTextEnc;
  twitterUrl += "&tw_p=tweetbutton&url=";
  //  twitterUrl += "http%3A%2F%2Fwww.eurofer.eu%2Findex.php%2Feng%2FNews-Media%2FPress-Releases%2FENVI-Committee-backs-life-cycle-approach-to-vehicle-emissions";
  //  twitterUrl += Wt::Utils::urlEncode(sFullUrl);
  twitterUrl += sCurl;
  twitterUrl += "&via=";
  twitterUrl += "EUROFER_eu";
  LOG(DEBUG) << "WsModTwitter::handleTwitterClick " << twitterUrl;
  m_buttonReTweet->setLink(twitterUrl);
}

std::string WsModTwitter::description() const
{
  return "WsModTwitter wittyShare module";
}

