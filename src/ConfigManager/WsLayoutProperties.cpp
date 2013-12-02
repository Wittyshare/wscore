/*
 *
 *       Filename:  WsLayoutProperties.cpp
 *
 *    Description:
 *
 *        Created:  03/29/2012 03:47:05 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsLayoutProperties.h"

#include <Logger/WsLogger.h>

#include <Include/WsGlobalConfig.h>

using namespace Json;

WsLayoutProperties* WsLayoutProperties::m_instance = 0;

WsLayoutProperties::WsLayoutProperties():
  WsAbstractProperties()
{
}

WsLayoutProperties* WsLayoutProperties::instance()
{
  if (m_instance == 0) {
    m_instance = new WsLayoutProperties();
    LOG(DEBUG) << "WsLayoutProperties::instance() :  Created instance";
  }
  return m_instance;
}

std::string WsLayoutProperties::get(const std::string& section, const std::string& id, const std::string& def)
{
  if (!m_parsed) {
    parse(GlobalConfig::LayoutPropertiesPath);
  }
  Value val = m_root[section][id];
  if (val == Value::null)
    return def;
  else
    return val.asString();
}
