/**
 *
 *       @file  WsLdapAuth.cpp
 *
 *      @brief  Ldap connector class
 *
 *    @version  1.0
 *       @date  07/15/11 09:25:47
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsLdapAuth.h"
#include <Include/WsGlobalConfig.h>
#include <boost/tokenizer.hpp>
#include <stdlib.h>
using namespace std;

extern "C" {
  void WsLdapAuthInit(void)
  {
    LOG(DEBUG) << "WsLdapAuth::WsLdapAuthInit() : Preparing required libraries : libgdcore ";
    void* hndl = dlopen("libgdcore.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL )
      LOG(ERROR) << "WsLdapAuth::WsLdapAuthInit() : Cannot load libgdcore.so shared library! " << dlerror();
  }
}


WsLdapAuth::WsLdapAuth()
{
  m_ldapStatus = notLoaded;
}

int WsLdapAuth::authentify(const string& uid, const string& pass, const std::string& ip)
{
  m_uid = uid;
  m_ip = ip;
  bool isPublicSite = WsGlobalProperties::instance()->get("global", "public_site", "false") == "true" ? true : false;
  if (isPublicSite) {
    string guest = WsGlobalProperties::instance()->get("global", "guest_group", "guest");
    if (!ipValid(ip) || uid == guest) {
      LOG(INFO) << "WsLdapAuth::authentify() : IP not valid " << ip << " For user " << uid;
      /* Generate unique id for the guest */
      int ran = rand() % 10000000 + 1;
      m_uid = m_uid + "-" + boost::lexical_cast<string>(ran);
      m_groups.insert(guest);
      return ErrorCode::Success;
    }
  }
  m_ldapStatus = notLoaded;
  m_properties = WsGlobalProperties::instance();
  gdCLdapServer cLdapServer;
  cLdapServer.strServer      =  m_properties->get("ldap", "server", "");
  cLdapServer.strDisplayName =  m_properties->get("ldap", "server_name", "");
  /* Common settings for groups and uid */
  gdCLdap             cLdap;
  gdCLdapAttributes   cWitchAttrs;
  gdCLdapFilter       cFilter;
  gdCLdapEntries      cEntries;
  cFilter.pServer = &cLdapServer;
  cFilter.nScope        = LDAP_SCOPE_SUB;
  /* Uid specific settings  */
  cFilter.strInitialSearchBase = m_properties->get("ldap", "search_base_uid", "");
  LOG(DEBUG) << "WsLdapAuth :: getting filter for uid " << m_uid;
  cFilter.strFilter     = m_properties->getAndReplace("ldap", "filter_uid", m_uid, "");
  LOG(DEBUG) << "WsLdapAuth :: Filter for uid " << cFilter.strFilter;
  cWitchAttrs.push_back(new gdCLdapAttribute(m_properties->get("ldap", "sn_key", "")));
  cWitchAttrs.push_back(new gdCLdapAttribute(m_properties->get("ldap", "gn_key", "")));
  cWitchAttrs.push_back(new gdCLdapAttribute(m_properties->get("ldap", "mail_key", "")));
  cWitchAttrs.push_back(new gdCLdapAttribute("dn"));
  if ( !cLdap.GetInfo(cFilter, cWitchAttrs, cEntries) ) {
    LOG(ERROR) << "WsLdapAuth::authentify() : Could not query ldap !" << endl;
    m_ldapStatus = onError;
    /* Free memory */
    for (int i = 0; i < cWitchAttrs.size(); ++i) {
      delete cWitchAttrs[i];
    }
    return ErrorCode::Failure;
  }
  if ( cEntries.size() != 1 ) {
    LOG(ERROR) << "WsLdapAuth::authentify() : Null or too much entries when querying LDAP !" << endl;
    m_ldapStatus = onError;
    for (int i = 0; i < cWitchAttrs.size(); ++i) {
      delete cWitchAttrs[i];
    }
    return ErrorCode::Failure;
  }
  /* Retrive uid related data */
  for (int nEntry = 0; nEntry < cEntries.size(); nEntry++) {
    for (int nAttr = 0; nAttr < cEntries[nEntry]->Attrs.size(); nAttr++) {
      if ( cEntries[nEntry]->Attrs[nAttr]->Attr == m_properties->get("ldap", "sn_key", "")) {
        m_surname = cEntries[nEntry]->Attrs[nAttr]->Values[0]->ToStr();
        continue;
      }
      if ( cEntries[nEntry]->Attrs[nAttr]->Attr == m_properties->get("ldap", "gn_key", "")) {
        m_name = cEntries[nEntry]->Attrs[nAttr]->Values[0]->ToStr();
        continue;
      }
      if ( cEntries[nEntry]->Attrs[nAttr]->Attr == m_properties->get("ldap", "mail_key", "")) {
        m_email = cEntries[nEntry]->Attrs[nAttr]->Values[0]->ToStr();
        continue;
      }
      if ( cEntries[nEntry]->Attrs[nAttr]->Attr == "dn" ) {
        m_dn = cEntries[nEntry]->Attrs[nAttr]->Values[0]->ToStr();
        continue;
      }
    }
  }
  // Authenticate in ldap db : require the DN + password. The DN is retrieved by the previous query based on the uid passed as argument.
  if ( isPublicSite && uid.size() > 0 ) {
    cLdapServer.DisConnect();
    cLdapServer.strLogon    = m_dn;
    cLdapServer.strPassword = pass;
    if ( !cLdapServer.Connect() ) {
      LOG(DEBUG) << "WsLdapAuth::authentify() : Cannot connect with uid " << m_uid;
      m_ldapStatus = onError;
      return ErrorCode::Failure;
    } else
      LOG(DEBUG) << "WsLdapAuth :: Connected with dn " << m_dn;
  }
  /* Groups specific settings */
  cFilter.strInitialSearchBase = m_properties->get("ldap", "search_base_groups", "");
  cFilter.strFilter     = m_properties->getAndReplace("ldap", "filter_groups", m_uid, "");
  LOG(DEBUG) << "WsLdapAuth::authentify() : Filter for groups " << cFilter.strFilter;
  gdCLdapAttributes   cWitchAttrs2;
  gdCLdapEntries      cEntries2;
  cWitchAttrs2.push_back(new gdCLdapAttribute(m_properties->get("ldap", "groups_key", "")));
  cLdap.setManageDSAIT(true);
  if ( !cLdap.GetInfo(cFilter, cWitchAttrs2, cEntries2) ) {
    LOG(ERROR) << "WsLdapAuth::authentify() : Could not query ldap !";
    m_ldapStatus = onError;
    for (int i = 0 ; i < cWitchAttrs2.size(); ++i) {
      delete cWitchAttrs[i];
    }
    return ErrorCode::Failure;
  }
  LOG(DEBUG) << "WsLdapAuth::authentify() : groups entries size : " << cEntries2.size();
  for (int nEntry = 0; nEntry < cEntries2.size(); nEntry++) {
    for (int nAttr = 0; nAttr < cEntries2[nEntry]->Attrs.size(); nAttr++) {
      if ( cEntries2[nEntry]->Attrs[nAttr]->Attr == m_properties->get("ldap", "groups_key", "")) {
        for (int nVal = 0; nVal < cEntries2[nEntry]->Attrs[nAttr]->Values.size(); nVal++) {
          m_groups.insert(cEntries2[nEntry]->Attrs[nAttr]->Values[nVal]->ToStr().c_str());
        }
        continue;
      }
    }
  }
  for (int i = 0 ; i < cWitchAttrs2.size(); ++i) {
    delete cWitchAttrs[i];
  }
  m_ldapStatus = loaded;
  return ErrorCode::Success;
}

string WsLdapAuth::getUid()
{
  return m_uid;
}

string WsLdapAuth::getFirstName()
{
  return m_name;
}

string WsLdapAuth::getSurname()
{
  return m_surname;
}

string WsLdapAuth::getEmail()
{
  return m_email;
}

set<string> WsLdapAuth::getUserGroups()
{
  return m_groups;
}

set<string> WsLdapAuth::getAllGroups()
{
  m_ldapStatus = notLoaded;
  m_properties = WsGlobalProperties::instance();
  gdCLdapServer cLdapServer;
  cLdapServer.strServer      =  m_properties->get("ldap", "server", "");
  cLdapServer.strDisplayName =  m_properties->get("ldap", "server_name", "");
  /* Common settings for groups */
  gdCLdap             cLdap;
  gdCLdapFilter       cFilter;
  cFilter.pServer = &cLdapServer;
  cFilter.nScope        = LDAP_SCOPE_SUB;
  /* Groups specific settings */
  cFilter.strInitialSearchBase = m_properties->get("ldap", "search_base_allgroups", "");
  cFilter.strFilter     = m_properties->get("ldap", "filter_allgroups", "");
  gdCLdapAttributes   cWitchAttrs2;
  gdCLdapEntries      cEntries2;
  cWitchAttrs2.push_back(new gdCLdapAttribute(m_properties->get("ldap", "allgroups_key", "")));
  cLdap.setManageDSAIT(true);
  if ( !cLdap.GetInfo(cFilter, cWitchAttrs2, cEntries2) ) {
    LOG(ERROR) << "WsLdapAuth::authentify() : Could not query ldap !";
    m_ldapStatus = onError;
    return set<string>();
  }
  for (int nEntry = 0; nEntry < cEntries2.size(); nEntry++) {
    for (int nAttr = 0; nAttr < cEntries2[nEntry]->Attrs.size(); nAttr++) {
      if ( cEntries2[nEntry]->Attrs[nAttr]->Attr == m_properties->get("ldap", "allgroups_key", "")) {
        for (int nVal = 0; nVal < cEntries2[nEntry]->Attrs[nAttr]->Values.size(); nVal++) {
          m_allGroups.insert(cEntries2[nEntry]->Attrs[nAttr]->Values[nVal]->ToStr().c_str());
        }
        continue;
      }
    }
  }
  m_ldapStatus = loaded;
  return m_allGroups;
}

bool WsLdapAuth::ipValid(string ip)
{
  string ipMask = WsGlobalProperties::instance()->get("ldap", "ip_mask", "*");
  if (ipMask == "*")
    return true;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  boost::char_separator<char> sep(".");
  tokenizer maskTok(ipMask, sep);
  tokenizer uipTok(ip, sep);
  tokenizer::iterator mask_iter = maskTok.begin();
  tokenizer::iterator ip_iter = uipTok.begin();
  for (; ip_iter != uipTok.end(); ++mask_iter, ++ip_iter) {
    if (mask_iter == maskTok.end())
      return false;
    if (*mask_iter == "*")
      continue;
    if (*ip_iter != *mask_iter)
      return false;
  }
  return true;
}
