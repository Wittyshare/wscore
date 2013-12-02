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
