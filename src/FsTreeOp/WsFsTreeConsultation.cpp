/*
 *
 *       Filename:  WsFsTreeConsultation.cpp
 *
 *    Description:
 *
 *        Created:  03/21/2013 04:04:50 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#include "WsFsTreeConsultation.h"

#include <ConfigManager/WsGlobalProperties.h>
#include <Search/WsSearch.h>
#include <boost/lexical_cast.hpp>

WsFsTreeConsultation::WsFsTreeConsultation(WsFsTreeUpdater* updater):
  m_updater(updater)
{
  m_conf = WsGlobalProperties::instance();
}

int WsFsTreeConsultation::getPermissions( const std::set<std::string>& groups, const std::string& p)
{
  NodePtr n;
  FileSystemTreePtr ft = m_updater->getLastTree();
  /* Get node */
  n = ft->eatPath(p);
  if (n.get() == 0) {
    /* Path not found */
    LOG(INFO) << "WsFsTreeConsultation::getPermissions() : No such file or directory :" << p;
    return GlobalConfig::NotFound;
  }
  if (groups.count(m_conf->get("global", "admin_group", "administrator")) > 0) {
    /* user is admin , readwrite perms*/
    return GlobalConfig::ReadWrite;
  }
  WsGlobalProperties* props = WsGlobalProperties::instance();
  if (props->get("global", "public_site", "false") == "true") {
    /* Public site read access granted */
    return GlobalConfig::Read;
  }
  /* Check if user is allowed */
  if (n.get()->isAllowed(groups)) {
    if (groups.count(m_conf->get("global", "editor_group", "editor")) > 0) {
      /* user is editor. read write perms */
      return GlobalConfig::ReadWrite;
    } else {
      /* user is a normal user. read perms */
      return GlobalConfig::Read;
    }
  } else {
    /* no access */
    return GlobalConfig::NoAccess;
  }
  return FAILURE;
}

WsNodeProperties* WsFsTreeConsultation::getProperties ( const std::set<std::string>& groups, const std::string& p)
{
  /* Get the node */
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(p);
  if (n.get() == 0) {
    /* Path not found */
    LOG(INFO) << "WsFsTreeConsultation::getProperties() : No such file or directory :" << p;
    return 0;
  }
  /* Check if user has access to it */
  if (!n.get()->isAllowed(groups)) {
    return 0;
  }
  return n.get()->getProperties().get();
}

std::string WsFsTreeConsultation::getProperty( const std::set<std::string>& groups, const std::string& section, const std::string& p, const std::string& prop)
{
  WsNodeProperties* props = this->getProperties(groups, p);
  if ( props == 0)
    return "";
  return props->get(section, prop, "");
}

WsMenuTree* WsFsTreeConsultation::getMenuTree( const std::set<std::string>& groups)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  int depth;
  try {
    depth = boost::lexical_cast<int>(m_conf->get("global", "max_menu_depth", "0"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::getMenuTree() : Cannot cast depth to int. Check conf value. Assuming depth is unlimited";
    depth = 0;
  }
  NodePtr n;
  n = ft->getRoot();
  if (n.get() == 0)
    return 0;
  WsMenuTree* tree = new WsMenuTree(n, groups, depth, ft->getRootPath(), ft->getStamp());
  if (tree->build() == FAILURE) {
    return 0;
  }
  return tree;
}

WsMenuTree* WsFsTreeConsultation::getMenuTree( const std::set<std::string>& groups, const std::set<string>& exclNames, const std::set<string>& exclExt)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  int depth;
  try {
    depth = boost::lexical_cast<int>(m_conf->get("global", "max_menu_depth", "0"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::getMenuTree() : Cannot cast depth to int. Check conf value. Assuming depth is unlimited";
    depth = 0;
  }
  NodePtr n;
  n = ft->getRoot();
  if (n.get() == 0)
    return 0;
  WsMenuTree* tree = new WsMenuTree(n, groups, depth, exclNames, exclExt, ft->getRootPath(), ft->getStamp());
  if (tree->build() == FAILURE) {
    return 0;
  }
  return tree;
}


int WsFsTreeConsultation::getLock(const std::set<std::string> groups, const std::string& uid, const std::string& path)
{
  boost::mutex::scoped_lock lock(m_lockEditMutex);
  int ld = 0;
  try {
    ld = boost::lexical_cast<int>(m_conf->get("global", "lock_duration", "3600"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::getLock() : Cannot cast lock_duration to int. Check conf value";
    ld = 3600;
  }
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(path);
  if (n.get() == 0) {
    /* Path not found */
    LOG(INFO) << "WsFsTreeConsultation::getLock : No such file or directory :" << path;
    return -1;
  }
  // Check if lock already exist and if yes if it is owned by this user
  boost::filesystem::path root = m_updater->getLastTree()->getRootPath();
  boost::filesystem::path p;
  if (n.get()->isDirectory()) return -1; //Cannot lock directory
  else p = n.get()->getPath().parent_path();
  std::string name = n.get()->getName() + ".lock";
  p = root / p / GlobalConfig::PathToNodeLock / name;
  LOG(DEBUG) << "WsFsTreeConsultation::getLock() : path is " << p.string();
  std::string id = "";
  std::string ts = "";
  Json::Reader reader;
  Json::Value v;
  //Conf file exist
  if (boost::filesystem::exists(p)) {
    std::ifstream lock(p.c_str(), std::ifstream::binary);
    bool bOk = reader.parse(lock, v, false);
    lock.close();
    if ( !bOk )
      boost::filesystem::remove(p);
    else {
      id = v["uid"].asString();
      ts = v["timestamp"].asString();
      // File is already locked by someone else
      if ( id != uid)
        if (boost::lexical_cast<long>(ts) > getTimeMs() - (ld * 1000) )
          return 0;
        else boost::filesystem::remove(p); //timeout
    }
  }
  // write updated data to file
  long millis = getTimeMs();
  ts = boost::lexical_cast<string>(millis);
  v["uid"] = uid;
  v["timestamp"] = ts;
  ofstream out;
  out.open(p.string().c_str(), ios::out | ios::trunc | ios::binary);
  if (out.is_open()) {
    out << v.toStyledString();
    out.close();
    return ld;
  } else return -1;
}

int WsFsTreeConsultation::putLock(const std::set<std::string> groups, const std::string& uid, const std::string& path)
{
  boost::mutex::scoped_lock lock(m_lockEditMutex);
  int ld = 0;
  try {
    ld = boost::lexical_cast<int>(m_conf->get("global", "lock_duration", "3600"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::getLock() : Cannot cast lock_duration to int. Check conf value";
    ld = 3600;
  }
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(path);
  if (n.get() == 0) {
    /* Path not found */
    LOG(INFO) << "WsFsTreeConsultation::putLock : No such file or directory :" << path;
    return -1;
  }
  // Check if lock already exist and if yes if it is owned by this user
  boost::filesystem::path root = m_updater->getLastTree()->getRootPath();
  boost::filesystem::path p;
  if (n.get()->isDirectory()) return -1; //Cannot unlock directory
  else p = n.get()->getPath().parent_path();
  std::string name = n.get()->getName() + ".lock";
  p = root / p / GlobalConfig::PathToNodeLock / name;
  LOG(DEBUG) << "WsFsTreeConsultation::putLock() : path is " << p.string();
  std::string id = "";
  std::string ts = "";
  Json::Reader reader;
  Json::Value v;
  //Conf file exist
  if (boost::filesystem::exists(p)) {
    std::ifstream lock(p.c_str(), std::ifstream::binary);
    bool bOk = reader.parse(lock, v, false);
    lock.close();
    if ( !bOk ) {
      boost::filesystem::remove(p);
      return 1;
    } else {
      id = v["uid"].asString();
      ts = v["timestamp"].asString();
      // File is already locked by someone else
      if ( id != uid) {
        if (boost::lexical_cast<long>(ts) > getTimeMs() - (ld * 1000) )
          return 0;
        else {
          boost::filesystem::remove(p); //timeout existing lock
          return 1;
        }
      }
      boost::filesystem::remove(p); //Lock is hold by current user so we can delete it
      return 1;
    }
  }
  // No lock found
  return 1;
}


int WsFsTreeConsultation::isLocked(const std::set<std::string> groups, const std::string& uid, const std::string& path, std::string& id)
{
  boost::mutex::scoped_lock lock(m_lockEditMutex);
  int ld = 0;
  try {
    ld = boost::lexical_cast<int>(m_conf->get("global", "lock_duration", "3600"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::isLocked() : Cannot cast lock_duration to int. Check conf value";
    ld = 3600;
  }
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(path);
  if (n.get() == 0) {
    /* Path not found */
    LOG(INFO) << "WsFsTreeConsultation:isLocked : No such file or directory :" << path;
    return -1;
  }
  // Check if lock already exist and if yes if it is owned by this user
  boost::filesystem::path root = m_updater->getLastTree()->getRootPath();
  boost::filesystem::path p;
  if (n.get()->isDirectory()) return -1; //Cannot lock directory
  else p = n.get()->getPath().parent_path();
  std::string name = n.get()->getName() + ".lock";
  p = root / p / GlobalConfig::PathToNodeLock / name;
  LOG(DEBUG) << "WsFsTreeConsultation::isLocked() : path is " << p.string();
  std::string lid = "";
  std::string ts = "";
  Json::Reader reader;
  Json::Value v;
  //Conf file exist
  if (boost::filesystem::exists(p)) {
    std::ifstream lock(p.c_str(), std::ifstream::binary);
    bool bOk = reader.parse(lock, v, false);
    lock.close();
    if ( !bOk ) {
      boost::filesystem::remove(p);
      return 1;
    } else {
      lid = v["uid"].asString();
      ts = v["timestamp"].asString();
      // File is already locked by someone else
      if ( lid != uid) {
        if (boost::lexical_cast<long>(ts) > getTimeMs() - (ld * 1000) ) {
          id = lid;
          return 0;
        } else {
          boost::filesystem::remove(p); //timeout existing lock
          return 1;
        }
      }
      boost::filesystem::remove(p); //Lock is hold by current user so we can delete it
      return 1;
    }
  }
  // No lock found
  return 1;
}

long WsFsTreeConsultation::getTimeMs()
{
  timeval time;
  gettimeofday(&time, 0);
  return (time.tv_sec * 1000) + (time.tv_usec / 1000);
}

WsAccessTree* WsFsTreeConsultation::getAccessTree( const std::set<std::string>& groups)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  int depth;
  try {
    depth = boost::lexical_cast<int>(m_conf->get("global", "max_menu_depth", "0"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsTreeConsultation::getAccessTree() : Cannot cast depth to int. Check conf value. Assuming depth is unlimited";
    depth = 0;
  }
  n = ft->getRoot();
  if (n.get() == 0)
    return 0;
  WsAccessTree* tree = new WsAccessTree(n, groups, depth, ft->getRootPath(), ft->getStamp());
  if (tree->build() == FAILURE) {
    return 0;
  }
  return tree;
}

std::vector<WsResultItem> WsFsTreeConsultation::getSearchResults( const std::set<std::string>& groups, const std::string& terms)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  WsSearch s(ft, terms);
  if (s.load() < 0)
    return vector<WsResultItem>();
  return s.getResults(groups);
}

const std::string WsFsTreeConsultation::getRootPath()
{
  return m_conf->get("global", "root_path", ".");
}

std::vector<std::string> WsFsTreeConsultation::getTemplatesList( const std::set<std::string>& groups, const std::string& path)
{
}

const string& WsFsTreeConsultation::getFsTreeStamp()
{
  return m_updater->getLastTree()->getStamp();
}
