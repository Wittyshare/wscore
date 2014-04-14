/*
 *
 *       Filename:  WsAuthenticator.cpp
 *
 *    Description:  Handles connection of user
 *
 *        Created:  02/15/12 12:37:55
 *
 *         Author:  Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAuthenticator.h"

#include <Include/WsGlobalConfig.h>
#include <ConfigManager/WsGlobalProperties.h>
#include <Logger/WsLogger.h>
#include <dlfcn.h>
#include <stdio.h>
#include <iostream>
using namespace std;

WsAuthenticator::WsAuthenticator()
{
}

WsAuthenticator::~WsAuthenticator()
{
  if (m_auth != 0)
    delete m_auth;
}

int WsAuthenticator::authentify(const string& uid, const string& pass, const std::string& ip)
{
  if (loadModule() == ErrorCode::Failure)
    return ErrorCode::Failure;
  m_uid = uid;
  m_pass = pass;
  m_ip = ip;
  if (m_auth->authentify(m_uid, m_pass, ip) == ErrorCode::Failure) {
    LOG(ERROR) << "WsAuthenticator::authentify() :  Could not authentify " << m_uid << " : " << ip;
    return ErrorCode::Failure;
  }
  LOG(INFO) << "WsAuthenticator::authentify() : Authentication success for " << m_uid << " : " << ip;
  return ErrorCode::Success;
}

string WsAuthenticator::getUid()
{
  if (m_auth != 0)
    return m_auth->getUid();
  return "";
}

string WsAuthenticator::getFirstName()
{
  if (m_auth != 0)
    return m_auth->getFirstName();
  return "";
}

string WsAuthenticator::getSurname()
{
  if (m_auth != 0)
    return m_auth->getSurname();
  return "";
}

string WsAuthenticator::getEmail()
{
  if (m_auth != 0)
    return m_auth->getEmail();
  return "";
}

set<string> WsAuthenticator::getUserGroups()
{
  if (m_auth != 0)
    return m_auth->getUserGroups();
  return set<string>();
}

set<string> WsAuthenticator::getAllGroups()
{
  if (loadModule() == ErrorCode::Failure)
    return set<string>();
  return m_auth->getAllGroups();
}

int WsAuthenticator::loadModule()
{
  LOG(DEBUG) << "WsAuthenticator :: Loading auth module";
  /* Get the module name to load from the confifuration */
  string libName = WsGlobalProperties::instance()->get("global", "auth_libname", "libWsModAuthd.so");
  /* Open the library file (the module) */
  void* hndl = dlopen(libName.c_str(), RTLD_LAZY);
  if ( hndl == NULL ) {
    LOG(ERROR) << "WsAuthenticator::loadModule() : load ERROR " << dlerror();
    return ErrorCode::Failure;
  }
  LOG(DEBUG) << "WsAuthenticator :: Building module";
  /* Call the buildmodule function of the loaded so file */
  pf_wsAuthMod func = (pf_wsAuthMod) dlsym(hndl, "buildModule");
  m_auth = func();
  LOG(DEBUG) << "WsAuthenticator :: Done loading";
}
