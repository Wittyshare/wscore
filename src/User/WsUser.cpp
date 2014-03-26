
#include "WsUser.h"
#include <Include/WsGlobalConfig.h>


WsUser::WsUser(const string& strUid, const string& pass, const string& ip)
  : m_uid(strUid),
    m_pass(pass),
    m_ip(ip)
{
}

WsUser::~WsUser()
{
  LOG(DEBUG) << "WsUser::~WsUser() : Deleting WsUser Object";
  if (m_client != 0)
    delete m_client;
  m_client = 0;
}

int WsUser::load()
{
  /* load the conf */
  WsGlobalProperties* prop = WsGlobalProperties::instance();
  m_conf = prop;
  /* Enable daemon */
#ifdef HAS_DAEMON
  if (m_conf->get("global", "daemon", "false") == "true") {
    /* Daemon enabled */
    LOG(INFO) << "WsUser::load() : Using FsDaemon" << endl;
    /* m_client will be a WsFsDaemonClient because we are using the daemon */
    m_client = new WsFsDaemonClient(m_uid, m_pass, m_ip);
  } else {
#endif //HAS_DAEMON
    /* Tree enabled */
    LOG(DEBUG) << "WsUser::load() : Using local FsTree" << endl;
    /* m_client will be a WsFsTreeClient because we are using the daemon */
    m_client = new WsFsTreeClient(m_uid, m_pass, m_ip);
#ifdef HAS_DAEMON
  }
  LOG(DEBUG) << "WsUser::load() : Getting PropertiesConfig::Daemon property OK";
#endif //HAS_DAEMON
  /* load */
  if (m_client->load() == FAILURE)
    return FAILURE;
  return SUCCESS;
}

NodePtr WsUser::getAccessRoot()
{
  return m_client->getAccessRoot();
}

const string WsUser::getRootPath()
{
  string s = m_client->getRootPath();
  return s;
}

const string& WsUser::getSurname() const
{
  return m_client->getSurname();
}

const string& WsUser::getFirstName() const
{
  return m_client->getFirstName();
}

const string& WsUser::getEmail() const
{
  return m_client->getEmail();
}

NodePtr WsUser::getMenuRoot()
{
  return m_client->getMenuRoot();
}

int WsUser::getPermissions(const string& p)
{
  string newPath = p;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->getPermissions(newPath);
}

const WsNodeProperties* WsUser::getProperties(const string& p)
{
  string newPath = p;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->getProperties(newPath);
}

string WsUser::getProperty(const string& path, const string& section, const string& attr, const string& def)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  /* Return the property found if not empty string otherwise return def */
  string ret = m_client->getProperty(section, newPath, attr);
  if ( ret.size() > 0 ) return ret;
  return def;
}

int WsUser::saveProperty(const string& path, const string& section, const string& attr, const string& val)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->saveProperty(newPath, section, attr, val);
}


int WsUser::createNode(const string& path, NodeType type)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  switch (type) {
  case File:
    return m_client->createNode(newPath, 0);
  case Directory:
    return m_client->createNode(newPath, 1);
  }
}

int WsUser::deleteNode(const string& path)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->deleteNode(newPath);
}

int WsUser::renameNode(const string& path, const string& newPath)
{
  string npath = path;
  boost::algorithm::replace_all(npath, "//","/");
  string nnewPath = newPath;
  boost::algorithm::replace_all(nnewPath, "//","/");
  return m_client->renameNode(npath, nnewPath);
}

NodePtr WsUser::getMenuRoot(const set<string>& exclNames, const set<string>& exclExt)
{
  return m_client->getMenuRoot(exclNames, exclExt);
}

const string WsUser::getHomePage()
{
  //FIXME  get property from here ?
  return m_conf->get("global", "home_page", "");
}

vector<WsResultItem> WsUser::getSearchResults(const string& terms)
{
  return m_client->getSearchResults(terms);
}

set<string> WsUser::getAllGroups()
{
  return m_client->getAllGroups();
}

int WsUser::saveProperties(WsNodeProperties* props, const string& path)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->saveProperties(props, newPath);
}

bool WsUser::isEditor()
{
  return m_client->isEditor();
}

bool WsUser::isAdministrator()
{
  return m_client->isAdministrator();
}

vector<string> WsUser::getTemplatesList(const string& path)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//","/");
  return m_client->getTemplatesList(newPath);
}
