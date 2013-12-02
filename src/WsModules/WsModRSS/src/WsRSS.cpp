/*
 *
 *       Filename:  WsRSS.cpp
 *
 *    Description:  latest news Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Guy Deleeuw, Guy.Deleeuw@gmail.com
 *
 */

#include <boost/filesystem.hpp>

#include <stdio.h>

#include <uuid/uuid.h>

#include <Wt/WDateTime>
#include <Wt/WEnvironment>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <ConfigManager/WsGlobalProperties.h>

#include <WsModule/WsModule.h>

#include "WsSearchRSS.h"
#include "WsRSS.h"

#define WSMODLATESTNEWS   "wsmodlatestnews"
#define NB_NEWS            "nb_news"
#define ROOT              "root"

using namespace Wt;

WsRSS::WsRSS(WContainerWidget* parent)
  : WContainerWidget(parent)
{
  addStyleClass("WsRSS");
}

WsRSS::~WsRSS()
{ }

void WsRSS::load()
{
  WContainerWidget::load();
}

void WsRSS::buildRSS()
{
  WDateTime   dt(WDateTime::currentDateTime());
  std::string sDateTime    = dt.toString("yyyy-MM-dd hh:mm:ss").narrow();
  WsUser*     user         = WsApplication::wsInstance()->wsUser();
  std::string sServerUrl  = WsGlobalProperties::instance()->get("global", "serverUrl", "http://localhost");
  //  std::string serverUrl   = asString(option("serverUrl")).toUTF8();
  std::string serverDesc  = asString(option("serverDesc")).toUTF8();
  std::string serverTitle = asString(option("serverTitle")).toUTF8();
  std::string rssFile     = asString(option("rssFile")).toUTF8();
  std::string path2RSS = user->getRootPath() + rssFile;
  FILE* xmlFile = fopen(path2RSS.c_str(), "w+");
  if ( !xmlFile ) return;
  fprintf(xmlFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  fprintf(xmlFile, "<rss version=\"2.0\">\n");
  fprintf(xmlFile, "  <channel>\n");
  fprintf(xmlFile, "   <title>%s</title>\n", serverTitle.c_str());
  fprintf(xmlFile, "   <description>%s</description>\n", serverDesc.c_str());
  fprintf(xmlFile, "   <lastBuildDate>%s</lastBuildDate>\n", sDateTime.c_str());
  fprintf(xmlFile, "   <link>%s</link>\n", sServerUrl.c_str());
  std::string rootPath = asString(option("rootPath")).narrow();
  boost::algorithm::replace_all(rootPath, "&amp;",  "&");
  NodePtr root;
  if ( rootPath.size() < 1 )
    root = user->getAccessRoot();
  else
    root = user->getAccessRoot();
  root = root.get()->eatPath(rootPath);
  if (root == 0) {
    LOG(DEBUG) << "WsRSS::setTable cannot get root !";
    return;
  }
  WsSearchRSS searchRSS(root);
  int rowsCount = asNumber(option("rowsCount"));
  std::vector<NodePtr> news =  searchRSS.getRSS(rowsCount);
  /* Exemple le soir.be
   <item>
    <category><![CDATA[Monde]]></category>
    <title><![CDATA[Jean-Luc Melenchon aux Chypriotes: «Bienvenue en enfer»]]></title>
    <link>http://www.lesoir.be/209787/article/actualite/monde/2013-03-18/jean-luc-melenchon-aux-chypriotes-«bienvenue-en-enfer»</link>
    <description><![CDATA[<img src="http://www.lesoir.be/sites/default/files/imagecache/flowpublish_preset/2013/03/18/1745967359_B97244034Z.1_20130318105823_000_GT9K6204.2-0.jpg" width="120" align="left" hspace="4" vspace="4" />
                  Chypre a dû accepter d’instaurer une taxe exceptionnelle sur les dépôts bancaires en contrepartie d’un prêt européen de 10 milliards d’euros. Le début de l’enfer, selon Jean-Luc Mélenchon. Qui est touché, et comment? Quelques repères sur le plan de sauvetage qui doit être soumis au Parlement.
              ]]></description>
    <comments>http://www.lesoir.be/209787/article/actualite/monde/2013-03-18/jean-luc-melenchon-aux-chypriotes-«bienvenue-en-enfer»#ancre_commentaires</comments>
    <pubDate>Mon, 18 Mar 2013 09:49:47 +0100</pubDate>
    <author><![CDATA[ Afp]]></author>
    <guid isPermaLink="false">209787 at http://www.lesoir.be</guid>
      <media:content url="http://www.lesoir.be/sites/default/files/imagecache/flowpublish_preset/2013/03/18/1745967359_B97244034Z.1_20130318105823_000_GT9K6204.2-0.jpg" type="image/jpeg" fileSize="1503654">
    <media:title><![CDATA[]]></media:title>
    <media:thumbnail url="http://www.lesoir.be/sites/default/files/imagecache/flowpublish_thumb_preset/2013/03/18/1745967359_B97244034Z.1_20130318105823_000_GT9K6204.2-0.jpg"/>
    </media:content>
    </item>
  */
  for (int i = 0; i < news.size(); ++i) {
    if ( !(
           news[i].get()->getPath().extension().string() == ".fhtml" ||
           news[i].get()->getPath().extension().string() == ".pdf"   ||
           news[i].get()->getPath().extension().string() == ".rpg"   ||
           news[i].get()->getPath().extension().string() == ".zip"
         ) )
      continue;
    if ( news[i].get()->getProperties().get()->get("global", "in_view", "true") == "false"   ) continue;
    boost::filesystem::path wsRes = news[i]->getPath();
    bool bRes = false;
    while (wsRes.string() != "/") {
      wsRes = wsRes.parent_path();
      if ( wsRes.filename().string() == "ws.res" ) {
        bRes = true;
        break;
      }
      if ( wsRes.filename().string() == "ws" )     {
        bRes = true;
        break;
      }
      continue;
    }
    if ( bRes ) continue;
    fprintf(xmlFile, "     <item>\n");
    WDateTime   dt(WDateTime::currentDateTime());
    std::string sTitle    = news[i].get()->getDisplayName(true);
    std::string sDesc     = news[i].get()->getProperties().get()->get("global", "short_description", "");
    std::string sPubDate  = news[i].get()->getProperties().get()->get("global", "publish_date", boost::lexical_cast<std::string>(dt.toTime_t()));
    dt.setTime_t(boost::lexical_cast<time_t>(sPubDate));
    sPubDate = dt.toString("yyyy-MM-dd hh:mm:ss").narrow();
    //    dateTime.setTime_t(news[i]->getModifyDate());
    std::string sLink     = sServerUrl + news[i]->getPath().string();
    //    boost::algorithm::replace_all(sTitle, "&",  "&amp;");
    //    boost::algorithm::replace_all(sDesc,  "&",  "&amp;");
    boost::algorithm::replace_all(sLink,  "&",  "&amp;");
    char uuidBuff[36];
    uuid_t uuidGenerated;
    uuid_generate_random(uuidGenerated);
    uuid_unparse(uuidGenerated, uuidBuff);
    std::string sGuid     = news[i]->getProperties().get()->get("global", "guid", uuidBuff);
    fprintf(xmlFile, "       <title><![CDATA[%s]]></title>\n", sTitle.c_str());
    fprintf(xmlFile, "       <description><![CDATA[%s]]></description>\n", sDesc.c_str());
    fprintf(xmlFile, "       <pubDate>%s</pubDate>\n", sPubDate.c_str());
    fprintf(xmlFile, "       <link>%s</link>\n", sLink.c_str());
    fprintf(xmlFile, "       <guid>%s</guid>\n", sGuid.c_str());
    fprintf(xmlFile, "     </item>\n");
  }
  fprintf(xmlFile, "  </channel>\n");
  fprintf(xmlFile, "</rss>\n");
  fclose(xmlFile);
}

