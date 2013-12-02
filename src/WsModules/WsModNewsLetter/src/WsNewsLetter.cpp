/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <stdio.h>

#include <iostream>

#include <boost/filesystem.hpp>

#include <vmime/vmime.hpp>
#include <vmime/platforms/posix/posixHandler.hpp>

#include <Wt/WLogger>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WRegExpValidator>
#include <Wt/WValidator>
#include <Wt/WDate>
#include <Wt/WTime>
#include <Wt/WRandom>
#include <Wt/Dbo/WtSqlTraits>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/backend/Sqlite3>

#include <Wt/Auth/HashFunction>

#include <gdcore/gdCore.h>
#include <gdcore/gdLdap.h>

#include <Logger/WsLogger.h>
#include <ConfigManager/WsLayoutProperties.h>

#include <Main/WsApplication.h>

#include "EmailToken.h"
#include "WsNewsLetter.h"

// Class WsNewsLetter
// ==================
WsNewsLetter::WsNewsLetter(WContainerWidget* parent)
  : WContainerWidget(parent), m_pLineEdit(0), m_pButSubscribe(0), sqlite3_(0)
{
  if ( asString(option("debug")) == "true" )
    LOG(DEBUG) << "WsNewsLetter::WsNewsLetter() !";
  addStyleClass("WsNewsLetter");
}

WsNewsLetter::~WsNewsLetter()
{
  if ( asString(option("debug")) == "true" )
    LOG(DEBUG) << "WsNewsLetter::~WsNewsLetter() !";
  if ( sqlite3_ )
    delete sqlite3_;
  sqlite3_ = 0;
}

void WsNewsLetter::load()
{
  WContainerWidget::load();
  if ( asString(option("debug")) == "true" )
    WApplication::instance()->log("notice") << "WsNewsLetter::load()";
  initDB();
  WText* pTitle = new WText(asString(option("title")).toUTF8());
  pTitle->addStyleClass("WsTitle");
  addWidget(pTitle);
  WText* pSubTitle = new WText(asString(option("subTitle")).toUTF8());
  pSubTitle->addStyleClass("WsSubTitle");
  addWidget(pSubTitle);
  WRegExpValidator* validator = new WRegExpValidator("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
  m_pLineEdit = new WLineEdit();
  m_pLineEdit->addStyleClass("WsEmailAdr");
  m_pLineEdit->setValidator(validator);
  m_pLineEdit->keyWentUp().connect(SLOT(this, WsNewsLetter::doLineEditChanged));
  m_pLineEdit->enterPressed().connect(SLOT(this, WsNewsLetter::doSubscribe));
  addWidget(m_pLineEdit);
  WPushButton* m_pButSubscribe = new WPushButton("Subscribe");
  m_pButSubscribe->addStyleClass("WsBtnSubscribe");
  m_pButSubscribe->clicked().connect(SLOT(this, WsNewsLetter::doSubscribe));
  addWidget(m_pButSubscribe);
  if ( asString(option("useReadAll")) == "true" ) {
    std::string link = asString(option("link")).toUTF8();
    std::string text = asString(option("text")).toUTF8();
    WAnchor* pAnchor = new WAnchor(WLink(WLink::InternalPath, link), text);
    pAnchor->addStyleClass("WsReadAll");
    addWidget(pAnchor);
  }
}

void WsNewsLetter::initDB()
{
  //Init DB
  // Implemantation from EMWEB
  std::string dbPath = asString(option("dbNewsLetter")).toUTF8();
  wApp->log("notice") << "WsNewsLetter::initDB() sqlite3 backend db = " << dbPath;
  sqlite3_ =  new dbo::backend::Sqlite3(dbPath);
  sqlite3_->setProperty("show-queries", "true");
  session_.setConnection(*sqlite3_);
  session_.mapClass<EmailToken>("user");
  try {
    session_.createTables(); //Use it to create the DB.
    wApp->log("notice") << "WsNewsLetter::initDB() sqlite3 createTables and database";
  } catch (std::exception& e) {
    wApp->log("notice") << "WsNewsLetter::initDB() sqlite3 createTables and database : using existing database error = " << e.what();
  }
}

void WsNewsLetter::doLineEditChanged()
{
  if ( m_pButSubscribe  && m_pLineEdit )
    m_pButSubscribe->setDisabled(m_pLineEdit->validate() != Wt::WValidator::Valid);
}

void WsNewsLetter::doSubscribe()
{
  if ( !m_pLineEdit->validate() ) return;
  if ( m_pLineEdit->text().toUTF8().size() < 1 ) return;
  isEmailExistInLdap(m_pLineEdit->text().toUTF8());
}

WsNewsLetter::emailAvailability WsNewsLetter::isEmailExistInLdap(const std::string& email)
{
  gdCLdapServer       m_cLdapServer;
  m_cLdapServer.strServer        = asString(option("ldapServer")).narrow();
  m_cLdapServer.strDisplayName   = asString(option("ldapName")).toUTF8();
  m_cLdapServer.strLogon         = asString(option("ldapLogon")).toUTF8();
  m_cLdapServer.strPassword      = asString(option("ldapSecret")).toUTF8();
  if ( m_cLdapServer.TryServer() )
    WApplication::instance()->log("notice") << " WsNewsLetter::isEmailExistInLdap() Ldap try ok !";
  else {
    std::string error = "WsNewsLetter::isEmailExistInLdap() Ldap try not ok !";
    WApplication::instance()->log("notice") << error;
    wApp->setInternalPath("/ws/Errors/ServerTempUnavailable.fhtml", true);
    return ldapError;
  }
  gdCLdapAttributes          cWitchAttrs;
  cWitchAttrs.push_back(new gdCLdapAttribute("givenName"));
  cWitchAttrs.push_back(new gdCLdapAttribute("sn"));
  cWitchAttrs.push_back(new gdCLdapAttribute("mail"));
  cWitchAttrs.push_back(new gdCLdapAttribute("dn"));
  gdCLdap             m_cLdap;
  m_cLdap.setDebugging(true);
  m_cLdap.setManageDSAIT(true);
  gdCLdapFilter       cFilter;
  cFilter.pServer = &m_cLdapServer;
  cFilter.strInitialSearchBase = asString(option("ldapBase")).toUTF8();
  cFilter.nScope               = LDAP_SCOPE_SUB;
  cFilter.strFilter            = "(&(mail=" + email + "))";
  gdCLdapEntries             cEntries;
  if ( !m_cLdap.GetInfo(cFilter, cWitchAttrs, cEntries) && m_cLdap.GetLastError() != CLdapErrNoEntries ) {
    std::string notice = "WsNewsLetter::isEmailExistInLdap() cannot get ldap info or no entries!";
    WApplication::instance()->log("notice") << notice;
    wApp->setInternalPath("/ws/Errors/ServerTempUnavailable.fhtml", true);
    return ldapError;
  }
  // Email not available
  if ( cEntries.size() == 0 ) {
    emailNotInLdap(email);
    return emailNotAvailable;
  }
  // Multiple email entries
  if ( cEntries.size() >  1 ) {
    multipleEmail(email);
    return emailMultiple;
  }
  std::string  sGivenName;
  std::string  sSN;
  std::string  sEmail;
  std::string  sDn;
  for (int nAttr = 0; nAttr < cEntries[0]->Attrs.size(); ++nAttr) {
    if ( cEntries[0]->Attrs[nAttr]->Attr == "sn" )        sSN        = cEntries[0]->Attrs[nAttr]->Values[0]->ToStr();
    if ( cEntries[0]->Attrs[nAttr]->Attr == "givenName" ) sGivenName = cEntries[0]->Attrs[nAttr]->Values[0]->ToStr();
    if ( cEntries[0]->Attrs[nAttr]->Attr == "mail" )      sEmail     = cEntries[0]->Attrs[nAttr]->Values[0]->ToStr();
    if ( cEntries[0]->Attrs[nAttr]->Attr == "dn" )        sDn        = cEntries[0]->Attrs[nAttr]->Values[0]->ToStr();
  }
  std::string notice = "WsNewsLetter::isEmailExistInLdap() match givenName, " + sGivenName + " sn, " + sSN + " email, " + sEmail + " dn, " + sDn;
  WApplication::instance()->log("notice") << notice;
  gdCLdapEntry        entry;
  entry.Dn = sDn;
  gdCLdapAttribute   updateAttr("EurListsMember");
  updateAttr.bNewAttribute = true; // false destroy all existing values, true add a new one, in all case if the attribute does not exist it is added
  updateAttr.addValue("Newsletter03");
  // if the value exist : we receive error 312 "Type of value exists"
  if ( !m_cLdapServer.UpdateAttribute(entry, updateAttr) )
    wApp->log("notice") << "WsNewsLetter::isEmailExistInLdap() Cannot update ldap entry : " << sDn << "Contain a correct value";
  else
    wApp->log("notice") << "WsNewsLetter::isEmailExistInLdap() Ldap entry : " << sDn << " correctly updated !";
}

void WsNewsLetter::multipleEmail(const std::string& email)
{
  wApp->log("notice") << "WsNewsLetter::emailMultiple() email " << email;
  std::string sTo         = asString(option("NLEmailTo")).toUTF8();
  std::string sSign       = asString(option("NLSignature")).toUTF8();
  std::string sSubject    = "Muliple email detection when registering a NewsLetter";
  std::string sBody       = "Dear web administrator,<br /><br /> pending a check of the email <b>"
                            + email
                            + "</b> I detect a multiple entry on our LDAP database,<br />"
                            + " please check which correct action is required and process a manual respons to our client<br /><br />"
                            + "Thanks in advance.<br/><br/>"
                            + sSign;
  std::string sBodyPlain  = "Dear web administrator,\n\npending a check of the email *"
                            + email
                            + "* I detect a multiple entry on our LDAP database,\n"
                            + " please check which correct action is required and process a manual respons to our client\n\n"
                            + "Thanks in advance.\n\n"
                            + sSign;
  sendEmail(sTo, sSubject, sBody, sBodyPlain);
}

void WsNewsLetter::sendEmail(const std::string& email, const std::string& sSubject, const std::string& sBody, const std::string& sBodyPlain)
{
  std::string sSmtpServer = asString(option("smtpServer")).toUTF8();
  std::string sFrom       = asString(option("NLEmailFrom")).toUTF8();
  vmime::platformDependant::setHandler<vmime::platforms::posix::posixHandler>();
  try {
    vmime::messageBuilder mb;
    // Fill in the basic fields
    mb.setExpeditor(vmime::mailbox(sFrom));
    vmime::addressList to;
    to.appendAddress(vmime::create <vmime::mailbox>(email));
    mb.setRecipients(to);
    mb.setSubject(vmime::text(sSubject));
    // Message body
    mb.constructTextPart(vmime::mediaType(vmime::mediaTypes::TEXT, vmime::mediaTypes::TEXT_HTML));
    vmime::ref<vmime::htmlTextPart> textPart = mb.getTextPart().dynamicCast<vmime::htmlTextPart>();
    textPart->setCharset(vmime::charsets::UTF_8);
    textPart->setText(vmime::create<vmime::stringContentHandler>(sBody));
    textPart->setPlainText(vmime::create<vmime::stringContentHandler>(sBodyPlain));
    //    wApp->log("notice") << "WsNewsLetter::emailMultiple() plain text end smtpserver = " << sSmtpServer;
    vmime::ref<vmime::message> msg = mb.construct();
    vmime::ref<vmime::net::session> sess = vmime::create<vmime::net::session>();
    vmime::utility::url url(sSmtpServer);
    vmime::ref<vmime::net::transport> tr = sess->getTransport(url);
    tr->connect();
    tr->send(msg);
    tr->disconnect();
  }
  // VMime exception
  catch (vmime::exception& e) {
    std::cout << "vmime::exception: " << e.what() << std::endl;
    throw;
  }
  // Standard exception
  catch (std::exception& e) {
    std::cout << "std::exception: " << e.what() << std::endl;
    //throw;
  }
}

void WsNewsLetter::registerValidEmail(const std::string& email)
{
}

// Implemantation from EMWEB
void WsNewsLetter::emailNotInLdap(const std::string& email)
{
  std::string random = WRandom::generateId(16);
  Auth::SHA1HashFunction hf;
  std::string hash = hf.compute(random, std::string());
  bool bMatchEmail = false;
  {
    dbo::Transaction transaction(session_);
    wApp->log("notice") << "WsNewsLetter::emailNotInLdap() email = " << email << " find it !";
    EmailTokens eTokens = session_.find<EmailToken>().where("email = ?").bind(email);
    if ( eTokens.size() != 0 ) {
      std::string text = "Email is already in db, nothing to do !";
      new WText(text, this);
      wApp->log("notice") << "WsNewsLetter::emailNotInLdap() email exist in database " << email;
      bMatchEmail = true;
    } else {
      EmailToken* eToken = new EmailToken();
      eToken->email       = email;
      eToken->token       = hash;
      eToken->currentDate = WDate::currentDate();
      eToken->registered  = 0;
      session_.add(eToken);
      wApp->log("notice") << "WsNewsLetter::emailNotInLdap() email does not exist in database " << email << " token = " << random << " hash = " << hash;
    }
  }
  if ( bMatchEmail ) {
    wApp->setInternalPath("/ws/Errors/AlreadyInDb.fhtml", true);
    return;
  }
  std::string sSubject    = asString(option("NLSubject")).toUTF8();
  std::string sBody       = asString(option("NLBody")).toUTF8();
  std::string sBodyPlain  = asString(option("NLBodyPlain")).toUTF8();
  std::string sServerUrl  = WsGlobalProperties::instance()->get("global", "serverUrl", "http://localhost");
  std::string url = sServerUrl + "/Newsletter/Subscribe/" + random;
  wApp->log("notice") << "WsNewsLetter::emailNotInLdap() url created = " << url;
  sBody      += "<a href=\"" + url + "\">Register</a>";
  sBodyPlain += url;
  sendEmail(email, sSubject, sBody, sBodyPlain);
  wApp->setInternalPath("/ws/ThanksSubscription.fhtml", true);
}

std::string WsNewsLetter::checkPath(const std::string& path)
{
  wApp->log("notice") << "WsNewsLetter::checkPath() currentPath = " << path;
  if ( path.compare(0, 22, "/Newsletter/Subscribe/") != 0 ) return path;
  std::string token = path;
  boost::algorithm::replace_all(token, "/Newsletter/Subscribe/", "");
  wApp->log("notice") << "WsNewsLetter::checkPath() token is " << token;
  initDB();
  Auth::SHA1HashFunction hf;
  std::string hash = hf.compute(token, std::string());
  dbo::Transaction transaction(session_);
  EmailTokens eTokens = session_.find<EmailToken>().where("token = ?").bind(hash);
  std::string retPage;
  if ( eTokens.size() == 0 )
    retPage = "/ws/Errors/NotInDB.fhtml";
  else {
    EmailTokens::const_iterator i = eTokens.begin();
    if ( (*i)->registered == 0 ) {
      // TODO : How to set the email in the form ?
      // the form save the data.
      retPage = "/ws/Subscribe.form";
      i->modify()->registered++;
    } else {
      retPage = "/ws/Errors/AlreadyRegistered.fhtml";
      // TODO : Update value ?
    }
  }
  wApp->setInternalPath(retPage, true);
  return retPage;
}

