/*
 *
 *       Filename:  WsFsTreeOperations.h
 *
 *    Description:
 *
 *        Created:  31/03/13 15:33:40
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFsTreeOperations.h"
#include <iostream>
using namespace std;
#include <boost/lexical_cast.hpp>

WsFsTreeOperations::WsFsTreeOperations()
{
  /* Get global properties instance */
  WsGlobalProperties* props = WsGlobalProperties::instance();
  m_conf = props;
  int refresh;
  try {
    /* Get refresh rate of FileSystemTree */
    refresh = boost::lexical_cast<int>(m_conf->get("global", "refresh_delay", "30"));
    if (refresh == 0)
      refresh = 30;
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsDaemon::bind() : Cannot cast refresh delay to int. Check conf value. Assuming delay is 3600s";
    refresh = 3600;
  }
  bool monitoring;
  /* Check if Gamin is enabled */
  if (m_conf->get("global", "monitoring", "false") == "true")
    monitoring = true;
  else
    monitoring = false;
  /* Instanciate the updater */
  m_updater = new WsFsTreeUpdater(m_conf->get("global", "root_path", "."), refresh, monitoring);
  /* Instanciate the Consultation and modification class */
  m_consultation = new WsFsTreeConsultation(m_updater);
  m_modification = new WsFsTreeModification(m_updater);
}


int WsFsTreeOperations::getPermissions( const std::set<std::string>& groups, const std::string& p)
{
  return m_consultation->getPermissions(groups, p);
}

WsNodeProperties* WsFsTreeOperations::getProperties ( const std::set<std::string>& groups, const std::string& p)
{
  return m_consultation->getProperties(groups, p);
}

std::string WsFsTreeOperations::getProperty( const std::set<std::string>& groups, const std::string& section, const std::string& p, const std::string& prop)
{
  return m_consultation->getProperty(groups, section, p, prop);
}

int WsFsTreeOperations::getLock(const std::set<std::string> groups, const std::string& uid, const std::string& path)
{
  return m_consultation->getLock(groups, uid, path);
}

int WsFsTreeOperations::putLock(const std::set<std::string> groups, const std::string& uid, const std::string& path)
{
  return m_consultation->putLock(groups, uid, path);
}

int WsFsTreeOperations::isLocked(const std::set<std::string> groups, const std::string& uid, const std::string& path, std::string& id)
{
  return m_consultation->isLocked(groups, uid, path, id);
}

WsAccessTree* WsFsTreeOperations::getAccessTree( const std::set<std::string>& groups)
{
  return m_consultation->getAccessTree(groups);;
}


std::vector<WsResultItem> WsFsTreeOperations::getSearchResults( const std::set<std::string>& groups, const std::string& terms)
{
  return m_consultation->getSearchResults(groups, terms);
}


const std::string WsFsTreeOperations::getRootPath()
{
  return m_consultation->getRootPath();
}

std::vector<std::string> WsFsTreeOperations::getTemplatesList( const std::set<std::string>& groups, const std::string& path)
{
  return m_consultation->getTemplatesList(groups, path);
}

int WsFsTreeOperations::saveProperties( const std::set<std::string>& groups, const std::string& json, const std::string& path)
{
  return m_modification->saveProperties(groups, json, path);
}

int WsFsTreeOperations::saveProperty( const std::set<std::string>& groups, const std::string& path, const std::string& section, const std::string& attr, const std::string& val)
{
  return m_modification->saveProperty(groups, path, section, attr, val);
}

int WsFsTreeOperations::createNode( const std::set<std::string>& groups, const std::string& uid, const string& path, int type)
{
  return m_modification->createNode( groups, uid, path, type);
}

int WsFsTreeOperations::deleteNode( const std::set<std::string>& groups, const std::string& uid, const string& path)
{
  return m_modification->deleteNode(groups, uid, path);
}

int WsFsTreeOperations::renameNode( const std::set<std::string>& groups, const std::string& uid, const string& path, const string& newPath)
{
  return m_modification->renameNode(groups, uid, path, newPath);
}

bool WsFsTreeOperations::isAdministrator( const std::set<std::string>& groups)
{
  return m_modification->isAdministrator(groups);
}

bool WsFsTreeOperations::isEditor( const std::set<std::string>& groups)
{
  return m_modification->isEditor(groups);
}

int WsFsTreeOperations::update()
{
  return m_updater->update();
}

const string& WsFsTreeOperations::getFsTreeStamp()
{
  return m_consultation->getFsTreeStamp();
}
