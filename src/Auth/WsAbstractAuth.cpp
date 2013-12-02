/*
 *
 *       Filename:  WsAbstractAuth.cpp
 *
 *    Description:  Abstract class used to authentificate in the application
 *
 *        Created:  02/10/12 15:33:40
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAbstractAuth.h"
#include <set>
#include <iostream>

std::string WsAbstractAuth::getFirstName()
{
  return "";
}

std::string WsAbstractAuth::getSurname()
{
  return "";
}

std::string WsAbstractAuth::getEmail()
{
  return "";
}

std::set<std::string> WsAbstractAuth::getUserGroups()
{
  return std::set<std::string>();
}

std::set<std::string> WsAbstractAuth::getAllGroups()
{
  return std::set<std::string>();
}
