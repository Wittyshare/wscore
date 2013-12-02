/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <Wt/WApplication>
#include <Wt/WPanel>
#include <Wt/WTable>
#include <Wt/WText>
#include <Wt/WTheme>
#include <Wt/WIntValidator>
#include <Wt/WRegExpValidator>

#include <gdcore/gdCore.h>

#include "WsApplication.h"
#include "WsFormConfig.h"

using namespace Wt;

WsFormConfig::WsFormConfig(NodePtr pNode, WsModulesLoader& rMl, WContainerWidget* parent)
  : WContainerWidget(parent), m_pNode(pNode), m_ml(rMl), m_pAuthor(0), m_pShortDescEditor(0), m_pShortDescTextArea(0), m_pCBInMenu(0), m_pCBInView(0), m_pFhtmlEditor(0), m_pInitPage(0), m_bUseTextArea(true)
{
  //  if ( WsLayoutProperties::instance()->get("global", "by_object_stylesheet", "false") == "true" )
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsFormConfig.css");
  addStyleClass("WsFormConfig");
  /*
   * in properties
   * =============
       - name, for renaming a node
       - publish_date
       - author
       - short_desc
       - display_name
       - in_menu
       - in_view
       - sort
       - sort index
       - inherit_rights_from_parent
       -
  */
  WsUser* pUser = WsApp->wsUser();
  WTable* pTable = new WTable();
  int     iRow   = 0;
  // Les dates sont sockées en string format time_t TODO : add try/catch
  // Publish Date
  WDateTime                dt(WDateTime::currentDateTime());
  std::string sDateTime  = pNode.get()->getProperties().get()->get("global", "publish_date", boost::lexical_cast<std::string>(dt.toTime_t()));
  m_wDateTime.setTime_t(boost::lexical_cast<time_t>(sDateTime));
  Wt::WRegExpValidator* validatorName = new Wt::WRegExpValidator("[a-zA-Z0-9._& ]{2,48}");
  new WText("(new ?) name", pTable->elementAt(iRow, 0));
  m_sName                = pNode.get()->getName();
  m_pName                = new WLineEdit(WString::fromUTF8(m_sName), pTable->elementAt(iRow, 2));
  m_pName->setValidator(validatorName);
  m_pName->setTextSize(48);
  if ( pNode.get()->isDirectory() ) {
    new WText("Initial page", pTable->elementAt(iRow, 4));
    m_sInitPage            = pNode.get()->getProperties().get()->get("global", "initial_page", "");
    m_pInitPage            = new WLineEdit(WString::fromUTF8(m_sInitPage), pTable->elementAt(iRow, 6));
    m_pInitPage->setTextSize(32);
  }
  iRow++;
  new WText("Publish Date", pTable->elementAt(iRow, 0));
  m_pDatePicker =          new WDatePicker(pTable->elementAt(iRow, 2));
  m_pDatePicker->setFormat("yyyy-MM-dd");
  m_pDatePicker->setDate(m_wDateTime.date());
  // Display name : pNode->getDisplayName() return the pNode->getName if the display_name is empty.
  new WText("Display name", pTable->elementAt(iRow, 4));
  //  std::string ds         = pNode->getProperties().get()->get("global", "display_name",  pNode->getName());
  m_sDisplayName         = pNode.get()->getDisplayName(true);
  m_pDisplayName         = new WLineEdit(WString::fromUTF8(m_sDisplayName), pTable->elementAt(iRow, 6));
  m_pDisplayName->setTextSize(60);
  iRow++;
  // TODO : get all possible author ?
  std::string curUser    = pUser->getFirstName() + " " + pUser->getSurname();
  m_sAuthor              = pNode.get()->getProperties().get()->get("global", "author", curUser);
  new WText("Author", pTable->elementAt(iRow, 0));
  m_pAuthor              = new WLineEdit(WString::fromUTF8(m_sAuthor), pTable->elementAt(iRow, 2));
  m_pAuthor->setTextSize(30);
  std::string sSortNumber = pNode.get()->getProperties().get()->get("global", "sort_number", "-1");
  new WText("Sort Number", pTable->elementAt(iRow, 4));
  m_lSortNumber           = boost::lexical_cast<long>(sSortNumber);
  m_pSortNumber           = new WLineEdit(WString::fromUTF8(sSortNumber), pTable->elementAt(iRow, 6));
  m_pSortNumber->setTextSize(10);
  WIntValidator* validator = new WIntValidator(-1, 9999);
  validator->setMandatory(true);
  m_pSortNumber->setValidator(validator);
  iRow++;
  m_pCBInheritRights    = new WCheckBox("Inherit parent rights", pTable->elementAt(iRow, 0));
  std::string sInheritRights = pNode.get()->getProperties().get()->get("global", "inherit_rights_from_parent", "false");
  if ( sInheritRights == "true" )
    m_pCBInheritRights->setCheckState(Wt::Checked);
  m_bInheritRights = m_pCBInheritRights->checkState();
  m_pCBInMenu    = new WCheckBox("In menu", pTable->elementAt(iRow, 2));
  // TODO : Il faut activer ce qui apparait dans les menus ?
  std::string sInMenu = pNode.get()->getProperties().get()->get("global", "in_menu", "false");
  if ( sInMenu == "true" )
    m_pCBInMenu->setCheckState(Wt::Checked);
  m_binMenu = m_pCBInMenu->checkState();
  std::string sSort = pNode.get()->getProperties().get()->get("global", "sort", "false");
  m_pCBSort    = new WCheckBox("Sort", pTable->elementAt(iRow, 4));
  if ( sSort == "true" )
    m_pCBSort->setCheckState(Wt::Checked);
  m_bSort = m_pCBSort->checkState();
  m_pCBInView = new WCheckBox("In view", pTable->elementAt(iRow, 6));
  std::string sInView = pNode.get()->getProperties().get()->get("global", "in_view", "true");
  if ( sInView == "true" )
    m_pCBInView->setCheckState(Wt::Checked);
  m_binView = m_pCBInView->checkState();
  iRow++;
  new WText("Template", pTable->elementAt(iRow, 0));
  m_pCBBTemplates = new Wt::WComboBox(pTable->elementAt(iRow, 2));
  std::string sTemplate = pNode.get()->getProperties().get()->get("global", "template", "stdSubPage.tpl");
  // TODO : boucler avec les droits ? sur /.config/templates/*.tpl
  std::vector<std::string> vTemplates;
  vTemplates.push_back("root.tpl");
  vTemplates.push_back("stdSubPage.tpl");
  for ( int countTemplates = 0; countTemplates < vTemplates.size(); ++countTemplates) {
    m_pCBBTemplates->addItem(vTemplates[countTemplates]);
    if ( vTemplates[countTemplates] == sTemplate ) {
      m_pCBBTemplates->setCurrentIndex(countTemplates);
      m_sTemplate = vTemplates[countTemplates];
    }
  }
  iRow++;
  m_sShortDesc                    = pNode.get()->getProperties().get()->get("global", "short_description", "");
  if ( m_bUseTextArea ) {
    m_pShortDescTextArea              = new WTextArea(m_sShortDesc);
    m_pShortDescTextArea->resize(WLength(100, WLength::Percentage), WLength(300));
    //     m_pShortDescTextArea->setOverflow(WContainerWidget::OverflowAuto);
  } else {
    m_pShortDescEditor              = new gdFHtmlEditor(m_sShortDesc, false);
    m_pShortDescEditor->resize(WLength(100, WLength::Percentage), WLength(300));
    m_pShortDescEditor->setOverflow(WContainerWidget::OverflowAuto);
  }
  iRow++;
  WPushButton* pButCancel = new WPushButton("Cancel");
  pButCancel->clicked().connect(SLOT(this, WsFormConfig::doCancel));
  addWidget(pButCancel);
  WPushButton* pBut = new WPushButton("Save");
  pBut->clicked().connect(SLOT(this, WsFormConfig::doSave));
  addWidget(pBut);
  WContainerWidget* pCW = new WContainerWidget();
  pCW->addWidget(pTable);
  pCW->addWidget(new WText("<b>Short Description</b>"));
  if ( m_bUseTextArea )
    pCW->addWidget(m_pShortDescTextArea);
  else
    pCW->addWidget(m_pShortDescEditor);
  std::string title = "Properties of " + pNode.get()->getPath().string();
  WPanel* pPanel = new WPanel();
  pPanel->setTitle(title);
  pPanel->setCentralWidget(pCW);
  pPanel->setCollapsible(true);
  //  pPanel->setOverflow(Wt::WContainerWidget::OverflowAuto);
  addWidget(pPanel);
  pPanel->expand();
  //pCW->addWidget(new WText("Modules specifics options"));
  for (int iModule = 0; iModule < m_ml.modules().size(); iModule++) {
    WsModule* curModule = m_ml.modules()[iModule]->module;
    if ( !curModule->isLoaded() ) continue;
    WsEditorWidget* w = curModule->createEditor();
    if ( w ) {
      m_vEditorWidget.push_back(w);
      WPanel* pPanel2 = new WPanel();
      pPanel2->setTitle(curModule->moduleName());
      pPanel2->setCentralWidget(w);
      pPanel2->setCollapsible(true);
      pPanel2->expand();
      pCW->addWidget(pPanel2);
    }
  }
}

WsFormConfig::~WsFormConfig()
{ }

void WsFormConfig::setEditorFhtml(gdFHtmlEditor* pFhtmlEditor)
{
  m_pFhtmlEditor =  pFhtmlEditor;
}

void WsFormConfig::doCancel()
{
  wApp->setInternalPath(m_pNode->getPath().string(), true);
}

void WsFormConfig::doSave()
{
  if ( !m_pName->validate() ) return;
  if ( !m_pSortNumber->validate() ) return;
  bool     bUpdated = false;
  WDateTime   dt(m_pDatePicker->date(), WTime::currentServerTime());
  std::string sDateTime    = dt.toString("yyyy-MM-dd hh:mm:ss").narrow();
  wApp->log("notice") << "WsFormConfig::doSave() dateTime " << sDateTime << " initial value = " << m_wDateTime.toString("yyyy-MM-dd hh:mm:ss").narrow();
  std::string sName        = m_pName->text().toUTF8();
  if ( sName.size() < 2 ) return;
  std::string sInitPage    = "";
  if ( m_pInitPage )
    sInitPage    = m_pInitPage->text().toUTF8();
  std::string sDisplayName = m_pDisplayName->text().toUTF8();
  std::string sAuthor      = m_pAuthor->text().toUTF8();
  std::string sInMenu      = (m_pCBInMenu->checkState() == Wt::Checked ? "true" : "false");
  std::string sInView      = (m_pCBInView->checkState() == Wt::Checked ? "true" : "false");
  std::string sShortDesc;
  if ( m_bUseTextArea )
    sShortDesc   = m_pShortDescTextArea->text().toUTF8();
  else
    sShortDesc   = m_pShortDescEditor->text().toUTF8();
  std::string sSort        = (m_pCBSort->checkState() == Wt::Checked ? "true" : "false");
  std::string sSortNumber  = m_pSortNumber->text().narrow();
  std::string sInheritRights = (m_pCBInheritRights->checkState() == Wt::Checked ? "true" : "false");
  std::string sTemplate    = m_pCBBTemplates->currentText().toUTF8();
  gdcore_string_erase_right(sDisplayName, ' ');
  gdcore_string_erase_left(sDisplayName, ' ');
  gdcore_string_erase_right(sAuthor, ' ');
  gdcore_string_erase_left(sAuthor, ' ');
  for (int iModule = 0; iModule < m_vEditorWidget.size(); ++iModule) {
    m_vEditorWidget[iModule]->saveEditor();
  }
  WsUser* pUser = WsApp->wsUser();
  wApp->log("notice") << "WsFormConfig::doSave() date = " << sDateTime << " sInitPage " << sInitPage << " sName " << sName << " sTemplate " << sTemplate
                      << " DisplayName = " << sDisplayName << " author = " << sAuthor
                      << " in_menu = " << sInMenu << " in_view = " << sInView << " sort = " << sSort << " sort number = " << sSortNumber
                      << " inherit_rights_from_parent " << sInheritRights << " short_description = " << sShortDesc;
  if ( m_wDateTime != dt ) {
    m_pNode->getProperties().get()->set("global", "publish_date",      boost::lexical_cast<std::string>(dt.toTime_t()));
    bUpdated = true;
  }
  if ( m_pNode->isDirectory() ) {
    if ( m_sInitPage != sInitPage )
      m_pNode->getProperties().get()->set("global", "initial_page",         sInitPage);
    bUpdated = true;
  }
  if ( m_sDisplayName != sDisplayName ) {
    m_pNode->getProperties().get()->set("global", "display_name",      sDisplayName);
    bUpdated = true;
  }
  if ( m_sAuthor != sAuthor ) {
    m_pNode->getProperties().get()->set("global", "author",            sAuthor);
    bUpdated = true;
  }
  if ( m_binMenu != m_pCBInMenu->checkState() ) {
    m_pNode->getProperties().get()->set("global", "in_menu",           sInMenu);
    bUpdated = true;
  }
  if ( m_binView != m_pCBInView->checkState() ) {
    m_pNode->getProperties().get()->set("global", "in_view",           sInView);
    bUpdated = true;
  }
  if ( sTemplate != m_sTemplate ) {
    m_pNode->getProperties().get()->set("global", "template",          sTemplate);
    bUpdated = true;
  }
  if ( m_sShortDesc != sShortDesc ) {
    m_pNode->getProperties().get()->set("global", "short_description", sShortDesc);
    bUpdated = true;
  }
  if ( m_bSort != m_pCBSort->checkState() ) {
    m_pNode->getProperties().get()->set("global", "sort",              sSort);
    bUpdated = true;
  }
  if ( m_lSortNumber != boost::lexical_cast<long>(sSortNumber) ) {
    m_pNode->getProperties().get()->set("global", "sort_number",       sSortNumber);
    bUpdated = true;
  }
  if ( m_bInheritRights != m_pCBInheritRights->checkState() ) {
    m_pNode->getProperties().get()->set("global", "inherit_rights_from_parent",      sInheritRights);
    bUpdated = true;
  }
  if ( bUpdated )
    pUser->saveProperties(m_pNode->getProperties().get(), m_pNode->getPath().string());
  if ( m_pFhtmlEditor ) {
    wApp->log("notice") << "WsFormConfig::doSave() fhtml file = " << m_pNode->getFullPath(); // << " text = " <<  m_pFhtmlEditor->text().toUTF8();
    FILE* hData = fopen(m_pNode->getFullPath().c_str(), "w");
    if ( hData ) {
      fprintf(hData, "%s", m_pFhtmlEditor->text().toUTF8().c_str());
      fclose(hData);
    } else
      wApp->log("notice") << "WsFormConfig::doSave() cannot open " << m_pNode->getFullPath();
  }
  std::string oldName = m_pNode->getPath().string();
  std::string newName = m_pNode->getPath().parent_path().string();
  if ( newName != "/" ) newName += "/";
  newName += sName;
  wApp->log("notice") << "WsFormConfig::doSave() rename oldName = " << oldName << " newName " << newName;
  if ( m_sName != sName )
    pUser->renameNode(oldName, newName);
  //  wApp->setInternalPath(m_pNode->getPath().string(), true);
  wApp->setInternalPath(newName, true);
}

