/*
 *
 *       Filename:  WsLatestNews2.cpp
 *
 *    Description:  latest news Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Guy Deleeuw, Guy.Deleeuw@gmail.com
 *
 */

#include <boost/filesystem.hpp>

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <Wt/WDateTime>
#include <Wt/WStandardItemModel>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <ConfigManager/WsGlobalProperties.h>

#include <WsModule/WsModule.h>

#include "WsLatestNews.h"
#include "WsLatestNews2.h"

#define WSMODLATESTNEWS   "wsmodlatestnews"
#define NB_NEWS            "nb_news"
#define ROOT              "root"


using namespace Wt;

WsLatestNews2::WsLatestNews2(WContainerWidget* parent)
  : WContainerWidget(parent), m_pTableView(0)
{
  addStyleClass("WsLatestNews2");
}

WsLatestNews2::~WsLatestNews2()
{ }

void WsLatestNews2::load()
{
  WContainerWidget::load();
  setTable();
  WText* pTitle = new WText(asString(option("title")).toUTF8());
  pTitle->addStyleClass("WsTitle");
  addWidget(pTitle);
  if ( m_pTableView )
    addWidget(m_pTableView);
  if ( asString(option("useReadAll")) == "true" ) {
    std::string link = asString(option("link")).toUTF8();
    boost::replace_all(link, "&amp;", "&");
    std::string text = asString(option("text")).toUTF8();
    WAnchor* pAnchor = new WAnchor(WLink(WLink::InternalPath, link), text);
    pAnchor->addStyleClass("WsReadAll");
    addWidget(pAnchor);
  }
}

void WsLatestNews2::setTable()
{
  WsUser* user = WsApplication::wsInstance()->wsUser();
  std::string path = WsApplication::wsInstance()->internalPath();
  std::string sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(path);
  std::string rootPath = asString(option("rootPath")).narrow();
  boost::algorithm::replace_all(rootPath, "&amp;",  "&");
  if ( rootPath.size() < 1 ) rootPath = user->getProperty(sWithoutPrefix, WSMODLATESTNEWS, ROOT, "");
  NodePtr root;
  if (rootPath.size() == 0)
    root = user->getAccessRoot();
  else {
    //root = user->getAccessRoot(rootPath);
    root = user->getAccessRoot();
    root = root.get()->eatPath(rootPath);
  }
  if (root.get() == 0) {
    LOG(DEBUG) << "WsLatestNews2::setTable cannot get root ! root = " << root << " rootPath " << rootPath;
    return;
  }
  WsLatestNews latest(root);
  WsGlobalProperties* props = WsGlobalProperties::instance();
  int nbl = boost::lexical_cast<int>(props->get(WSMODLATESTNEWS, NB_NEWS, "10"));
  int rowsCount = asNumber(option("rowsCount"));
  if ( rowsCount > 0 ) nbl = rowsCount;
  std::vector<NodePtr> news =  latest.getLatestNews(nbl);
  WStandardItemModel* m_pModelView = new WStandardItemModel(0, 4, this);
  // TODO : add option show/hide header
  m_pModelView->setHeaderData(0, WString::tr("WsModLatestNews2-Name"));
  m_pModelView->setHeaderData(1, WString::tr("WsModLatestNews2-Desc"));
  m_pModelView->setHeaderData(2, WString::tr("WsModLatestNews2-Size"));
  m_pModelView->setHeaderData(3, WString::tr("WsModLatestNews2-Date"));
  m_pModelView->setSortRole(UserRole);
  for (int i = 0; i < news.size(); ++i) {
    if ( !(
           news[i].get()->getPath().extension().string() == ".fhtml" ||
           news[i].get()->getPath().extension().string() == ".pdf"   ||
           news[i].get()->getPath().extension().string() == ".rpg"   ||
           news[i].get()->getPath().extension().string() == ".zip"
         ) )
      continue;
    if ( news[i].get()->getProperties().get()->get("global", "in_view", "true") == "false"   ) continue;
    boost::filesystem::path wsRes = news[i].get()->getPath();
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
    std::vector<WStandardItem*> pRow;
    WDateTime dateTime;
    dateTime.setTime_t(boost::lexical_cast<time_t>(dynamic_cast<WsFileNode*>(news[i].get())->getPublishDate()));
    std::string sName = news[i].get()->getName();
    std::string sPath = news[i].get()->getPath().string();
    WStandardItem* nameItem = new WStandardItem(sName);
    nameItem->setData(sPath, UserRole);
    pRow.push_back(nameItem);
    std::string    sDesc;
    sDesc += "<p class='ls2Date'>";
    sDesc += dateTime.toString("yyyy-MM-dd").toUTF8();
    sDesc += "</p>";
    if ( asString(option("useDisplayName")) == "true" ) {
      std::string dispName = news[i].get()->getDisplayName(true);
      boost::algorithm::replace_all(dispName, "&",  "&amp;");
      sDesc += "<p>" + dispName  + "</p>";
    } else
      sDesc += news[i].get()->getProperties().get()->get( "global", "short_description", "");
    WStandardItem* descItem = new WStandardItem(sDesc);
    //    WLink lnk;
    //    lnk.setUrl("http://www.google.be");
    //    descItem->setData(lnk, LinkRole);
    descItem->setData(sDesc, UserRole);
    pRow.push_back(descItem);
    std::stringstream sstr;
    sstr << news[i].get()->getSize();
    std::string    sSize(sstr.str());
    sstr.clear();
    WStandardItem* sizeItem = new WStandardItem(sSize);
    long    lSize = news[i].get()->getSize();
    sizeItem->setData(sSize, UserRole);
    pRow.push_back(sizeItem);
    WStandardItem* dateItem = new WStandardItem(dateTime.toString("yyyy-MM-dd : hh-mm-ss"));
    dateItem->setData(dateTime, UserRole);
    pRow.push_back(dateItem);
    m_pModelView->invisibleRootItem()->appendRow(pRow);
  }
  m_pModelView->sort(3, Wt::DescendingOrder);
  m_pTableView = new WTableView();
  m_pTableView->setModel(m_pModelView);
  m_pTableView->setColumnResizeEnabled(true);
  m_pTableView->setSortingEnabled(true);
  m_pTableView->setAlternatingRowColors(true);
  m_pTableView->setSelectionMode(SingleSelection);
  m_pTableView->setColumnAlignment(2, AlignRight);
  m_pTableView->setColumnAlignment(3, AlignRight);
  long rowHeight = asNumber(option("rowHeight"));
  if ( rowHeight < 1 )
    m_pTableView->setRowHeight(38);
  else
    m_pTableView->setRowHeight(rowHeight);
  std::string  hideHeader = asString(option("hideHeader")).narrow();
  if ( hideHeader == "true" )
    m_pTableView->setHeaderHeight(0);
  std::string  columnsWidth = asString(option("columnsWidth")).narrow();
  if ( columnsWidth.size() < 1 ) {
    m_pTableView->setColumnWidth(0, 180);
    m_pTableView->setColumnWidth(1, 300);
  } else {
    std::vector<std::string> sVectColumnsWidth;
    gd_core_string2vector(sVectColumnsWidth, columnsWidth.c_str(), ",");
    for (int iCol = 0; iCol < sVectColumnsWidth.size(); ++iCol) {
      double width = asNumber(sVectColumnsWidth[iCol]);
      if ( width != -1 )
        m_pTableView->setColumnWidth(iCol, width);
    }
  }
  std::string  columnsToHide = asString(option("hideColumns")).narrow();
  std::vector<std::string> sVectColumns;
  gd_core_string2vector(sVectColumns, columnsToHide.c_str(), ",");
  for (int iCol = 0; iCol < sVectColumns.size(); ++iCol) {
    int col = asNumber(sVectColumns[iCol]);
    m_pTableView->setColumnHidden(col, true);
  }
  gdWTextDelegate* delegate2 = new gdWTextDelegate(this);
  delegate2->setRowHeight(m_pTableView->rowHeight().value());
  m_pTableView->setItemDelegateForColumn(1, delegate2);
  m_pTableView->selectionChanged().connect(SLOT(this, WsLatestNews2::onViewSelectionChanged));
}

void WsLatestNews2::onViewSelectionChanged()
{
  WModelIndexSet pSet = m_pTableView->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  wApp->log("notice") <<  "WsLatestNews2::onViewSelectionChanged() : path = " << str;
  wApp->setInternalPath(str, true);
}

