/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>

#include <Wt/WLogger>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/WTheme>

#include "WsApplication.h"
#include "Main/WsFunctionnalities.h"

#include <gdcore/gdCore.h>

#include <gdwtcore/gdWLogon.h>

#include "Main/WsTemplate.h"
#include "Main/WsTopBanner.h"
#include "Main/WsBottomBanner.h"
#include "Main/WsFunctionnalities.h"
#include <ConfigManager/WsLayoutProperties.h>

using namespace Wt;

WsApplication::WsApplication(const WEnvironment& env)
  : gdApplication(env), m_pUser(0), m_pButtonsBar(0), m_pContent(0), m_pImages(0)
{
  std::string sUid = gdWApp->getUserID();
  std::string sPwd;
  wApp->log("notice") << "WsApplication::WsApplication - Start load user with uid : " << sUid;
  std::string cip = environment().clientAddress();
  wApp->log("notice") << "WsApplication::WsApplication - Start load user with client IP : " << cip << " env ip " << environment().clientAddress();
  // TODO : Provisoire, si on est sur le reseau local on authentifie via ldap, sinon on considère que on est guest
  bool isPublicSite = WsGlobalProperties::instance()->get("global", "public_site", "false") == "true" ? true : false;
  if ( isPublicSite ) {
    bool enableLoginWindow = WsGlobalProperties::instance()->get("global", "login_window", "true") == "true" ? true : false;
    if (enableLoginWindow) {
      if ( cip.compare(0, 8, "192.168.") == 0 || cip.compare(0, 9, "127.0.0.1") == 0 ) {
        gdWLogon* logOn = new gdWLogon(sUid, sPwd, root());
        logOn->logonValidated().connect(SLOT(this, WsApplication::doEndDialogLogon));
        logOn->load();
      } else
        doEndDialogLogon("guest", "");
    } else
      doEndDialogLogon("guest", "");
  } else // TODO : Improve the solution
    doEndDialogLogon("", "");
}

void WsApplication::doEndDialogLogon(std::string sUid, std::string pPassword)
{
  //  // TODO : Remove the 2 next lines
  //  sUid = "angelique";
  wApp->log("notice") << "WsApplication::doEndDialogLogon() - logon : " << sUid;
  m_pUser = new WsUser(sUid, pPassword, environment().clientAddress());
  if ( !m_pUser || m_pUser->load() == FAILURE ) {
    wApp->log("ERROR") << "WsApplication::doEndDialogLogon - Cannot load user with uid : " << sUid;
    if ( m_pUser )
      delete m_pUser;
    root()->clear();
    root()->addWidget(new WText("Forbidden !"));
    m_pUser = 0;
    wApp->quit();
    // TODO : page d'erreur
    return;
  }
  googleAnalyticsLogger("/");
  wApp->log("notice") << "WsApplication::doEndDialogLogon - End load user with uid : " << sUid;
  root()->addStyleClass("wsMainWin");
  // On recherche le theme par dans la configuration, si rien n'est configuré on prend le theme polised par defaut.
  wApp->setCssTheme(WsLayoutProperties::instance()->get("global", "css_theme", "polished"));
  wApp->useStyleSheet(wApp->theme()->resourcesUrl() + "wittyshare/Css/wittyshare.css");
  wApp->log("notice") << "WsApplication::doEndDialogLogon - themes : " << wApp->theme()->resourcesUrl();
  // Chargement du fichier de resources .xml
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->resourcesUrl() + "wittyshare/Transl/wittyshare");
  // Chargement du fichier de configuration .xml
  wApp->messageResourceBundle().use(wApp->docRoot() + wApp->theme()->resourcesUrl() + "wittyshare/Conf/wittyshare_config");
  wApp->setTitle(WsLayoutProperties::instance()->get("global", "win_title", ""));
  WApplication::instance()->setLoadingIndicator(new WOverlayLoadingIndicator());
  m_ml.load(); // Load all modules
  std::string    oldInternalPath = wApp->internalPath();
  boost::algorithm::replace_all(oldInternalPath, "&amp;",  "&");
  wApp->log("notice") <<  "WsApplication::doEndDialogLogon() internalPath = "  << oldInternalPath;
  oldInternalPath = m_ml.checkPath(oldInternalPath);
  wApp->log("notice") <<  "WsApplication::doEndDialogLogon() internalPath after module check path= "  << oldInternalPath;
  std::string truePath(WsModules().pathWithoutPrefix(oldInternalPath));
  wApp->log("notice") <<  "WsApplication::doEndDialogLogon() internalPath without prefix = "  << truePath;
  if ( oldInternalPath != "/" && oldInternalPath.size() > 0 ) {
    // TODO : Boucler par module
    wApp->log("notice") <<  "WsApplication::doEndDialogLogon() checking perms in "  << oldInternalPath;
    int perms = m_pUser->getPermissions(truePath);
    if ( perms == GlobalConfig::NotLogged ) {
      wApp->log("ALERT") <<  "WsApplication::doEndDialogLogon() USer is not logged " ;
      wApp->redirect("/");
    }
    if ( perms != GlobalConfig::Read && perms != GlobalConfig::ReadWrite)  {
      wApp->log("ALERT") <<  "WsApplication::doEndDialogLogon() URL not allowed !!! "  << truePath << " with oldInternalPath = " << oldInternalPath << "perms = " << perms;
      wApp->redirect("/");
      // TODO : ERROR Page
      // Do not display error because urls from the old website then display an empty page with only Forbidden, and not link.
      //root()->clear();
      //root()->addWidget(new WText("<b>Forbidden !</b>"));
      return;
    }
  } else {
    oldInternalPath = ""; // reset sais + pq
    wApp->log("notice") <<  "WsApplication::doEndDialogLogon() reset internalPath new value = "  << oldInternalPath;
  }
  m_sHomePage = m_pUser->getHomePage();
  if ( WsLayoutProperties::instance()->get("global", "use_template", "true") == "true") {
    if ( oldInternalPath.size() > 1 ) {
      wApp->log("notice") << "WsApplication::doEndDialogLogon() - Using template : " << oldInternalPath;
      setTemplate(oldInternalPath);
    } else {
      wApp->log("notice") << "WsApplication::doEndDialogLogon() - Using template : " <<  m_sHomePage;
      setTemplate(m_sHomePage);
    }
  } else {
    WVBoxLayout* vbox = new WVBoxLayout();
    vbox->setContentsMargins(0, 0, 0, 0);
    WsContent* pContent = new WsContent();
    m_pContent = pContent;
    if ( oldInternalPath.size() > 0 )
      m_pContent->setPath(oldInternalPath);
    else
      m_pContent->setPath(m_sHomePage);
    wApp->log("notice") << "WsApplication::doEndDialogLogon - Not using template : " << m_sHomePage;
    if ( WsLayoutProperties::instance()->get("global", "use_topbanner", "true") == "true" ) {
      LOG(DEBUG) << "WsApplication :: Use top banner ";
      WsTopBanner* pTB = new WsTopBanner();
      vbox->addWidget(pTB, 0);
    }
    /*
         if ( WsLayoutProperties::instance()->get("global", "use_menubar", "true") == "true" )
           {
            LOG(DEBUG)<<"WsApplication :: Use menu bar";
            WsMenuBar* pBar = new WsMenuBar();
            vbox->addWidget(pBar, 0);
           }
    */
    if ( WsLayoutProperties::instance()->get("global", "use_fonctionnalities", "true") == "true" ) {
      LOG(DEBUG) << "WsApplication :: Use funcionnalities";
      vbox->addWidget(new WsFunctionnalities());
    }
    if (WsLayoutProperties::instance()->get("global", "use_ws_images", "false") == "true") {
      m_pImages = WsApp->WsModules().module("WsModImages2")->createContents();
      WHBoxLayout* hbox = new WHBoxLayout();
      hbox->addWidget(m_pImages, 0);
      hbox->addWidget(pContent, 1);
      vbox->addLayout(hbox, 1);
    } else
      vbox->addWidget(pContent, 1);
    if (WsLayoutProperties::instance()->get("global", "use_bottombanner", "true") == "true") {
      LOG(DEBUG) << "WsApplication :: Use bottom banner";
      vbox->addWidget(new WsBottomBanner(WsLayoutProperties::instance()->get("global", "bottombanner_text", "")), 0);
    }
    root()->setLayout(vbox);
  }
  wApp->internalPathChanged().connect(SLOT(this, WsApplication::doPathChanged));
  wApp->internalPathChanged().connect(SLOT(this, WsApplication::googleAnalyticsLogger));
  //root()->resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
}

WsApplication::~WsApplication()
{
  if ( m_pUser )
    delete m_pUser;
  m_pUser = 0;
}


void WsApplication::doPathChanged(std::string newPath)
{
  setTemplate(newPath);
}

void WsApplication::hideImages(bool bHide)
{
  if ( m_pImages ) m_pImages->setHidden(bHide);
}

WsUser* WsApplication::wsUser()
{
  return m_pUser;
}

WsModulesLoader& WsApplication::WsModules()
{
  return m_ml;
}

void WsApplication::setTemplate(const std::string& sPageOrig)
{
  std::string sPage = sPageOrig;
  boost::algorithm::replace_all(sPage, "&amp;",  "&");
  if ( WsLayoutProperties::instance()->get("global", "use_template", "true") == "true" ) {
    std::string     truePath(WsModules().pathWithoutPrefix(sPage));
    NodePtr rootNode = m_pUser->getAccessRoot();
    if ( !rootNode.get() ) {
      wApp->log("ALERT") <<  "WsApplication::setTemplate() root node is null !! ";
      // TODO : ERROR Page
      wApp->quit();
      return;
    }
    NodePtr curNode = rootNode.get()->eatPath(truePath);
    if ( !curNode.get() ) {
      boost::filesystem::path p(truePath);
      truePath = p.parent_path().string();
      curNode = rootNode.get()->eatPath(truePath);
    }
    // Boucle sur parent, jusqu'a obtention du template
    std::string sTemplate;
    std::string sPath;
    NodePtr pNode = curNode;
    while (pNode.get()) {
      sTemplate = pNode.get()->getProperties().get()->get("global", "template", "");
      if ( sTemplate.size() > 0 ) break;
      pNode = pNode.get()->getParent();
    }
    if ( sTemplate.size() < 1 ) sTemplate = "root.tpl"; // ce template doit exister
    if ( sPage.compare(0,  5, "/Edit" ) == 0 )    sTemplate = "stdEditPage.tpl"; // cui-ci aussi
    if ( sPage.compare(0,  8, "/SiteMap" ) == 0 ) sTemplate = "stdContact.tpl"; // cui-ci aussi
    if ( sPage.compare(0,  5, "/Logo" ) == 0 )    sTemplate = "GhFullWidth.tpl"; // cui-ci aussi
    if ( sPage.compare(0,  7, "/Search" ) == 0 )  sTemplate = "stdContact.tpl"; // cui-ci aussi
    if ( pNode.get() ) {
      wApp->log("notice") << "WsApplication::setTemplate() - Using template : " << sTemplate << " with home page : " << sPage;
      sPath = sPage;
    } else {
      wApp->log("notice") << "WsApplication::setTemplate() - Using template : " << sTemplate << ", with home page : " << m_sHomePage;
      sPath = m_sHomePage;
    }
    if ( sTemplate != m_sTemplate ) {
      m_sTemplate = sTemplate;
      root()->clear();
      m_pButtonsBar = 0;
      m_pContent = 0; // le template doit prevoir la creation du content
      m_pImages = 0;
      WsTemplate* pTemplate = new WsTemplate(sTemplate, sPath);
      bool bUseLayout = false; // TODO : Put this option in the layout.json
      if ( bUseLayout ) {
        WVBoxLayout* vbox = new WVBoxLayout();
        vbox->setContentsMargins(0, 0, 0, 0);
        vbox->addWidget(pTemplate, 1);
        root()->setLayout(vbox);
      } else {
        root()->addWidget(pTemplate);
        //           root()->resize(WLength(100, WLength::Percentage), WLength(100, WLength::Percentage));
      }
      return;
    }
  }
  if ( m_pContent )
    m_pContent->setPath(sPage);
}

void WsApplication::setContent(WsContent* pContent)
{
  //FIXME double free here
  //if ( m_pContent )
  //  delete m_pContent;
  m_pContent = pContent;
  m_pContent->setMaximumSize(WLength::Auto, WLength(10000));
}

void WsApplication::setContentButtonsBar(WsContentButtonsBar* pButtonsBar)
{
  m_pButtonsBar = pButtonsBar;
}

WsContentButtonsBar* WsApplication::contentButtonsBar()
{
  return m_pButtonsBar;
}

const std::string& WsApplication::homePage()
{
  return m_sHomePage;
}

void WsApplication::googleAnalyticsLogger(std::string newPath)
{
    LOG(DEBUG)<<"WsApplication::googleAnalyticsLogger() : called ";
    std::string googleCmd = "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,'script','//www.google-analytics.com/analytics.js','ga');ga('create', 'UA-46314760-1', 'eurofer.eu');ga('send', 'pageview');";
doJavaScript(googleCmd);
}
