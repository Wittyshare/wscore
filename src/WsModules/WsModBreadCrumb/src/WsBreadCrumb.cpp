/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/exception.hpp>

#include <Wt/WStackedWidget>
#include <Wt/WTheme>

#include <Main/WsApplication.h>

#include "WsBreadCrumb.h"

using namespace Wt;

WsBreadCrumb::breadCrump::breadCrump()
{ }

WsBreadCrumb::breadCrump::~breadCrump()
{
  wApp->log("notice") << "breadCrumb::~breadCrumb() deleting title = " << title;
}






WsBreadCrumb::WsBreadCrumb(WContainerWidget* parent)
  : WContainerWidget(parent), m_pMenu(0), m_bDebug(false)
{
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsBreadcrumb.css");
  addStyleClass("WsBreadcrumb");
}

WsBreadCrumb::~WsBreadCrumb()
{ }

void WsBreadCrumb::load()
{
  WContainerWidget::load();
  if ( asString(option("debug")) == "true" )
    m_bDebug = true;
  doPathChanged(wApp->internalPath());
  wApp->internalPathChanged().connect(SLOT(this, WsBreadCrumb::doPathChanged));
}

void WsBreadCrumb::setDebugging(bool bDebug)
{
  m_bDebug = bDebug;
}

void WsBreadCrumb::doPathChanged(std::string newPathOrig)
{
  std::string newPath = newPathOrig;
  boost::algorithm::replace_all(newPath, "&amp;",  "&");
  clear();
  m_vBreadCrumb.clear();
  WsUser*           pUser     = WsApp->wsUser();
  NodePtr   startNode = pUser->getAccessRoot();
  breadCrump        bc;
  bc.title                   = startNode.get()->getDisplayName(true);
  bc.path                    = "/";
  m_vBreadCrumb.push_back(bc);
  std::string sWithoutPrefix  = newPath; //WsApp->WsModules().pathWithoutPrefix(newPath);
  std::string sPath           = "/";
  if ( m_bDebug )
    wApp->log("notice") << "WsBreadCrumb::doPathChanged - path = " << newPath << " sWithoutPrefix = " << sWithoutPrefix << " bcp->path = " << bc.path;
  boost::filesystem::path myPath(sWithoutPrefix);
  if ( myPath.string() != sPath )
    while ( myPath.has_parent_path() ) {
      breadCrump  bc2;
      NodePtr curNode    = startNode.get()->eatPath(myPath.string());
      if ( curNode )  bc2.title = curNode.get()->getDisplayName(true);
      else           bc2.title = myPath.filename().string();
      bc2.path  = myPath.string();
      bc2.node  = curNode;
      m_vBreadCrumb.insert(m_vBreadCrumb.begin() + 1, bc2);
      myPath = myPath.parent_path();
      if ( myPath.string() == sPath ) break;
    }
  Wt::WStackedWidget*   contents = new Wt::WStackedWidget();
  m_pMenu  = new Wt::WMenu(contents, Wt::Horizontal);
  m_pMenu->itemSelected().connect(SLOT(this, WsBreadCrumb::onBreadCrumbSelected));
  addWidget(m_pMenu);
  for (int ibc = 0; ibc < m_vBreadCrumb.size(); ++ibc) {
    WMenuItem* pItem =  new WMenuItem(m_vBreadCrumb[ibc].title);
    //    pItem->setLink(WLink(WLink::InternalPath, m_vBreadCrumb[ibc].path));
    wApp->log("notice") << "WsBreadCrumb::doPathChanged - add link = " << m_vBreadCrumb[ibc].path;
    m_pMenu->addItem(pItem);
  }
}

void WsBreadCrumb::onBreadCrumbSelected(WMenuItem* pMenuItem)
{
  if ( m_bDebug )
    wApp->log("notice") << "WsBreadCrumb::onBreadCrumbSelected - index path = " << m_vBreadCrumb[m_pMenu->currentIndex()].path;
  if ( m_vBreadCrumb[m_pMenu->currentIndex()].path.empty() ) return;
  if ( m_bDebug )
    wApp->log("notice") << "WsBreadCrumb::onBreadCrumbSelected - index path not empty = " << m_vBreadCrumb[m_pMenu->currentIndex()].path;
  if (
    m_vBreadCrumb[m_pMenu->currentIndex()].node &&
    m_vBreadCrumb[m_pMenu->currentIndex()].node->isDirectory() &&
    asString(option("directorySelectable")) == "false"
  ) return;
  wApp->setInternalPath(m_vBreadCrumb[m_pMenu->currentIndex()].path, true);
}

