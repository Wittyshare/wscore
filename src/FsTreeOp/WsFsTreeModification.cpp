/*
 *
 *       Filename:  WsFsTreeModification.cpp
 *
 *    Description:
 *
 *        Created:  03/21/2013 04:18:54 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#include "WsFsTreeModification.h"

#include <ConfigManager/WsGlobalProperties.h>
#include <Search/WsSearch.h>
#include <boost/lexical_cast.hpp>


WsFsTreeModification::WsFsTreeModification(WsFsTreeUpdater* updater):
  m_updater(updater)
{
  m_conf = WsGlobalProperties::instance();
}

int WsFsTreeModification::saveProperties( const std::set<std::string>& groups, const std::string& json, const std::string& path)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(path);
  if ( n.get() == 0) {
    /* No access */
    return ErrorCode::NotFound;
  }
  if (!canEdit(n, groups)) return ErrorCode::NoAccess;
  /* Set the root of the properties to the new Json */
  n.get()->getProperties().get()->setRoot(json);
  /* save it */
  if (n.get()->getProperties().get()->save() == ErrorCode::Failure ) {
    return ErrorCode::Failure;
  }
  return ErrorCode::Success;
}

int WsFsTreeModification::saveProperty( const std::set<std::string>& groups, const std::string& path, const std::string& section, const std::string& attr, const std::string& val)
{
  FileSystemTreePtr ft = m_updater->getLastTree();
  NodePtr n;
  n = ft->eatPath(path);
  if ( n.get() == 0) {
    /* No access */
    return ErrorCode::NotFound;
  }
  if (!canEdit(n, groups)) return ErrorCode::NoAccess;
  /* Set property */
  n.get()->getProperties().get()->set(section, attr, val);
  /* save it */
  if (n.get()->getProperties().get()->save() == ErrorCode::Failure ) {
    return ErrorCode::Failure;
  }
  return ErrorCode::Success;
}

int WsFsTreeModification::createNode( const std::set<std::string>& groups, const string& uid, const string& p, int type)
{
  NodePtr n;
  boost::filesystem::path parent = p;
  boost::filesystem::path root = m_updater->getLastTree()->getRootPath();
  parent = parent.parent_path();
  FileSystemTreePtr ft = m_updater->getLastTree();
  /* Test if parent directory exists */
  n = ft->eatPath(parent.string());
  if ( n.get() == 0) {
    LOG(ERROR) << "WsFsTreeClient::createNode() : Parent Node not found " << parent.string();
    return ErrorCode::NotFound;
  }
  /* Test if parent is a dirNode */
  if ( !n.get()->isDirectory()) {
    LOG(ERROR) << "WsFsTreeClient::createNode() : Parent Node is not a directory " << parent.string();
    return ErrorCode::Failure;
  }

  if (!canEdit(n, groups)) return ErrorCode::NoAccess;

  /* Check if new node already exit */
  if (ft->eatPath(p).get() != 0) {
    LOG(INFO) << "WsFsTreeClient::createNode() : Node already exist" << p;
    return ErrorCode::Failure;
  }
  switch (type) {
  case 0: { /* Create a FileNode */
      fstream f;
      /* Create the file on disk */
      if(!exists( string(root.string() + p).c_str())){
	f.open( string(root.string() + p).c_str(), ios::out );
	if (!f.is_open())
		return ErrorCode::Failure;
	f.close();
      }
      /* Create Node and properties */
      WsFileNode* fn = new WsFileNode(root / p, root);
      NodePropertiesPtr props(new WsNodeProperties(root / path(p), WsNodeProperties::File));
      fn->setProperties(props);
      NodePtr nn = NodePtr(fn);
      /* Insert Node */
      m_updater->getLastTree()->insertNode(nn);
      return ErrorCode::Success;
    }
  case 1: { /* DirNode */
      /* Create Node */
      try {
        /* Create the directory on disk */
        boost::filesystem::create_directory(root / path(p));
      } catch (std::exception& e) {
        LOG(ERROR) << "WsFsTreeClient::createNode() : could not create node " << e.what();
        return ErrorCode::Failure;
      }
      /* Create Config Files */
      try {
        boost::filesystem::create_directory(root / path(p) / GlobalConfig::PathToConfig);
        NodePropertiesPtr fp(new WsNodeProperties( root / path(p), WsNodeProperties::Dir));
        set<string> groups;
        groups.insert(uid);
        fp->setGroups(groups);
        fp->save();
        WsDirNode* dn = new WsDirNode(root / p, root);
        dn->setProperties(fp);
        NodePtr nn = NodePtr(dn);
        /* Insert the new Node */
        m_updater->getLastTree()->insertNode(nn);
        return ErrorCode::Success;
      } catch (std::exception& e) {
        LOG(ERROR) << "WsFsTreeClient::createNode() : could not create node " << e.what();
        return ErrorCode::Failure;
      }
    }
  default:
    return ErrorCode::Failure;
  }
}

int WsFsTreeModification::deleteNode( const std::set<std::string>& groups, const string& uid,  const string& p)
{
  NodePtr n;
  FileSystemTreePtr ft = m_updater->getLastTree();
  n = ft->eatPath(p);
  if ( n.get() == 0) {
    LOG(ERROR) << "WsFsTreeClient::deleteNode() : Node not found " << p;
    return ErrorCode::NotFound;
  }
  if (!canEdit(n, groups)) return ErrorCode::Failure;
  /* Check if file or directory exist */
  if ( !boost::filesystem::exists(ft->getRootPath() / p)) {
    LOG(ERROR) << "WsFsTreeClient::deleteNode() : Node do not exist" << p;
    return ErrorCode::NoAccess;
  }
  if ( n.get()->isRegularFile()) {
    /* Remove config file if it exists */
    std::string propFileName = n.get()->getName() + ".json";
    if (exists(ft->getRootPath() / n.get()->getPath().parent_path() / GlobalConfig::PathToNodeProperties / propFileName)) {
      try {
        boost::filesystem::remove_all(ft->getRootPath() / n.get()->getPath().parent_path() / GlobalConfig::PathToNodeProperties / propFileName );
      } catch (std::exception& e) {
        LOG(ERROR) << "WsFsTreeClient::deleteNode() : could not delete node " << propFileName << " : " << e.what();
        return ErrorCode::Failure;
      }
      LOG(INFO) << "WsFsTreeClient::deleteNode() : Removing config files for " << p;
    }
    std::string lockFileName = n.get()->getName() + ".lock";
    if (exists(ft->getRootPath() / n.get()->getPath().parent_path() / GlobalConfig::PathToNodeLock / lockFileName)) {
      try {
        boost::filesystem::remove_all(ft->getRootPath() / n.get()->getPath().parent_path() / GlobalConfig::PathToNodeLock / lockFileName );
      } catch (std::exception& e) {
        LOG(ERROR) << "WsFsTreeClient::deleteNode() : could not delete node " << lockFileName << " : " << e.what();
        return ErrorCode::Failure;
      }
      LOG(INFO) << "WsFsTreeClient::deleteNode() : Removing lock files for " << p;
    }
  }
  /* Remove Node */
  try {
    boost::filesystem::remove_all(ft->getRootPath() / p);
  } catch (std::exception& e) {
    LOG(ERROR) << "WsFsTreeClient::deleteNode() : could not create node " << e.what();
    return ErrorCode::Failure;
  }
  LOG(INFO) << "WsFsTreeClient :: Removing Node " << p;
  return ErrorCode::Success;
}

int WsFsTreeModification::renameNode( const std::set<std::string>& groups, const string& uid, const string& p, const string& newPath)
{
  path root = m_updater->getLastTree()->getRootPath();
  NodePtr n;
  FileSystemTreePtr ft = m_updater->getLastTree();
  n = ft->eatPath(p);
  if ( n.get() == 0) {
    LOG(ERROR) << "WsFsTreeClient::renameNode() : Node not found " << p;
    return ErrorCode::NotFound;
  }
  /* Check if parent is a dir */
  if (!n.get()->getParent()->isDirectory()) {
    LOG(DEBUG) << "WsFsDaemon::renameNode() : Rename dest parent not a dir";
    return ErrorCode::Failure;
  }
  if (!canEdit(n, groups)) return ErrorCode::Failure;
  /* Check if node exist */
  if ( !boost::filesystem::exists(root / p)) {
    LOG(ERROR) << "WsFsTreeClient::renameNode() : Node do not exist" << p;
    return ErrorCode::NotFound;
  }
  /* Check if destination folder exist */
  if ( !boost::filesystem::exists(root / path(newPath).parent_path())) {
    LOG(ERROR) << "WsFsTreeClient::renameNode() : Parent dest node do not exist" << p;
    return ErrorCode::NotFound;
  }
  /* Check if destination path already exist */
  if ( boost::filesystem::exists(root / newPath)) {
    LOG(ERROR) << "WsFsTreeClient::renameNode() : Destination already exist" << newPath;
    return ErrorCode::Failure;
  }
  if (n.get()->isRegularFile() ) {
    /* Move config if exist */
    string oldConfFilename = path(p).filename().string() + GlobalConfig::ConfExt;
    path oldConfPath = root / path(p).parent_path() / path(GlobalConfig::PathToNodeProperties);
    string newConfFilename = path(newPath).filename().string() + GlobalConfig::ConfExt;
    path newConfPath = root / path(newPath).parent_path() / path(GlobalConfig::PathToNodeProperties);
    if (exists(oldConfPath / oldConfFilename)) {
      try {
        /* Move configuraton file */
        rename(oldConfPath / oldConfFilename , newConfPath / newConfFilename);
      } catch (std::exception& e) {
        LOG(ERROR) << "WsFsTreeClient::renameNode() : could not create node " << e.what();
        return ErrorCode::Failure;
      }
    }
  }
  /* Rename node */
  try {
    boost::filesystem::rename(ft->getRootPath() / p, ft->getRootPath() / newPath );
    if (n->isDirectory()) {
      /* Create a new DirNode and properties */
      NodePropertiesPtr props(new WsNodeProperties(root / newPath, WsNodeProperties::Dir));
      WsDirNode* dn = new WsDirNode(root / newPath, root);
      dn->setProperties(props);
      NodePtr nn = NodePtr(dn);
      /* Insert it in tree */
      if ( m_updater->getLastTree()->insertNode(nn) == ErrorCode::Failure)
        return ErrorCode::Failure;
      return ErrorCode::Success;
    } else {
      /* Create a new FileNode and properties */
      NodePropertiesPtr props(new WsNodeProperties(root / newPath, WsNodeProperties::File));
      WsFileNode* dn = new WsFileNode(root / newPath, root);
      dn->setProperties(props);
      NodePtr nn = NodePtr(dn);
      /* Insert it in tree */
      if ( m_updater->getLastTree()->insertNode(nn) == ErrorCode::Failure)
        return ErrorCode::Failure;
      return ErrorCode::Success;
    }
  } catch (std::exception& e) {
    LOG(ERROR) << "WsFsTreeClient::renameNode() : could not create node " << e.what();
    return ErrorCode::Failure;
  }
  return ErrorCode::Success;
}

bool WsFsTreeModification::isEditor(const std::set<std::string>& groups)
{
  return groups.count(m_conf->get("global", "editor_group", "editor")) > 0;
}

bool WsFsTreeModification::isAdministrator(const std::set<std::string>& groups)
{
  return groups.count(m_conf->get("global", "admin_group", "administrator")) > 0;
}

bool WsFsTreeModification::canEdit(NodePtr node, std::set<std::string> groups)
{
  /* If allowed and editor or if admin return true */
  if ((node.get()->isAllowed(groups) && groups.count(m_conf->get("global", "editor_group", "editor")) >  0)
      || groups.count(m_conf->get("global", "admin_group", "administrator")) > 0){
      LOG(DEBUG)<<"WsFsTreeModification::canEdit() : user can edit ";
    return true;
  }
  LOG(DEBUG)<<"WsFsTreeModification::canEdit() : user cannot edit ";
  return false;
}
