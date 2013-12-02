/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <boost/algorithm/string/replace.hpp>

#include <ConfigManager/WsLayoutProperties.h>
#include <User/WsUser.h>

#include <Wt/WTheme>

#include <gdcore/gdCore.h>

#include "Main/WsLogo.h"
#include "Main/WsSiteMap.h"
#include "Main/WsSearchObject.h"
#include "Main/WsTopBanner.h"
#include "Main/WsBottomBanner.h"
#include "Main/WsContentButtonsBar.h"
#include "Main/WsFunctionnalities.h"
#include "Main/WsContent.h"

#include "WsTemplate.h"

// http://www.johndcook.com/cpp_regex.html
//#include <regex>

using namespace Wt;

WsTemplate::WsTemplate(const std::string& templateFile, const std::string& initialPath, WContainerWidget* parent)
  : WTemplate(parent), m_sInitialPath(initialPath)
{
  wApp->log("notice") << "WsTemplate::WsTemplate - templateFile " << templateFile << " initialPath = " << initialPath;
  propagateSetEnabled(true);
  if ( WString::tr("byObjectStyleSheet").narrow() == "true" )
    wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/WsTemplate.css");
  addStyleClass("WsTemplate");
  std::string tFile = matchTemplate(templateFile);
  wApp->log("notice") << "WsTemplate::WsTemplate - loading " << tFile;
  std::string fileContent;
  if ( !gdcore_file_content2string(tFile.c_str(), fileContent) ) {
    wApp->log("ERROR") << "WsTemplate::WsTemplate - cannot load " << tFile;
    return;
  }
  setTemplateText(fileContent);
}

WsTemplate::~WsTemplate()
{ }

std::string WsTemplate::matchTemplate(const std::string& templateFile)
{
  WsUser*     pUser   = WsApp->wsUser();
  std::string curPath = wApp->internalPath();
  boost::algorithm::replace_all(curPath, "&amp;",  "&");
  std::string tFile   = pUser->getRootPath() + curPath;
  wApp->log("notice") << "WsTemplate::matchTemplate() tFile = " << tFile;
  boost::filesystem::path current(tFile);
  if ( !boost::filesystem::is_directory(current) )
    current = current.parent_path();
  std::string sCurrent;
  while (1) {
    sCurrent      = current.string() + GlobalConfig::PathToTemplates + templateFile;
    wApp->log("notice") << "WsTemplate::matchTemplate() sCurrent = " << sCurrent;
    if ( boost::filesystem::exists(sCurrent) )         return sCurrent;
    if ( pUser->getRootPath() == current.string() )    return templateFile;
    current = current.parent_path();
  }
}

WWidget* WsTemplate::resolveWidget(const std::string& varName)
{
  wApp->log("notice") << "WsTemplate::resolveWidget var = " << varName;
  // if ( fileContent.find("${WsTopBanner}") != string::npos ) {
  if ( varName == "WsLogo" ) {
    WsLogo* pLogo = new WsLogo();
    if (pLogo)
      bindWidget(varName, pLogo);
    return pLogo;
  }
  if ( varName == "WsSiteMap" ) {
    WsSiteMap* pSiteMap = new WsSiteMap();
    if (pSiteMap)
      bindWidget(varName, pSiteMap);
    return pSiteMap;
  }
  if ( varName == "WsSearchObject" ) {
    WsSearchObject* pSearch = new WsSearchObject();
    if (pSearch)
      bindWidget(varName, pSearch);
    return pSearch;
  }
  if ( varName == "WsTopBanner" ) {
    WsTopBanner* pTB = new WsTopBanner();
    if (pTB)
      bindWidget(varName, pTB);
    return pTB;
  }
  if ( varName == "WsContentButtonsBar" ) {
    WsContentButtonsBar* pCBB = new WsContentButtonsBar();
    if (pCBB)
      bindWidget(varName, pCBB);
    return pCBB;
  }
  if ( varName == "WsFunctionnalities" ) {
    WsFunctionnalities* pFunc = new WsFunctionnalities();
    if (pFunc)
      bindWidget(varName, pFunc);
    return pFunc;
  }
  if ( varName == "WsBottomBanner" ) {
    WsBottomBanner* pBB = new WsBottomBanner(WsLayoutProperties::instance()->get("global", "bottombanner_text", ""));
    if (pBB)
      bindWidget(varName, pBB);
    return pBB;
  }
  return WTemplate::resolveWidget(varName);
}

void WsTemplate::resolveString(const std::string& varName, const std::vector< WString >& args, std::ostream& result)
{
  wApp->log("notice") << "WsTemplate::resolveString var = " << varName;
  // TODO : remove  when Koen adapt resolveWidget with a list of arguments.
  // WsContent does exist only one time
  if ( varName == "WsContent" ) {
    WsContent* pContent = new WsContent();
    WsApp->setContent(pContent);
    std::string overflow;
    std::string sHomePage;
    bool        bUseLayout = false;
    for (int count = 0; count < args.size(); ++count) {
      std::string attr;
      std::string val;
      gdcore_string_split(args[count].toUTF8(), attr, val, '=');
      wApp->log("notice") << "WsTemplate::resolveString args " << count << " attr = " << attr << " val " << val;
      if ( attr == "useLayout" && val == "true" )
        bUseLayout = true;
      if ( attr == "homePage" )
        sHomePage = val;
      if ( attr == "overflow" )
        overflow = val;
    }
    //if ( bUseLayout ) { }
    if ( overflow == "auto" )
      pContent->setOverflow(WContainerWidget::OverflowAuto);
    if ( m_sInitialPath.size() > 0 ) sHomePage = m_sInitialPath;
    if (pContent)
      bindWidget(varName, pContent);
    if ( sHomePage.size() > 0 )
      pContent->setPath(sHomePage);
    return WTemplate::resolveString(varName, args, result);
  }
  //if ( varName == "WsTemplate" ) {
  std::string moduleName = "WsTemplate";
  if ( varName.compare(0, moduleName.size(), moduleName) == 0 ) {
    std::string templateFile;
    std::string templateClass;
    for (int count = 0; count < args.size(); ++count) {
      std::string attr;
      std::string val;
      gdcore_string_split(args[count].toUTF8(), attr, val, '=');
      wApp->log("notice") << "WsTemplate::resolveString creating new WsTemplate with args " << count << " attr = " << attr << " val " << val;
      if ( attr == "templateFile" )
        templateFile = val;
      if ( attr == "templateClass" )
        templateClass = val;
    }
    if ( templateFile.size() > 0 ) {
      wApp->log("notice") << "WsTemplate::resolveString creating new WsTemplate with templateFile = " << templateFile;
      WsTemplate* pTemplate = new WsTemplate(templateFile, m_sInitialPath);
      if ( templateClass.size() > 0 )
        pTemplate->addStyleClass(templateClass);
      if (pTemplate)
        bindWidget(varName, pTemplate);
    }
    return WTemplate::resolveString(varName, args, result);
  }
  WsUser*             pUser                   = WsApp->wsUser();
  std::string         m_sDocumentRoot         = pUser->getRootPath(); // /var/www/demo_site
  std::string         m_httpDocumentRoot      = gdWApp->getParameter("DOCUMENT_ROOT", "/var/www");
  std::string         m_sRelativeDocumentRoot = m_sDocumentRoot;
  boost::algorithm::replace_first(m_sRelativeDocumentRoot, m_httpDocumentRoot, "");
  std::string         sysPath(m_sDocumentRoot + wApp->internalPath());
  for (int iModule = 0; iModule < WsApp->WsModules().modules().size(); ++iModule) {
    WsModule* curModule = WsApp->WsModules().modules()[iModule]->module;
    //  if WsModMenu || WsModMenuImages crash, not possible to have 2 objects identical.
    //  I add a _ in place of by example WsModAnchor_Disclaimer and locate this _ after I split the string and compare the module name
    std::string right;
    gdcore_string_split(varName, moduleName, right, '_');
    //    wApp->log("notice") << "WsTemplate::resolveString compare module name " << curModule->moduleName() << " varName = " << varName << " with splited varname " << moduleName;
    if ( curModule->moduleName() != moduleName ) continue;
    wApp->log("notice") << "WsTemplate::resolveString creating module " << curModule->moduleName();
    // TODO : Verifier si c'est compatible dans tous les cas de figures
    curModule->setSysPath(sysPath);
    curModule->setDiffPath(m_sRelativeDocumentRoot);
    WWidget* w = curModule->createContents();
    for (int count = 0; count < args.size(); ++count) {
      std::string attr;
      std::string val;
      gdcore_string_split(args[count].toUTF8(), attr, val, '=');
      wApp->log("notice") << "WsTemplate::resolveString args and setting widget option " << count << " attr = " << attr << " val " << val;
      WsOptions* wOpt = dynamic_cast<WsOptions*>(w);
      wOpt->setOption(attr, val);
    }
    if (w)
      bindWidget(varName, w);
    break;
  }
  return WTemplate::resolveString(varName, args, result);
}
