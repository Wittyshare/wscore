/**
 *
 *       @file  WsNodeProperties.cpp
 *
 *      @brief  Node properties
 *
 *    @version  1.0
 *       @date  07/26/11 14:18:49
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsNodeProperties.h"
#include <Include/WsGlobalConfig.h>
#include "WsGlobalProperties.h"
#include <iostream>

#include <set>
using namespace std;

using namespace Json;

using namespace boost::filesystem;


WsNodeProperties::WsNodeProperties(boost::filesystem::path nodePath, Type t):
  WsAbstractProperties(),
  m_nodePath(nodePath),
  m_type(t)
{
}

WsNodeProperties::WsNodeProperties(string jsonInput)
{
  Reader r;
  if (r.parse(jsonInput, m_root, false))
    m_parsed = true;
}

WsNodeProperties::~WsNodeProperties()
{
}

string WsNodeProperties::get(const string& section, const string& id, const string& def)
{
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return string();
    }
    Value val = m_root[section][id];
    if (val == Value::null || val.asString() == "null") {
      return def;
    } else return val.asString();
  }
}


std::set<string> WsNodeProperties::getGroups()
{
  std::set<std::string> grp;
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return grp;
    }
    const Value groups = m_root["global"]["groups"];
    if (groups != Value::null) {
      for ( int i = 0; i < groups.size(); ++i) {
        grp.insert(groups[i].asString());
      }
    }
    return grp;
  }
}

bool WsNodeProperties::isAllowed(std::set<string> gids)
{
  std::set<string> nodeGroups = getGroups();
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return false;
    }
    WsGlobalProperties* props = WsGlobalProperties::instance();
    string admgrp = props->get("global", "admin_group", "");
    /* Admin has access to everything */
    if (gids.count(admgrp) > 0 ) {
      LOG(DEBUG) << "WsNodeProperties::isAllowed() :  User is Admin. Access allowed for " << getPath();
      return true;
    }
    std::set<string>::iterator itGids;
    /* Iterate over the list of users allowed and check */
    /* if there is a similar entry in the gids list */
    for (itGids = gids.begin(); itGids != gids.end(); ++itGids) {
      if (nodeGroups.count(*itGids) > 0) {
        /* We found a similar entry, we need to check publish date */
        /* Editor has access to node even if not published yet */
        string edtgrp = props->get("global", "editor_group", "");
        if (gids.count(edtgrp) > 0 ) {
          LOG(DEBUG) << "WsNodeProperties::isAllowed() User is Editor. Access allowed for " << getPath();
          return true;
        }
        LOG(DEBUG) << "WsNodeProperties::isAllowed() : User not editor";
        try {
          time_t pnode = boost::lexical_cast<time_t>(this->get("global", "publish_date", "0"));
          time_t pnow  = time(NULL);
          if (pnode <=  pnow) {
            return true;
          } else {
            LOG(DEBUG) << "WsNodeProperties::isAllowed() : File is not published yet. Access Denied for " << getPath();
            return false;
          }
        } catch (boost::bad_lexical_cast&) {
          LOG(ERROR) << "WsNodeProperties::isAllowed() : Cannot parse publish date for " << this->getPath();
          return false;
        }
      }
    }
    /* No similar entries found. Group has no access */
    LOG(DEBUG) << "WsNodeProperties :: User has no access ";
    return false;
  }
}

Value WsNodeProperties::getRoot()
{
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return Value();
    }
    return m_root;
  }
}

void WsNodeProperties::setRoot(Json::Value root)
{
  m_root = root;
  m_parsed = true;
}

void WsNodeProperties::setRoot(const std::string& jsonInput)
{
  {
    boost::mutex::scoped_lock lock(m_mutex);
    Reader r;
    if (r.parse(jsonInput, m_root, false))
      m_parsed = true;
    else
      m_parsed = false;
  }
}

void WsNodeProperties::set(const string& section, const string& key, const string& value)
{
  if (key.size() == 0 || value.size() == 0)
    return;
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return;
    }
    m_root[section][key] = value;
  }
}

void WsNodeProperties::setGroups(std::set<string> grps)
{
  {
    boost::mutex::scoped_lock lock(m_mutex);
    if (!m_parsed) {
      string p = getPath();
      if (parse(p) == FAILURE)
        return;
    }
    m_root["global"]["groups"].clear();
    std::set<string>::iterator it;
    int i;
    for (it = grps.begin(), i = 0; it != grps.end(); ++it, ++i) {
      m_root["global"]["groups"][i] = *it;
    }
  }
}

int WsNodeProperties::createPropertiesDirectories()
{
  {
    boost::mutex::scoped_lock try_lock(m_mutex);
    try {
      path p = m_nodePath;
      for (int i = 0; i < GlobalConfig::NbItems; ++i) {
        if ( !exists(p / GlobalConfig::SubFolders[i]) ) {
          create_directory(p / GlobalConfig::SubFolders[i]);
          LOG(DEBUG) << "WsNodeProperties::createPropertiesDirectories() : Creating Config dir " << (p / GlobalConfig::SubFolders[i]).string();
        }
      }
      return SUCCESS;
    } catch (exception& e) {
      LOG(ERROR) << "WsNodeProperties::createPropertiesDirectories() : " << e.what();
    }
  }
  return SUCCESS;
}

int WsNodeProperties::save()
{
  boost::mutex::scoped_lock lock(m_mutex);
  ofstream conf;
  string p = getPath();
  conf.open(p.c_str(), ios::out | ios::trunc | ios::binary);
  if (conf.is_open()) {
    conf << m_root.toStyledString();
    conf.close();
    return SUCCESS;
  }
  return FAILURE;
}

string WsNodeProperties::getPath()
{
  string p;
  if (m_type == Dir) {
    p += m_nodePath.string();
    p += GlobalConfig::PathToDirProperties;
  } else {
    p += m_nodePath.parent_path().string();
    p += GlobalConfig::PathToNodeProperties + m_nodePath.filename().string() + GlobalConfig::ConfExt;
  }
  return p;
}

