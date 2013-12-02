/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsContentButtonsBar_H_
#define _WsContentButtonsBar_H_

#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WMessageBox>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

#include <gdwtcore/gdToolbar.h>

class WsContentButtonsBar : public gdToolbar {
public :
  WsContentButtonsBar(Wt::WContainerWidget* parent = 0);
  ~WsContentButtonsBar();

public slots :
  void                  doMenuEditPage(gdToolbarItem* pTbItem, Wt::WMouseEvent ev);
  void                  doFolderNew(gdToolbarItem* pTbItem, Wt::WMouseEvent ev);
  void                  doFileNew(gdToolbarItem* pTbItem, Wt::WMouseEvent ev);
  void                  doDeleteNode(gdToolbarItem* pTbItem, Wt::WMouseEvent ev);
  void                  doMBoxRespons(Wt::WMessageBox* pBox);
  void                  doFileUpload(gdToolbarItem* pTbItem, Wt::WMouseEvent ev);
  void                  doFolderNameChanged();
  void                  doFileNameChanged();
  void                  doFolderEnterPressed();
  void                  doFileEnterPressed();
  void                  doEndFolderNew();
  void                  doEndFileNew();

private :
  Wt::WDialog*          m_pDialog;
  Wt::WLineEdit*        m_pLE_Name;
  Wt::WPushButton*      m_pButOk;
  bool                  m_bDebug;
};

#endif // ifdef _WsContentButtonsBar_H_

