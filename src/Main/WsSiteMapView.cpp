/*
 * Copyright (C) 2006-Today G. Deleeuw
 *
 * See the LICENSE file for terms of use.
 */

// TODO :
//        - toolbar button UP
//        - ajouter root au tree left
//        - quand tree left width change : resize column0 du treeview
//        - si pas de container pas de Hscrollbar quand resize du panel
//        - quand Vscrollbar plus de button sort

#include <stdio.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>
#include <time.h>

#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WStandardItem>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WItemDelegate>
#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include "WsSiteMapView.h"
#include "WsApplication.h"

using namespace Wt;

WsSiteMapItem::WsSiteMapItem() : WStandardItem()
{ }

WsSiteMapItem::WsSiteMapItem(const WString& text) : WStandardItem(text)
{ }

void WsSiteMapItem::setNode(NodePtr pNode)
{
  m_node = pNode;
}

NodePtr WsSiteMapItem::node()
{
  return m_node;
}





WsSiteMapView::WsSiteMapView(gdwtcore::viewType nViewType, gdWFileView::tbFlags flags, WContainerWidget* parent)
  : WContainerWidget(parent), m_viewType(nViewType), m_tbFlags(flags), m_pModelTree(0), m_pModelView(0), m_pTVTree(0), m_pTVView(0),
    m_pTb(0), m_bOnDataChanged(false), m_pMenu(0), m_bOnRename(false), m_bLogSiteMap(false), m_pMb(0)

{
  //  if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsSiteMapView.css");
  addStyleClass("WsSiteMapView");
  if (WsLayoutProperties::instance()->get("sitemap", "log", "false") == "true")
    m_bLogSiteMap = true;
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "wittyshare/Transl/WsSiteMapView");
  if ( m_tbFlags == gdWFileView::tbDefault ) {
    int nTmp = gdWFileView::tbDefault | gdWFileView::tbUseDummy;
    m_tbFlags = (gdWFileView::tbFlags) nTmp;
  }
  if ( m_bLogSiteMap )
    wApp->log("notice") << "WsSiteMapView::WsSiteMapView() start";
  //  resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
  setOverflow(WContainerWidget::OverflowHidden);
  WsUser*         pUser          = WsApp->wsUser();
  m_sRootPath    = pUser->getRootPath();                        // /var/www/demo_site
  m_sSiteMap     = WsLayoutProperties::instance()->get("sitemap", "root", "/SiteMap");  // /SiteMap
  m_sRootSiteMap = m_sRootPath + m_sSiteMap;                    // /var/www/demo_site/SiteMap
  m_pTreecurNode = pUser->getAccessRoot();
  load();
}

WsSiteMapView::~WsSiteMapView()
{
  if ( m_bLogSiteMap )
    wApp->log("notice") << "WsSiteMapView::~WsSiteMapView()";
}

void WsSiteMapView::load()
{
  WContainerWidget::load();
  if ( asString(option("debug")) == "true" )
    wApp->log("notice") << "WsSiteMapView::load()";
  m_pModelTree = new WStandardItemModel(0, 1, this);
  m_pModelTree->setHeaderData(0, WString::tr("WsSiteMapView-Name"));
  m_pModelTree->setSortRole(UserRole);
  std::vector<WStandardItem*> pRow;
  WsSiteMapItem*  newItem        = new WsSiteMapItem(m_sSiteMap);
  newItem->setData("1" + m_sRootSiteMap, UserRole); // le path complet
  newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/folder.png");
  pRow.push_back(newItem);
  // TODO : creer une node et call setNode
  //m_pModelTree->invisibleRootItem()->appendRow(pRow);
  m_pTVTree = new WTreeView();
  //  m_pTVTree->setLayoutSizeAware(true);
  m_pTVTree->setModel(m_pModelTree);
  m_pTVTree->setColumnResizeEnabled(true);
  m_pTVTree->setSortingEnabled(true);
  m_pTVTree->setSelectionMode(SingleSelection);
  m_pTVTree->selectionChanged().connect(SLOT(this, WsSiteMapView::onTreeSelectionChanged));
  //  m_pTVTree->setDropsEnabled(true);
  // avec Node  m_pTVTree->expand(newItem->index());
  m_pTVTree->expanded().connect(SLOT(this, WsSiteMapView::onTreeExpanded));
  //  double dblWidth = (parent->width().value() * 30) / 100;
  //  m_pTVTree->resize(WLength(dblWidth, parent->width().unit()), WLength::Auto);
  //  m_pTVTree->resize(WLength::Auto, 550);
  //  m_pTVTree->resize(WLength::Auto, WLength::Auto);
  //  m_pTVTree->resize(WLength(30, WLength::Percentage), WLength(100, WLength::Percentage));
  m_pModelView = new WStandardItemModel(0, 3);
  m_pModelView->setHeaderData(0, WString::tr("WsSiteMapView-Name"));
  m_pModelView->setHeaderData(1, WString::tr("WsSiteMapView-Size"));
  m_pModelView->setHeaderData(2, WString::tr("WsSiteMapView-Date"));
  m_pModelView->setSortRole(UserRole);
  m_pModelView->itemChanged().connect(SLOT(this, WsSiteMapView::onDirectoryNameChanged));
  //TODO : implement the load method for the treeView and treeTree
  if ( m_tbFlags & gdWFileView::tbUseLeftTree )
    loadTree(m_pModelTree->invisibleRootItem(), m_pTreecurNode, gdWFileView::noSkipParent);
  m_pTVView = new WTreeView();
  m_pTVView->setModel(m_pModelView);
  m_pTVView->setColumnResizeEnabled(true);
  m_pTVView->setSortingEnabled(true);
  m_pTVView->setAlternatingRowColors(true);
  //  m_pTVView->setColumnWidth(0, 200);
  m_pTVView->setColumnWidth(1,  80);
  m_pTVView->setColumnWidth(2,  80);
  //  m_pTVView->setRowHeight(14);
  m_pTVView->setSelectionMode(ExtendedSelection);
  m_pTVView->setColumnAlignment(1, AlignRight);
  m_pTVView->doubleClicked().connect(SLOT(this, WsSiteMapView::onViewDblClick));
  m_pTVView->selectionChanged().connect(SLOT(this, WsSiteMapView::onViewSelectionChanged));
  m_pTVView->expanded().connect(SLOT(this, WsSiteMapView::onViewExpanded));
  //  m_pTVView->setDragEnabled(true);
  m_pTVView->setEditTriggers(WAbstractItemView::SingleClicked);
  WItemDelegate* delegate = new WItemDelegate(this);
  delegate->setTextFormat("%s");
  m_pTVView->setItemDelegate(delegate);
  // Pas necessaire, sera declenche par setRelativePath ?
  //  init(m_pTreecurNode);
  WText* pTitle = new WText("Site Map");
  pTitle->addStyleClass("WsTitle");
  WHBoxLayout* hbox = new WHBoxLayout();
  if ( m_tbFlags & gdWFileView::tbUseLeftTree )
    hbox->addWidget(m_pTVTree, 1);
  hbox->addWidget(m_pTVView, 2);
  if ( m_tbFlags & gdWFileView::tbUseLeftTree )
    hbox->setResizable(0, true, WLength(33, WLength::Percentage));
  WVBoxLayout* vbox = new WVBoxLayout();
  vbox->addWidget(pTitle, 0);
  if ( m_tbFlags & gdWFileView::tbUseToolbar ) {
    makeToolbar();
    vbox->addWidget(m_pTb, 0);
    if ( m_bLogSiteMap )
      wApp->log("notice") << "WsSiteMapView::WsSiteMapView() add toolBar";
  }
  vbox->addLayout(hbox, 1);
  setLayout(vbox);
  if ( m_bLogSiteMap )
    wApp->log("notice") << "WsSiteMapView::WsSiteMapView() end";
}

std::string WsSiteMapView::path2SiteMapPath(const std::string& path)
{
  std::string str(path);
  boost::algorithm::replace_first(str, m_sRootSiteMap, m_sRootPath);
  return str;
}

std::string WsSiteMapView::SiteMapPath2path(const std::string& siteMapPath)
{
  std::string str(siteMapPath);
  boost::algorithm::replace_first(str, m_sRootPath, m_sRootSiteMap);
  return str;
}

void WsSiteMapView::setRelativePath(const std::string& relativePath)
{
  if ( !loaded() ) load();
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::setRelativePath() = start with " << relativePath;
  std::string str(relativePath);
  boost::algorithm::replace_first(str, m_sSiteMap, "");
  if ( str.size() < 1 )
    str = "/";
  WsUser*         pUser          = WsApp->wsUser();
  NodePtr node           = m_pTreecurNode.get()->eatPath(str);
  //TODO FIXME redirect to not found page here
  if (!node.get())
    return;
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::setRelativePath() = " << str << " and node path = " << (node ? node->getPath().string() : " No path in Node");
  init(node);
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::setRelativePath() = end with " << node;
}

void WsSiteMapView::init(NodePtr pNode)
{
  if (!pNode.get())
    return;
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::init() start with node " << (pNode.get() ? pNode.get()->getPath().string() : " No path in Node");
  if ( m_pModelView->rowCount() )
    m_pModelView->removeRows(0, m_pModelView->rowCount());
  if ( m_tbFlags & gdWFileView::tbUseLeftTree )
    treeMatchPath(pNode);
  else if ( !loadTree(m_pModelView->invisibleRootItem(), pNode, gdWFileView::noSkipParent) ) {
    if ( m_bLogSiteMap )
      wApp->log("notice") <<  "WsSiteMapView::init() return with bad loadTree !!! ";
    return;
  }
  m_selectedDirectory = m_sRootSiteMap + pNode.get()->getPath().string();
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::init() set selectedDirectory to : "  << m_selectedDirectory;
}

bool WsSiteMapView::treeTraverse(WStandardItem* rootItem, const std::vector<std::string>& vPath, int nLevel)
{
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::treeTraverse() start with node = "  << rootItem->text() << " level " << nLevel;
  if ( nLevel >= vPath.size() ) return false;
  for (int nItem = 0; nItem < rootItem->rowCount(); ++nItem) {
    WsSiteMapItem* tmpItem = dynamic_cast<WsSiteMapItem*>(rootItem->child(nItem, 0));
    std::string str = tmpItem->text().toUTF8();
    if ( m_bLogSiteMap )
      wApp->log("notice") <<  "WsSiteMapView::treeTraverse() compare : "  << vPath[nLevel] << " str = " << str;
    if ( (vPath[nLevel]) == str )
      if ( nLevel == vPath.size() - 1 ) {
        if ( m_bLogSiteMap )
          wApp->log("notice") <<  "WsSiteMapView::treeTraverse() equal !";
        m_pTVTree->select(tmpItem->index());
        m_pTVTree->scrollTo(tmpItem->index());
        return true;
      } else {
        if ( m_bLogSiteMap )
          wApp->log("notice") <<  "WsSiteMapView::treeTraverse() not equal !";
        m_pTVTree->expand(tmpItem->index());
        onTreeExpanded(tmpItem->index());
        return treeTraverse(tmpItem, vPath, ++nLevel);
      }
  }
  return false;
}

void WsSiteMapView::treeMatchPath(NodePtr pNode)
{
  if (pNode.get() == 0)
    return;
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::treeMatchPath() start with node = "  << pNode.get()->getPath().string();
  WsUser*                     pUser    = WsApp->wsUser();
  std::string                 myPath(pNode.get()->getPath().string());
  std::string                 rootPath = m_pTreecurNode.get()->getPath().string();
  std::vector<std::string>    vPath;
  NodePtr             curNode = pNode;
  // Bug dans getAccessRoot ?
  if ( rootPath.size() < 1 )
    rootPath = "/";
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::treeMatchPath() myPath = "  << myPath << " and rootPath = " << rootPath;
  vPath.push_back(rootPath);
  if ( pNode != m_pTreecurNode )
    while (curNode.get() != 0 && curNode.get()->getParent() ) {
      curNode = curNode.get()->getParent();
      if ( m_bLogSiteMap )
        wApp->log("notice") <<  "WsSiteMapView::treeMatchPath() add to vector = "  << myPath;
      vPath.insert(vPath.begin() + 0, myPath);
      myPath = curNode.get()->getPath().string();
      if ( curNode != m_pTreecurNode ) break;
    }
  if ( !treeTraverse(m_pModelTree->invisibleRootItem(), vPath, 0) ) {
    if ( m_bLogSiteMap )
      wApp->log("notice") <<  "WsSiteMapView::treeMatchPath() treetraverse ";
  }
  loadTree(m_pModelView->invisibleRootItem(), pNode, gdWFileView::noSkipParent);
  WModelIndexSet  pSet  = m_pTVTree->selectedIndexes();
  if ( pSet.size() )
    m_pTVTree->scrollTo(*pSet.begin());
}

/* loadTree et loadFile utilise le path complet dans data UserRole, prefixe 1 = directory, 2 = file
     ceci afin de conserver les repertoires groupes lors d'un tri colonne.
*/
bool WsSiteMapView::loadTree(WStandardItem* currentItem, NodePtr pNode, gdWFileView::skipMode skipParentDir)
{
  WsUser*                     pUser    = WsApp->wsUser();
  if (!pNode.get())
    return false;
  if ( m_bLogSiteMap )
    if ( currentItem->model() == m_pModelTree )
      if ( m_bLogSiteMap )
        wApp->log("notice") <<  "WsSiteMapView::loadTree() m_pModelTree pNode full path =  "  << pNode.get()->getPath().string();
      else if ( m_bLogSiteMap )
        wApp->log("notice") <<  "WsSiteMapView::loadTree() m_pModelView pNode full path =  "  << pNode.get()->getPath().string();
  if ( pNode.get()->isDirectory() ) {
    if ( !skipParentDir && (pNode != m_pTreecurNode) && !(m_tbFlags & gdWFileView::tbUseLeftTree) ) {
      std::vector<WStandardItem*> pRow;
      std::string             newPath = pNode.get()->getPath().string();
      WsSiteMapItem*          newItem = new WsSiteMapItem("..");
      newItem->setData("1" + path2SiteMapPath(newPath), UserRole);
      newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/folder.png");
      pRow.push_back(newItem);
      pRow.push_back(new WsSiteMapItem());
      pRow.push_back(new WsSiteMapItem());
      currentItem->appendRow(pRow);
    }
    std::vector<NodePtr> tmpNode;
    std::vector<NodePtr>& dirNode = tmpNode;
    if ( currentItem->model() == m_pModelTree )  dirNode = pNode.get()->getDirectories();
    else                                        dirNode = pNode.get()->getAll();
    if ( m_bLogSiteMap )
      wApp->log("notice") <<  "WsSiteMapView::loadTree() dirNode size =  "  << dirNode.size();
    for (std::vector<NodePtr>::iterator it = dirNode.begin(); it != dirNode.end(); ++it) {
      NodePtr curNode = *it;
      // Items not displayed in view are still visible in the sitemap to the administrator
      if ( !(pUser->isAdministrator() || pUser->isEditor()) )
        if ( curNode.get()->getProperties().get()->get("global", "in_view", "true") != "true" ) continue;
      // Do not display ws.res directories in the sitemap
      if (curNode.get()->getName() == "ws.res") continue;
      std::string     curPath = m_sRootPath + curNode.get()->getPath().string();
      if ( m_bLogSiteMap )
        wApp->log("notice") <<  "WsSiteMapView::loadTree() curPath =  "  << curPath;
      if ( curNode.get()->isDirectory() ) {
        if ( m_bLogSiteMap )
          wApp->log("notice") <<  "WsSiteMapView::loadTree() detected directory";
        if ( !loadFolder(currentItem, curPath, curNode) ) return false;
      } else {
        if ( m_bLogSiteMap )
          wApp->log("notice") <<  "WsSiteMapView::loadTree() detected file";
        if ( currentItem->model() == m_pModelTree ) continue;
        if ( !loadFile(currentItem, curPath, curNode) ) return false;
      }
    }
    if ( currentItem->model() == m_pModelView )
      m_pModelView->sort(0);
  }
  return true;
}

bool WsSiteMapView::loadFolder(WStandardItem* currentItem, const std::string& newPath, NodePtr curNode, bool bEdit)
{
  WsSiteMapItem*  newItem = new WsSiteMapItem(WString::fromUTF8(curNode.get()->getName())); // le nom.extension du fichier
  newItem->setNode(curNode);
  newItem->setData("1" + newPath, UserRole); // le path complet
  newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/folder.png");
  if ( currentItem->model() == m_pModelTree ) {
    if ( m_bLogSiteMap )
      wApp->log("notice") <<  "WsSiteMapView::loadFolder() start with node = "  << curNode.get()->getPath().string();
    //     newItem->setFlags(newItem->flags() | ItemIsDropEnabled);
    currentItem->appendRow(newItem);
  } else {
    std::vector<WStandardItem*> pRow;
    pRow.push_back(newItem);
    pRow.push_back(new WsSiteMapItem());
    pRow.push_back(new WsSiteMapItem());
    currentItem->appendRow(pRow);
    if ( bEdit ) {
      //        m_pModelView->sort(0);
      newItem->setFlags(ItemIsSelectable | ItemIsEditable);
      WModelIndexSet  pSet;
      m_pTVView->setSelectedIndexes(pSet);
      m_pTVView->edit(newItem->index());
    }
  }
  if ( curNode.get()->getDirectories().size() == 0 ) return true;
  if ( currentItem->model() == m_pModelTree )
    if ( m_tbFlags & gdWFileView::tbUseDummy )
      newItem->appendRow(new WsSiteMapItem());
    else
      return loadTree(newItem, newItem->node(), gdWFileView::noSkipParent);
  else {
    if ( m_tbFlags & gdWFileView::tbUseLeftTree ) return true;
    std::vector<WStandardItem*> pRowChild;
    pRowChild.push_back(new WsSiteMapItem());
    pRowChild.push_back(new WsSiteMapItem());
    pRowChild.push_back(new WsSiteMapItem());
    newItem->appendRow(pRowChild);
  }
  return true;
}

bool WsSiteMapView::loadFile(WStandardItem* currentItem, const std::string& currentPath, NodePtr curNode)
{
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::loadFile() full path =  "  << currentPath;
  long fsize           = curNode.get()->getSize();
  std::string  strSize = boost::lexical_cast<std::string>(fsize);
  std::string  strDate;
  std::time_t  t       = curNode.get()->getModifyDate();
  struct tm    ttm;
  char         c[100];
  localtime_r(&t, &ttm);
  strftime(c, 100, "%Y-%m-%d", &ttm);
  strDate = c;
  std::vector<WStandardItem*> pRow;
  WsSiteMapItem*  newItem = new WsSiteMapItem(WString::fromUTF8(curNode.get()->getName())); // le nom.extension du fichier
  newItem->setNode(curNode);
  newItem->setData("2" + currentPath, UserRole); // le path complet du parent
  //  newItem->setFlags(newItem->flags() | ItemIsDragEnabled);
  newItem->setIcon(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/file.png");
  pRow.push_back(newItem);
  WsSiteMapItem* colSize = new WsSiteMapItem(strSize);
  colSize->setData(fsize, UserRole);
  pRow.push_back(colSize);
  WsSiteMapItem* colDate = new WsSiteMapItem(strDate);
  colDate->setData(strDate, UserRole);
  pRow.push_back(colDate);
  currentItem->appendRow(pRow);
  return true;
}

void WsSiteMapView::onTreeSelectionChanged()
{
  WModelIndexSet pSet = m_pTVTree->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  char type = str[0];
  str.erase(0, 1);
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::onTreeSelectionChanged() : type = " << type << " path = " << str.c_str();
  boost::algorithm::replace_all(str, m_sRootPath, m_sSiteMap);
  m_selectedDirectory = str;
  m_selectedFile.clear();
  WsSiteMapItem* pItem = dynamic_cast<WsSiteMapItem*>(m_pModelTree->itemFromIndex(idx));
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::onTreeSelectionChanged() : text = " << pItem->text() << " node string = " << pItem->node()->getPath().string();
  if ( m_tbFlags & gdWFileView::tbUseDummy )
    if ( !loadTree(pItem, pItem->node(), gdWFileView::skipParent) ) return;
  m_pModelView->invisibleRootItem()->removeRows(0, m_pModelView->invisibleRootItem()->rowCount());
  if ( !loadTree(m_pModelView->invisibleRootItem(), pItem->node(), gdWFileView::skipParent) ) return;
  wApp->setInternalPath(str, true);
  //  fileSelected().emit(gdWFileView::none, str);
}

void WsSiteMapView::onTreeExpanded(WModelIndex idx)
{
  if ( idx.column() != 0 ) return;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  char type = str[0];
  str.erase(0, 1);
  if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::onTreeExpanded() : type = " << type << " path = " << str.c_str();
  WsSiteMapItem* pItem = dynamic_cast<WsSiteMapItem*>(m_pModelTree->itemFromIndex(idx));
  if ( m_tbFlags & gdWFileView::tbUseDummy ) {
    pItem->removeRows(0, pItem->rowCount());
    if ( !loadTree(pItem, pItem->node(), gdWFileView::skipParent) ) return;
    pItem->sortChildren(0, Wt::AscendingOrder);
  }
}


void WsSiteMapView::onViewSelectionChanged()
{
  WModelIndexSet pSet = m_pTVView->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  char type = str[0];
  str.erase(0, 1);
  //  if ( m_bLogSiteMap )
  wApp->log("notice") <<  "WsSiteMapView::onViewSelectionChanged() : type = " << type << " path = " << str;
  boost::algorithm::replace_first(str, m_sRootPath, m_sSiteMap);
  if ( type == '2' ) {
    m_selectedDirectory.clear();
    m_selectedFile = str;
    fileSelected().emit(gdWFileView::selected, m_selectedFile);
    //     boost::algorithm::replace_first(str, "/SiteMap", "");
    //     wApp->setInternalPath(str, true);
  } else {
    //     WsSiteMapItem* pItem = dynamic_cast<WsSiteMapItem*>(m_pModelView->itemFromIndex(idx));
    m_selectedFile.clear();
    m_selectedDirectory = str;
    fileSelected().emit(gdWFileView::none, str);
  }
}

void WsSiteMapView::onViewDblClick(WModelIndex idx, WMouseEvent mouseEvent)
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
  char type = str[0];
  str.erase(0, 1);
  //  if ( m_bLogSiteMap )
  wApp->log("notice") <<  "WsSiteMapView::onViewDblClick() : type = " << type << " path = " << str;
  boost::algorithm::replace_first(str, m_sRootPath, m_sSiteMap);
  //fprintf(stderr, "onDblclick - Type = %c, Selected path = %s\n", type, str.c_str());
  if ( type == '1' ) {
    //init(str);
  } else {
    m_selectedFile = str;
    //     if ( m_bLogSiteMap )
    wApp->log("notice") <<  "WsSiteMapView::onViewDblClick() : type = " << type << " path = " << str;
    fileSelected().emit(gdWFileView::doubleClick, m_selectedFile);
  }
}

void WsSiteMapView::onViewExpanded(WModelIndex idx)
{
  if ( idx.column() != 0 ) return;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  char type = str[0];
  str.erase(0, 1);
  //fprintf(stderr, "onViewExpanded - Type = %c, Selected path = %s\n", type, str.c_str());
  WsSiteMapItem* pItem = dynamic_cast<WsSiteMapItem*>(m_pModelView->itemFromIndex(idx));
  if ( type == '1' ) {
    pItem->removeRows(0, pItem->rowCount());
    //if ( !loadTree(pItem, str, gdWFileView::skipParent) ) return;
    //     m_pModelView->sort(0);
  }
}

void WsSiteMapView::setCurrentPath(const std::string& currentPath)
{
  //init(currentPath);
}

std::string WsSiteMapView::selectedDirectory()
{
  return m_selectedDirectory;
}

std::string WsSiteMapView::selectedFile()
{
  return m_selectedFile;
}

std::string WsSiteMapView::selectedPath()
{
  if ( gdcore_isPathFile(m_selectedFile) ) return m_selectedFile;
  else                                    return m_selectedDirectory;
}

std::string WsSiteMapView::currentFolderPath()
{
  //return m_fsRootPath.string();
}

void WsSiteMapView::makeToolbar()
{
  m_pTb = new gdToolbar();
  m_tbiNewFolder = m_pTb->addToolbarItem(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/folder_new.png", WString::tr("WsSiteMapView-newfolder").toUTF8(), WString::tr("WsSiteMapView-newfolder-tooltips").toUTF8());
  m_tbiNewFolder->clicked().connect(SLOT(this, WsSiteMapView::doNewFolder));
  m_tbiDelete = m_pTb->addToolbarItem(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/Delete.png", WString::tr("WsSiteMapView-delete").toUTF8(), WString::tr("WsSiteMapView-delete-tooltips").toUTF8());
  m_tbiDelete->clicked().connect(SLOT(this, WsSiteMapView::doConfirmDelete));
  m_tbiRename = m_pTb->addToolbarItem(wApp->theme()->resourcesUrl() + "gdwtcore/Icons/rename.png", WString::tr("WsSiteMapView-rename").toUTF8(), WString::tr("WsSiteMapView-rename-tooltips").toUTF8());
  m_tbiRename->clicked().connect(SLOT(this, WsSiteMapView::doRename));
}

void WsSiteMapView::doRename()
{
  WModelIndexSet pSet = m_pTVView->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  WsSiteMapItem* pItem = dynamic_cast<WsSiteMapItem*>( m_pModelView->itemFromIndex(m_pModelView->index(idx.row(), 0, m_pModelView->parent(idx))) );
  pItem->setFlags(ItemIsSelectable | ItemIsEditable);
  WModelIndexSet  pSet2;
  m_bOnRename = true;
  m_pTVView->setSelectedIndexes(pSet2);
  m_pTVView->edit(pItem->index());
}

void WsSiteMapView::doConfirmDelete()
{
  WModelIndexSet pSet = m_pTVView->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  str.erase(0, 1);
  //boost::algorithm::replace_all(str, m_fsRootPath.string(), "");
  m_pMb = new WMessageBox("Confirm", "Remove " + str + " ?", Wt::Question, Wt::Ok | Wt::Cancel);
  m_pMb->buttonClicked().connect(SLOT(this, WsSiteMapView::doDelete));
  m_pMb->show();
}

void WsSiteMapView::doDelete(Wt::StandardButton button)
{
  delete m_pMb;
  if ( button != Wt::Ok ) return;
  WModelIndexSet pSet = m_pTVView->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  if ( pSet.empty() ) return;
  WModelIndex    idx = *it;
  std::string str = asString(idx.data(UserRole)).toUTF8();
  char type = str[0];
  str.erase(0, 1);
  try {
    boost::filesystem::remove_all(str);
  } catch (boost::filesystem::filesystem_error& e) {
    std::cerr << e.what() << std::endl;
    return;
  }
  fprintf(stderr, "delete item = %s\n", str.c_str());
  m_pModelView->removeRow(idx.row(), idx.parent());
  boost::filesystem::path newPath = str;
  m_selectedDirectory = newPath.parent_path().string();
  if ( m_tbFlags & gdWFileView::tbUseLeftTree && type == '1' ) {
    //treeMatchPath(str);
    pSet = m_pTVTree->selectedIndexes();
    if ( !pSet.empty() ) {
      it = pSet.begin();
      WModelIndex    idx = *it;
      WsSiteMapItem* treeItem = dynamic_cast<WsSiteMapItem*>(m_pModelTree->itemFromIndex(idx));
      treeItem->parent()->removeRow(treeItem->row());
      //treeMatchPath(newPath.parent_path().string());
    }
  }
}

void WsSiteMapView::doNewFolder()
{
  boost::filesystem::path newPath = currentFolderPath() + "/NewFolder";
  //loadFolder(m_pModelView->invisibleRootItem(), newPath, true);
}

void WsSiteMapView::onDirectoryNameChanged(WStandardItem* pItem)
{
  if ( pItem->index().column() != 0 ) return;
  if ( m_bOnDataChanged ) return;
  m_bOnDataChanged = true;
  std::string newName = asString(pItem->data(EditRole)).toUTF8();
  std::string oldPath = asString(pItem->data(UserRole)).toUTF8();
  char type = oldPath[0];
  oldPath.erase(0, 1);
  pItem->setFlags(ItemIsSelectable);
  std::string newPath = currentFolderPath() + "/" + newName;
  pItem->setData(type + newPath, UserRole); // le path complet
  m_bOnDataChanged = false;
  bool bOnRename = m_bOnRename;
  m_bOnRename = false;
  if ( !bOnRename ) {
    fprintf(stderr, "create new directory = %s\n", newPath.c_str());
    try {
      boost::filesystem::create_directory(newPath);
    } catch (boost::filesystem::filesystem_error& e) {
      std::cerr << e.what() << std::endl;
      return;
    }
  } else {
    fprintf(stderr, "Rename item = %s in %s\n", oldPath.c_str(), newPath.c_str());
    try {
      boost::filesystem::rename(oldPath, newPath);
    } catch (boost::filesystem::filesystem_error& e) {
      std::cerr << e.what() << std::endl;
      return;
    }
  }
  if ( type != '1' ) return;
  WModelIndexSet pSet = m_pTVTree->selectedIndexes();
  WModelIndexSet::iterator it = pSet.begin();
  WsSiteMapItem* treeItem = dynamic_cast<WsSiteMapItem*>(m_pModelTree->invisibleRootItem());
  if ( !pSet.empty() ) {
    WModelIndex    idx = *it;
    WsSiteMapItem* treeItem = dynamic_cast<WsSiteMapItem*>(m_pModelTree->itemFromIndex(idx));
  }
  if ( !bOnRename );
  //loadFolder(treeItem, newPath);
  else {
    oldPath = "1" + oldPath;
    for (int iChild = 0; iChild < treeItem->rowCount(); ++iChild) {
      std::string tvPath = asString(treeItem->child(iChild, 0)->data(UserRole)).toUTF8();
      if ( tvPath == oldPath ) {
        treeItem->child(iChild, 0)->setText(newName);
        treeItem->setData(type + newPath, UserRole); // le path complet
      }
    }
  }
  treeItem->sortChildren(0, Wt::AscendingOrder);
}

