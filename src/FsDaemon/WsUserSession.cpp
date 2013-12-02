/*
 *
 *       Filename:  WsUserSession.cpp
 *
 *    Description:
 *
 *        Created:  03/21/2013 01:47:11 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#include "WsUserSession.h"
#include <sys/time.h>
using namespace std;

WsUserSession::WsUserSession(const string& uid, const string& pass, const string& ip):
  m_uid(uid),
  m_pass(pass),
  m_ip(ip)
{
  m_timestamp = getNowTimestamp();
}

const set<string>& WsUserSession::getGroups()
{
  return m_groups;
}

void WsUserSession::setGroups(const set<string>& groups)
{
  m_groups = groups;
}

bool WsUserSession::isValidSession(const string& uid, const string& pass, const string& ip)
{
  if ( m_uid == uid && m_pass == pass && m_ip == ip)
    return getNowTimestamp() - m_timestamp <= 14400000; //Session is valid for 4 hours
  return false;
}

long WsUserSession::getNowTimestamp()
{
  timeval time;
  gettimeofday(&time, 0);
  long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
  return millis;
}

const string& WsUserSession::getName()
{
  return m_name;
}

const string& WsUserSession::getSurname()
{
  return m_surname;
}

const string& WsUserSession::getEmail()
{
  return m_email;
}

void WsUserSession::setName(const string& name)
{
  m_name = name;
}

void WsUserSession::setSurname(const string& name)
{
  m_surname = name;
}

void WsUserSession::setEmail(const string& email)
{
  m_email = email;
}
