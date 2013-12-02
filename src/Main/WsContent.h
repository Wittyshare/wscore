/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsContent_H_
#define WS_CONTENT_H__
#define _WsContent_H_

#include <Wt/WContainerWidget>

#include <gdwtcore/gdWFileView.h>
#include <gdwtcore/gdWForm.h>

#include "WsSiteMapView.h"

#include <ConfigManager/WsLayoutProperties.h>

class WsContent : public Wt::WContainerWidget {
public :
  WsContent(Wt::WContainerWidget* parent = 0);
  ~WsContent();
  virtual void     clear();
  void             load();
  void             buildSiteMap();
  void             siteMapChanged(std::string newPath);
  void             buildFileUpload(const std::string& path);
  void             setPath(std::string newPath);
  void             viewDirectory(const std::string& path);

public slots :
  void             doEditPage(std::string path);
  void             doSearch(Wt::WString sSearch);
  void             selectWidget(std::string path);
  void             doSiteMapItemSelected(gdWFileView::signalType sigType, std::string selectedPath);

private :
  std::string      m_searchPath;
  bool             m_bLogContent;
  bool             m_bAllowHtmlRendering;
  WsSiteMapView*   m_pSiteMapView;
  std::string      m_sDocumentRoot;
  std::string      m_httpDocumentRoot;
  std::string      m_sRelativeDocumentRoot;
  gdWForm*         m_curWForm;
};

#endif // ifdef _WsContent_H_

