/*
 *
 *       Filename:  WsModDirectoryView2.cpp
 *
 *    Description:  Zip Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include <boost/filesystem.hpp>

#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <Wt/WString>
#include <Wt/WDateTime>
#include <Wt/WStandardItemModel>
#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWTextDelegate.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <ConfigManager/WsGlobalProperties.h>
#include <Logger/WsLogger.h>

#include <WsModule/WsModule.h>

#include "WsModDirectoryView2.h"

using namespace Wt;

extern "C" {
  void WsModDirectoryView2Init(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so!\n");
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load wt.so shared library! %s\n", dlerror());
      return;
    }
  }
}



WsDirectoryItem::WsDirectoryItem() : WStandardItem()
{ }

WsDirectoryItem::WsDirectoryItem(const WString& text) : WStandardItem(text)
{ }

void WsDirectoryItem::setNode(NodePtr pNode)
{
  m_node = pNode;
}

NodePtr WsDirectoryItem::node()
{
  return m_node;
}




WsDirectoryView2::WsDirectoryView2(WContainerWidget* parent)
  : WTableView(parent)
{
  addStyleClass("WsDirectoryView2");
  LOG(DEBUG) << "WsDirectoryView2 :: end ctor of WsDirectoryView2 !";
}

WsDirectoryView2::~WsDirectoryView2()
{
  LOG(DEBUG) << "WsDirectoryView2 :: end dtor of WsDirectoryView2 !";
}

void WsDirectoryView2::load()
{
  LOG(DEBUG) << "WsDirectoryView2 :: load !";
  WTableView::load();
  WStandardItemModel* pModelView = new WStandardItemModel(0, 4, this);
  pModelView->setHeaderData(0, WString::tr("WsDirectoryView2-Name"));
  pModelView->setHeaderData(1, WString::tr("WsDirectoryView2-Desc"));
  pModelView->setHeaderData(2, WString::tr("WsDirectoryView2-Size"));
  pModelView->setHeaderData(3, WString::tr("WsDirectoryView2-Date"));
  pModelView->setSortRole(UserRole);
  WsUser* user = WsApplication::wsInstance()->wsUser();
  std::string  path = asString(option("rootPath")).narrow();
  if ( path.size() < 1 )
    path = WsApplication::wsInstance()->internalPath();
  std::string sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(path);
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsDirectoryView2::load() : rootPath = " << path;
  //std::vector<NodePtr> vDirContent = user->getDirContents(path);
  NodePtr accessTree = user->getAccessRoot();
  if (!accessTree.get()) {
    wApp->log("error") << "WsDirectoryView2::load() : accessTree is NULL";
    return;
  }
  NodePtr tmp = accessTree.get()->eatPath(path);
  if (!tmp) {
    wApp->log("error") << "WsDirectoryView2::load() : eatPath returned null";
    return;
  }
  std::vector<NodePtr> vDirContent = tmp.get()->getAll();
  int rc = vDirContent.size();
  long rowsCount = asNumber(option("rowsCount"));
  if ( rowsCount != -1 ) rc = rowsCount;
  for (int row = 0; row < rc; ++row) {
    std::string sName   = vDirContent[row].get()->getName();
    std::string sDesc   = vDirContent[row].get()->getProperties().get()->get( "global", "short_description", "");
    uintmax_t   lSize   = vDirContent[row].get()->getSize();
    time_t      tTime   = vDirContent[row].get()->getModifyDate();
    std::string  strDate;
    struct tm    ttm;
    char         c[100];
    localtime_r(&tTime, &ttm);
    strftime(c, 100, "%Y-%m-%d", &ttm);
    strDate = c;
    std::vector<WStandardItem*> pRow;
    WsDirectoryItem*  newItem = new WsDirectoryItem(WString::fromUTF8(sName));
    newItem->setNode(vDirContent[row]);
    if ( vDirContent[row].get()->isDirectory() ) {
      newItem->setData("1" + vDirContent[row].get()->getPath().string(), UserRole); // le path complet du parent
      newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/folder.png");
    } else {
      newItem->setData("2" + vDirContent[row].get()->getPath().string(), UserRole); // le path complet du parent
      newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/file.png");
    }
    pRow.push_back(newItem);
    WsDirectoryItem*  strDesc = new WsDirectoryItem();
    strDesc->setNode(vDirContent[row]);
    strDesc->setData(sDesc, UserRole); // le path complet du parent
    pRow.push_back(strDesc);
    std::string strSize = boost::lexical_cast<std::string>(lSize);
    WsDirectoryItem* colSize = new WsDirectoryItem(strSize);
    colSize->setData(lSize, UserRole);
    pRow.push_back(colSize);
    WsDirectoryItem* colDate = new WsDirectoryItem(strDate);
    colDate->setData(strDate, UserRole);
    pRow.push_back(colDate);
    pModelView->invisibleRootItem()->appendRow(pRow);
  }
  setModel(pModelView);
  setColumnResizeEnabled(true);
  setSortingEnabled(true);
  setAlternatingRowColors(true);
  setSelectionMode(ExtendedSelection);
  setColumnAlignment(2, AlignRight);
  setColumnAlignment(3, AlignRight);
  long lRowHeight = asNumber(option("lRowHeight"));
  if ( lRowHeight < 0 )
    setRowHeight(38);
  else
    setRowHeight(lRowHeight);
  if ( asString(option("hideHeader")).narrow() == "true" )
    setHeaderHeight(0);
  std::string  columnsWidth = asString(option("columnsWidth")).narrow();
  if ( columnsWidth.size() < 1 ) {
    setColumnWidth(0, 180);
    setColumnWidth(1, 300);
  } else {
    std::vector<std::string> sVectColumnsWidth;
    gd_core_string2vector(sVectColumnsWidth, columnsWidth.c_str(), ",");
    for (int iCol = 0; iCol < sVectColumnsWidth.size(); ++iCol) {
      double width = asNumber(sVectColumnsWidth[iCol]);
      if ( width != -1 )
        setColumnWidth(iCol, width);
    }
  }
  std::string  columnsToHide = asString(option("hideColumns")).narrow();
  std::vector<std::string> sVectColumnsToHide;
  gd_core_string2vector(sVectColumnsToHide, columnsToHide.c_str(), ",");
  for (int iCol = 0; iCol < sVectColumnsToHide.size(); ++iCol) {
    int col = asNumber(sVectColumnsToHide[iCol]);
    setColumnHidden(col, true);
  }
  gdWTextDelegate* delegate2 = new gdWTextDelegate(this);
  delegate2->setRowHeight(rowHeight().value() - 2);
  setItemDelegateForColumn(1, delegate2);
  selectionChanged().connect(SLOT(this, WsDirectoryView2::onViewSelectionChanged));
  //  pModelView->sort(0);
}

void WsDirectoryView2::onViewSelectionChanged()
{
  wApp->log("notice") <<  "WsDirectoryView2::onViewSelectionChanged() : entering ...";
  WModelIndexSet pSet = selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  if ( str[0] == '1' )
    boost::algorithm::replace_first(str, "1", "");
  else
    boost::algorithm::replace_first(str, "2", "");
  wApp->log("notice") <<  "WsDirectoryView2::onViewSelectionChanged() : path = " << str;
  wApp->setInternalPath(str, true);
}









WsModDirectoryView2::WsModDirectoryView2()
  : WsModule()
{
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "WsModDirectoryView2/Transl/WsModDirectoryView2");
  LOG(DEBUG) << "WsModDirectoryView2 :: end ctor of WsModDirectoryView2 !";
}

WsModDirectoryView2::~WsModDirectoryView2()
{
  LOG(DEBUG) << "WsModDirectoryView2 :: end dtor of WsModDirectoryView2 !";
}

WWidget* WsModDirectoryView2::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModDirectoryView2::createContents(WContainerWidget* parent) const
{
  WsDirectoryView2* dirView = new WsDirectoryView2(parent);
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") <<  "WsModDirectoryView2::createContents() : prepare options ";
  dirView->setOptions(options());
  if ( asString(option("debug")) == "true" )
    dirView->outOptions("WsModDirectoryView2::createContents()");
  return dirView;
}

WsEditorWidget* WsModDirectoryView2::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModDirectoryView2::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModDirectoryView2::description() const
{
  return "WsModDirectoryView2 wittyShare module";
}

