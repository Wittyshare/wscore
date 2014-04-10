/**
 *
 *       @file  WsFsTreeClient.cpp
 *
 *      @brief
 *
 *    @version  1.0
 *       @date  07/26/11 10:51:30
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFsTreeClient.h"
#include <Include/WsGlobalConfig.h>
#include <fstream>
#include <ConfigManager/WsNodeProperties.h>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

WsFsTreeClient::WsFsTreeClient(const string& id, const string& pass, const string& ip):
  m_uid(id),
  m_pass(pass),
  m_ip(ip)
{
}

WsFsTreeClient::~WsFsTreeClient()
{
  LOG(DEBUG) < "WsFsTreeClient :: destructor called";
}

int WsFsTreeClient::load()
{
  m_conf = WsGlobalProperties::instance();
  m_operation = new WsFsTreeOperations();
  if ( m_operation->update() == FAILURE)
    return FAILURE;
  int refresh;
  if (authentify() ==  FAILURE)
    return FAILURE;
}

int WsFsTreeClient::getPermissions(const string& p)
{
  return m_operation->getPermissions(m_groups, p);
}

const WsNodeProperties* WsFsTreeClient::getProperties(const string& p)
{
  return m_operation->getProperties(m_groups, p);
}

string WsFsTreeClient::getProperty(const std::string& section, const string& p, const string& prop)
{
  return m_operation->getProperty(m_groups, section, p, prop);
}

const string& WsFsTreeClient::getSurname() const
{
  return m_sn;
}

const string& WsFsTreeClient::getFirstName() const
{
  return m_givenName;
}

const string& WsFsTreeClient::getEmail() const
{
  return this->m_email;
}

int WsFsTreeClient::authentify()
{
  WsAuthenticator connector;
  if (connector.authentify(m_uid, m_pass, m_ip) != SUCCESS)
    return FAILURE;
  m_uid = connector.getUid();
  m_sn  = connector.getSurname();
  m_givenName = connector.getFirstName();
  m_groups = connector.getUserGroups();
  /* We add the uid to the groups, it allows to give access to a dir to only one user  */
  m_groups.insert(m_uid);
  m_email = connector.getEmail();
  return SUCCESS;
}

int WsFsTreeClient::getLock(const std::string& path)
{
  return m_operation->getLock(m_groups, m_uid, path);
}

int WsFsTreeClient::putLock(const std::string& path)
{
  return m_operation->putLock(m_groups, m_uid, path);
}

int WsFsTreeClient::isLocked(const std::string& path, std::string& uid)
{
  return m_operation->isLocked(m_groups, m_uid, path, uid);
}

NodePtr WsFsTreeClient::getAccessRoot(const bool& forceUpdate)
{
  return m_operation->getAccessTree(m_groups)->getRoot();
}

vector<WsResultItem> WsFsTreeClient::getSearchResults(const string& terms)
{
  return m_operation->getSearchResults(m_groups, terms);
}

set<string> WsFsTreeClient::getAllGroups()
{
  WsAuthenticator c;
  return c.getAllGroups();
}

const string WsFsTreeClient::getRootPath()
{
  return m_operation->getRootPath();
}

int WsFsTreeClient::saveProperties(WsNodeProperties* props, const string& p)
{
  return m_operation->saveProperties(m_groups, props->getRoot().toStyledString(), p);
}

int WsFsTreeClient::saveProperty(const string& path, const string& section, const string& attr, const string& val)
{
  return m_operation->saveProperty(m_groups, path, section, attr, val);
}

int WsFsTreeClient::createNode(const string& p, int type)
{
  return m_operation->createNode(m_groups, m_uid, p, type);
}

int WsFsTreeClient::deleteNode(const string& p)
{
  return m_operation->deleteNode(m_groups, m_uid, p);
}

int WsFsTreeClient::renameNode(const string& path, const string& newPath)
{
  return m_operation->renameNode(m_groups, m_uid, path, newPath);
}

bool WsFsTreeClient::isEditor()
{
  return m_groups.count(m_conf->get("global", "editor_group", "editor")) > 0;
}

bool WsFsTreeClient::isAdministrator()
{
  return m_groups.count(m_conf->get("global", "admin_group", "administrator")) > 0;
}

vector<string> WsFsTreeClient::getTemplatesList(const string& path)
{
}
