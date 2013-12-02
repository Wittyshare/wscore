/*
 * Copyright (C) 2006-Today G. Deleeuw
 *
 * See the LICENSE file for terms of use.
 */

#include <stdio.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <time.h>

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WText>
#include <Wt/WItemDelegate>
#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <Search/WsSearch.h>

#include "WsSearchView.h"
#include "WsApplication.h"

using namespace Wt;

WsSearchItem::WsSearchItem() : WStandardItem(), m_node(0)
{ }

WsSearchItem::WsSearchItem(const WString& text) : WStandardItem(text), m_node(0)
{ }

void WsSearchItem::setNode(WsAbstractNode* pNode)
{
  m_node = pNode;
}

WsAbstractNode* WsSearchItem::node()
{
  return m_node;
}





WsSearchView::WsSearchView(std::string sSearch, WContainerWidget* parent)
  : WTableView(parent), m_pModelView(0), m_bLogSearch(true), m_wNavBar(0)
{
  //if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsSearchView.css");
  parent->addStyleClass("WsSearchView");
  if (WsLayoutProperties::instance()->get("sitemap", "log", "false") == "true")
    m_bLogSearch = true;
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "wittyshare/Transl/WsSearchView");
  if ( m_bLogSearch )
    wApp->log("notice") << "WsSearchView::WsSearchView() start";
  m_pModelView = new WStandardItemModel(0, 4);
  m_pModelView->setHeaderData(0, WString::tr("WsSearchView-Name"));
  m_pModelView->setHeaderData(1, WString::tr("WsSearchView-Desc"));
  m_pModelView->setHeaderData(2, WString::tr("WsSearchView-Size"));
  m_pModelView->setHeaderData(3, WString::tr("WsSearchView-Date"));
  m_pModelView->setSortRole(UserRole);
  WsUser*                     pUser          = WsApp->wsUser();
  m_sRootPath    = pUser->getRootPath();                        // /var/www/demo_site
  NodePtr             pRootNode      = pUser->getAccessRoot();
  if ( !pRootNode.get() ) {
    wApp->log("notice") <<  "WsSearchView::WsSearchView() : getAccessRoot() is null";
    return;
  }
  std::vector<WsResultItem> vResSearch = pUser->getSearchResults(sSearch);
  if ( m_bLogSearch )
    wApp->log("notice") << "WsSearchView::WsSearchView() search results contain : " <<  vResSearch.size() << " rows";
  for (int row = 0; row < vResSearch.size(); ++row) {
    NodePtr pNode        = pRootNode.get()->eatPath(vResSearch[row].getPath().string());
    if ( !pNode.get() ) {
      wApp->log("notice") <<  "WsSearchView::WsSearchView() : Path = " << ", getAccessRoot() eatPath is null";
      continue;
    }
    if ( pNode.get()->isDirectory() ) continue;
    if ( pNode.get()->getProperties().get()->get("global", "in_view", "true") != "true" ) continue;
    // split sSearch separator space, then replace sDesc :"?<splitX>?" by "<b>splitX</b>"
    std::string sName   = vResSearch[row].getName(true);
    std::string sDesc   = vResSearch[row].getBody();
    long        lSize   = vResSearch[row].getSize();
    time_t      tTime   = vResSearch[row].getModifyDate();
    //    wApp->log("notice") << "WsSearchView::WsSearchView() tTime = " << tTime;
    std::vector<std::string>   searchWords;
    gd_core_string2vector(searchWords, sSearch.c_str(), " ");
    for (int iWord = 0; iWord < searchWords.size(); ++iWord) {
      boost::algorithm::replace_all(sDesc, "\u0002", "<strong>");  // u0002 = start text
      boost::algorithm::replace_all(sDesc, "\u0003", "</strong>"); // u0003 = end text
    }
    std::string  strDate;
    struct tm    ttm;
    char         c[100];
    localtime_r(&tTime, &ttm);
    strftime(c, 100, "%Y-%m-%d", &ttm);
    strDate = c;
    if ( m_bLogSearch )
      wApp->log("notice") << "WsSearchView::WsSearchView() item " << row << " = " << sName << " size = " << lSize << " date " << strDate << " full path = " << vResSearch[row].getPath().string();
    std::vector<WStandardItem*> pRow;
    WsSearchItem*  newItem = new WsSearchItem(WString::fromUTF8(sName));
    newItem->setNode(&vResSearch[row]);
    newItem->setData(vResSearch[row].getPath().string(), UserRole); // le path complet du parent
    newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/file.png");
    pRow.push_back(newItem);
    WsSearchItem*  strDesc = new WsSearchItem();
    strDesc->setNode(&vResSearch[row]);
    strDesc->setData(sDesc, UserRole); // le path complet du parent
    pRow.push_back(strDesc);
    std::string strSize = boost::lexical_cast<std::string>(lSize);
    WsSearchItem* colSize = new WsSearchItem(strSize);
    colSize->setData(lSize, UserRole);
    pRow.push_back(colSize);
    WsSearchItem* colDate = new WsSearchItem(strDate);
    colDate->setData(strDate, UserRole);
    pRow.push_back(colDate);
    m_pModelView->invisibleRootItem()->appendRow(pRow);
  }
  //  m_wNavBar = createPageNavigationBar();
  setModel(m_pModelView);
  setColumnResizeEnabled(true);
  setSortingEnabled(true);
  setAlternatingRowColors(true);
  setSelectionMode(ExtendedSelection);
  setColumnAlignment(2, AlignRight);
  setRowHeight(38);
  setColumnWidth(0, 300);
  setColumnWidth(1, 470);
  setColumnWidth(2,  80);
  setColumnWidth(3,  80);
  setHeight((rowHeight() * 50)); // 50 rows, TODO push this object as a module and set this as option
  WItemDelegate* delegate = new WItemDelegate(parent);
  delegate->setTextFormat("%s");
  setItemDelegate(delegate);
  gdWTextDelegate* delegate2 = new gdWTextDelegate(parent);
  delegate2->setRowHeight(rowHeight().value() - 2);
  setItemDelegateForColumn(1, delegate2);
  doubleClicked().connect(SLOT(this, WsSearchView::onViewDblClick));
  selectionChanged().connect(SLOT(this, WsSearchView::onViewSelectionChanged));
  if ( m_bLogSearch )
    wApp->log("notice") << "WsSearchView::WsSearchView() end";
}

WsSearchView::~WsSearchView()
{
  if ( m_bLogSearch )
    wApp->log("notice") << "WsSearchView::~WsSearchView() start";
  if ( m_pModelView ) {
    delete m_pModelView;
    m_pModelView = 0;
  }
  if ( m_bLogSearch )
    wApp->log("notice") << "WsSearchView::~WsSearchView() stop";
}

WWidget* WsSearchView::navBar()
{
  return m_wNavBar;
}

void WsSearchView::onViewDblClick(WModelIndex idx, WMouseEvent mouseEvent)
{
  WModelIndex    idxcol0;
  if ( idx.column() != 0 ) {
    if ( idx.parent().isValid() )
      idxcol0 = idx.parent().child(idx.row(), 0);
    else
      idxcol0 = idx.child(idx.row(), 0);
  } else
    idxcol0 = idx;
  if ( !idxcol0.isValid() ) return;
  std::string str = asString(idxcol0.data(UserRole)).toUTF8();
  wApp->log("notice") <<  "WsSearchView::onViewDblClick() : path = " << str;
  wApp->setInternalPath(str, true);
}

void WsSearchView::onViewSelectionChanged()
{
  WModelIndexSet pSet = selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  wApp->log("notice") <<  "WsSearchView::onViewSelectionChanged() : path = " << str;
  wApp->setInternalPath(str, true);
}

