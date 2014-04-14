/**
 *
 *       @file  WsNodeResources.cpp
 *
 *      @brief  Node Resources
 *
 *    @version  1.0
 *       @date  07/26/11 14:18:49
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsNodeResources.h"
#include <Include/WsGlobalConfig.h>
#include "WsGlobalProperties.h"
#include <iostream>

#include <set>
using namespace std;
using namespace boost::filesystem;

WsNodeResources::WsNodeResources(boost::filesystem::path nodePath):
  m_nodePath(nodePath)
{
  //TODO scan and fill
  //Create resources if not exist
}

const vector<string>& WsNodeResources::get(Type t)
{
  switch (t) {
  case Icones:
    return m_iconesUrl;
  case Images:
    return m_imagesUrl;
  case Templates:
    return m_templatesUrl;
  }
}

void WsNodeResources::add(Type t, const string& value)
{
  switch (t) {
  case Icones:
    m_iconesUrl.push_back(value);
    break;
  case Images:
    m_imagesUrl.push_back(value);
    break;
  case Templates:
    m_templatesUrl.push_back(value);
    break;
  }
}

int WsNodeResources::createResourcesDirectories()
{
  return ErrorCode::Success;
}

