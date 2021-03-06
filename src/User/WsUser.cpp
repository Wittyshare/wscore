
#include "WsUser.h"
#include <Include/WsGlobalConfig.h>
#include <fstream>


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
  if (m_client->load() == ErrorCode::Failure)
    return ErrorCode::Failure;
  return ErrorCode::Success;
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

const string& WsUser::getUid() const
{
  return m_uid;
}

int WsUser::getPermissions(const string& p)
{
  return m_client->getPermissions(cleanPath(p));
}

const WsNodeProperties* WsUser::getProperties(const string& p)
{
  return m_client->getProperties(cleanPath(p));
}

string WsUser::getProperty(const string& path, const string& section, const string& attr, const string& def)
{
  /* Return the property found if not empty string otherwise return def */
  string ret = m_client->getProperty(section, cleanPath(path), attr);
  if ( ret.size() > 0 ) return ret;
  return def;
}

int WsUser::saveProperty(const string& path, const string& section, const string& attr, const string& val)
{
  return m_client->saveProperty(cleanPath(path), section, attr, val);
}


int WsUser::createNode(const string& path, NodeType type)
{
  switch (type) {
  case File:
    return m_client->createNode(cleanPath(path), 0);
  case Directory:
    return m_client->createNode(cleanPath(path), 1);
  }
}

int WsUser::deleteNode(const string& path)
{
  return m_client->deleteNode(cleanPath(path));
}

int WsUser::renameNode(const string& path, const string& newPath)
{
  return m_client->renameNode(cleanPath(path), cleanPath(newPath));
}

int WsUser::getLock(const std::string& path)
{
  return m_client->getLock(cleanPath(path));
}

int WsUser::putLock(const std::string& path)
{
  return m_client->putLock(cleanPath(path));
}

int WsUser::isLocked(const std::string& path, std::string& uid)
{
  return m_client->isLocked(cleanPath(path), uid);
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
  return m_client->saveProperties(props, cleanPath(path));
}
  
int WsUser::writeFile(const std::string path, const std::string& text){
    int ret = getPermissions(path);
    if(ret == ErrorCode::NotFound)
        return ret;
    if(ret != GlobalConfig::ReadWrite)
        return ErrorCode::NoAccess;

    ofstream myfile;
    std::string p = getRootPath() + "/"+path;
    myfile.open (p.c_str());
    if(myfile.is_open()){
        myfile << text;
        myfile.close();
        return ErrorCode::Success;
    }
    return ErrorCode::Failure;
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
  return m_client->getTemplatesList(cleanPath(path));
}

std::string WsUser::cleanPath(const std::string& path)
{
  string newPath = path;
  boost::algorithm::replace_all(newPath, "//", "/");
  return newPath;
}
