/**
 *
 *       @file  WsGlobalProperties.cpp
 *
 *      @brief  Global properties
 *
 *    @version  1.0
 *       @date  07/26/11 14:18:49
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsGlobalProperties.h"

using namespace Json;
using namespace std;
#include <Include/WsGlobalConfig.h>
#include <boost/algorithm/string.hpp>

WsGlobalProperties* WsGlobalProperties::m_instance = 0;

WsGlobalProperties::WsGlobalProperties():
  WsAbstractProperties()
{
}

WsGlobalProperties* WsGlobalProperties::instance()
{
  if (m_instance == 0) {
    m_instance = new WsGlobalProperties();
    LOG(DEBUG) << "WsGlobalProperties :: Created instance";
  }
  return m_instance;
}


std::string WsGlobalProperties::get(const std::string& section, const std::string& id, const std::string& def)
{
  if (!m_parsed) {
    if (parse(GlobalConfig::GlobalPropertiesPath) == FAILURE) {
      cerr << "ERROR: WsGlobalProperties :: Could not parse main wittyshare-global.json file... Exiting application" << endl;
      exit(-1);
    }
  }
  Value val = m_root[section][id];
  if (val == Value::null)
    return def;
  else return val.asString();
}

std::string WsGlobalProperties::getAndReplace(const std::string& section, const std::string& id, const std::string& newVal, const std::string& def)
{
  if (!m_parsed) {
    if (parse(GlobalConfig::GlobalPropertiesPath) == FAILURE) {
      cerr << "ERROR: WsGlobalProperties :: Could not parse main wittyshare-global.json file... Exiting application";
      exit(-1);
    }
  }
  Value val = m_root[section][id];
  if (val == Value::null)
    return def;
  else {
    string r = val.asString();
    boost::replace_all(r, "$1", newVal);
    return r;
  }
}

void WsGlobalProperties::set(const std::string& section, std::string key, std::string value)
{
  if (!m_parsed)
    if (parse(GlobalConfig::GlobalPropertiesPath) == FAILURE) {
      cerr << "ERROR: WsGlobalProperties :: Could not parse main wittyshare-global.json file... Exiting application";
      exit(-1);
    }
  m_root[section][key] = value;
}
