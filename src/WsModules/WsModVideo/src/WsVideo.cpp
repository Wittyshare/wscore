
#include <boost/filesystem/operations.hpp>

#include <Wt/WVideo>
#include <Wt/WFlashObject>
#include <Wt/WMediaPlayer>

#include <Logger/WsLogger.h>
#include <User/WsUser.h>

#include <Main/WsApplication.h>

#include "WsVideo.h"

using namespace Wt;

WsVideo::WsVideo(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsVideo");
  LOG(DEBUG) << "end ctor of WsVideo !";
}

WsVideo::~WsVideo()
{
  LOG(DEBUG) << "end dtor of WsVideo !";
}

void WsVideo::load()
{
  WContainerWidget::load();
  WsUser*     pUser            = WsApp->wsUser();
  if ( !pUser ) return;
  NodePtr         rootNode = pUser->getAccessRoot();
  if ( !rootNode ) return;
  NodePtr         curNode  = rootNode.get()->eatPath(wApp->internalPath());
  if ( !curNode ) return;
  std::string m_sDocumentRoot  = pUser->getRootPath(); // /var/www/demo_site
  std::string videoFile          = m_sDocumentRoot + wApp->internalPath();
  LOG(DEBUG) << "WsVideo::load file = " << videoFile << " iPath = " << wApp->internalPath();
  WText* pText = new WText(curNode.get()->getDisplayName(true));
  pText->addStyleClass("WsTitle");
  addWidget(pText);
  std::string extension = boost::filesystem::path(videoFile).extension().string();
  if ( extension == ".ogv" || extension == "mp4" ) {
    boost::algorithm::replace_first(videoFile, WsApp->docRoot(), "");
    WVideo* video = new Wt::WVideo(this);
    video->addSource(Wt::WLink(videoFile));
    //video->addSource(Wt::WLink(wApp->internalPath());
    video->resize(640, 360);
    LOG(DEBUG) << "end WsVideo load .ogv or .mp4 ! videoFile = " << videoFile;
    return;
  }
  if ( extension == ".m4v" ) {
    boost::algorithm::replace_first(videoFile, WsApp->docRoot(), "");
    WMediaPlayer* player = new WMediaPlayer(Wt::WMediaPlayer::Video, this);
    player->addSource(Wt::WMediaPlayer::M4V, Wt::WLink(videoFile));
    //player->addSource(Wt::WMediaPlayer::M4V, Wt::WLink(wApp->internalPath()));
    player->setVideoSize(640, 360);
    LOG(DEBUG) << "end WsVideo load .m4v ! videoFile = " << videoFile;;
    return;
  }
  if ( extension == ".youtube" ) {
    std::string sUrl;
    if ( boost::filesystem::exists(videoFile) ) {
      std::ifstream f(videoFile.c_str());
      std::getline(f, sUrl); // url without CR
    }
    if ( sUrl.size() < 1 ) return;
    //     WFlashObject *flash = new WFlashObject("http://www.youtube.com/embed/k7ZfwYhnSE4", this);
    WFlashObject* flash = new WFlashObject(sUrl, this);
    flash->setFlashParameter("allowFullScreen", "true");
    flash->resize(640, 360);
    LOG(DEBUG) << "end WsVideo load .youtube ! url = [" << sUrl << "]";
  }
  LOG(DEBUG) << "end WsVideo load!";
}

