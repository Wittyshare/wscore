/**
 *
 *       @file  WsBasicAuth.cpp
 *
 *      @brief  Basic connector class
 *
 *    @version  1.0
 *       @date  07/15/11 09:25:47
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsBasicAuth.h"
#include <Include/WsGlobalConfig.h>
#include <boost/tokenizer.hpp>
#include <stdlib.h>
using namespace std;

extern "C" {
  void WsBasicAuthInit(void)
  {
    LOG(DEBUG) << "WsBasicAuth::WsBasicAuthInit() : Preparing required libraries : libgdcore ";
    void* hndl = dlopen("libgdcore.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL )
      LOG(ERROR) << "WsBasicAuth::WsBasicAuthInit() : Cannot load libgdcore.so shared library! " << dlerror();
  }
}


WsBasicAuth::WsBasicAuth()
{
  string guest = WsGlobalProperties::instance()->get("global", "guest_group", "guest");
  m_allGroups.insert(guest);
}

int WsBasicAuth::authentify(const string& uid, const string& pass, const std::string& ip)
{
  m_uid = uid;
  m_ip = ip;
  string guest = WsGlobalProperties::instance()->get("global", "guest_group", "guest");
  /* Generate unique id for the guest */
  int ran = rand() % 10000000 + 1;
  m_uid = m_uid + "-" + boost::lexical_cast<string>(ran);
  m_groups.insert(guest);
  return SUCCESS;
}

string WsBasicAuth::getUid()
{
  return m_uid;
}

string WsBasicAuth::getFirstName()
{
  return m_name;
}

string WsBasicAuth::getSurname()
{
  return m_surname;
}

string WsBasicAuth::getEmail()
{
  return m_email;
}

set<string> WsBasicAuth::getUserGroups()
{
  return m_groups;
}

set<string> WsBasicAuth::getAllGroups()
{
  return m_allGroups;
}

bool WsBasicAuth::ipValid(string ip)
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
