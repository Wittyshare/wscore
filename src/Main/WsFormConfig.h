/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsFormConfig_H_
#define _WsFormConfig_H_

#include <User/WsUser.h>
#include <ConfigManager/WsLayoutProperties.h>

#include <Wt/WContainerWidget>
#include <Wt/WDatePicker>
#include <Wt/WLineEdit>
#include <Wt/WCheckBox>
#include <Wt/WComboBox>

#include <Wt/WDate>
#include <Wt/WTime>
#include <Wt/WDateTime>

#include <gdwtcore/gdFHtmlEditor.h>

#include "WsModulesLoader.h"

class WsFormConfig : public Wt::WContainerWidget {
public :
  WsFormConfig(NodePtr pNode, WsModulesLoader& rMl, Wt::WContainerWidget* parent = 0);
  ~WsFormConfig();
  void                  setEditorFhtml(gdFHtmlEditor* pFhtmlEditor);

public slots :
  void               doCancel();
  void               doSave();

private :
  NodePtr    m_pNode;
  WsModulesLoader&   m_ml;
  std::vector<WsEditorWidget*> m_vEditorWidget;
  Wt::WLineEdit*     m_pName;
  Wt::WLineEdit*     m_pInitPage;
  Wt::WLineEdit*     m_pDisplayName;
  Wt::WLineEdit*     m_pAuthor;
  Wt::WDatePicker*   m_pDatePicker;
  gdFHtmlEditor*     m_pShortDescEditor;
  Wt::WTextArea*     m_pShortDescTextArea;
  Wt::WCheckBox*     m_pCBInMenu;
  Wt::WCheckBox*     m_pCBInView;
  Wt::WCheckBox*     m_pCBSort;
  gdFHtmlEditor*     m_pFhtmlEditor;
  Wt::WLineEdit*     m_pSortNumber;
  Wt::WCheckBox*     m_pCBInheritRights;
  Wt::WComboBox*     m_pCBBTemplates;

  // Initial values
  Wt::WDateTime      m_wDateTime;
  std::string        m_sName;
  std::string        m_sInitPage;
  std::string        m_sDisplayName;
  std::string        m_sAuthor;
  std::string        m_sTemplate;
  bool               m_binMenu;
  bool               m_binView;
  std::string        m_sShortDesc;
  bool               m_bSort;
  long               m_lSortNumber;
  bool               m_bInheritRights;
  bool               m_bUseTextArea;
};

#endif // ifdef _WsFormConfig_H_

