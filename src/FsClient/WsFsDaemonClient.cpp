/**
 *
 *       @file  WsfsDaemonClient.cpp
 *
 *      @brief Interacts with the menuTree and FsTree via the daemon
 *
 *    @version  1.0
 *       @date  07/26/11 09:39:31
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

//#ifdef HAS_DAEMON

#include "WsFsDaemonClient.h"
#include <ConfigManager/WsGlobalProperties.h>
#include <Serializer/WsTreeDeserializer.h>
#include <Serializer/WsArrayDeserializer.h>
#include <Include/WsGlobalConfig.h>
#include <Include/WsRequestType.h>

using namespace zmq;
using namespace Json;

zmq::context_t*  WsFsDaemonClient::m_context = 0;

WsFsDaemonClient::WsFsDaemonClient(const string& id, const string& pass, const string& ip):
  m_uid(id),
  m_pass(pass),
  m_ip(ip),
  m_name(""),
  m_surname(""),
  m_email(""),
  m_rootPath(""),
  m_isAdmin(-1),
  m_isEditor(-1),
  m_listen(true),
  m_compressor(0),
  m_decompressor(0),
  m_sendMutex(0),
  m_updateThread(0)
{
  //Init ZMQ context
  if (m_context == 0) {
    LOG(DEBUG) << "WsFsDaemonClient::WsFsDaemonClient() : Context created";
    m_context = new context_t(1);
  }
  m_sock = new socket_t(*m_context, ZMQ_REQ);
  m_sendMutex = new boost::mutex();
}

WsFsDaemonClient::~WsFsDaemonClient()
{
  m_listen = false;
  LOG(INFO) << "WsFsDaemonClient::~WsFsDaemonClient() :  Sending Clear Cache request to Server";
  clearServerCache();
  if (m_updateThread) {
    LOG(INFO) << "WsFsDaemonClient::~WsFsDaemonClient() : interrupting thread";
    m_updateThread->interrupt();
    LOG(INFO) << "WsFsDaemonClient::~WsFsDaemonClient() : joining thread";
    m_updateThread->join();
    LOG(INFO) << "WsFsDaemonClient::~WsFsDaemonClient() : thread ended";
    delete m_updateThread;
    LOG(INFO) << "WsFsDaemonClient::~WsFsDaemonClient() : thread deleted";
  }
  if (m_sendMutex)
    delete m_sendMutex;
  if (m_sock) {
    m_sock->close();
    delete m_sock;
  }
  if (m_compressor)
    delete m_compressor;
  if (m_decompressor)
    delete m_decompressor;
  m_updateThread = 0;
  m_sendMutex = 0;
  m_sock = 0;
  m_compressor = 0;
  m_decompressor = 0;
}

int WsFsDaemonClient::load()
{
  /* load conf file */
  WsGlobalProperties* pio = WsGlobalProperties::instance();
  m_conf = pio;
  //Load Server conf
  m_proto = m_conf->get("global", "protocol", "tcp");
  m_host = m_conf->get("global", "host", "127.0.0.1");
  m_port = m_conf->get("global", "port", "5555");
  //Enable Server-Client Compression ?
  if ( m_conf->get("global", "enable_compression", "true") == "true") {
    m_compressor = new WsCompressor();
    m_decompressor = new WsDecompressor();
    m_compress = true;
  } else m_compress = false;
  try {
    /* Connect to the server */
    m_sock->connect((m_proto + "://" + m_host + ":" + m_port).c_str());
  } catch (zmq::error_t e) {
    LOG(ERROR) << "WsFsDaemonClient::load() : Could not connect to host" << endl;
    return FAILURE;
  }
  if ( authentify() == FAILURE)
    return FAILURE;
  //Launch Thread that will download the tree if updated
  m_updateThread = new boost::thread(boost::bind(&WsFsDaemonClient::threadUpdate, this));
  return SUCCESS;
}

int WsFsDaemonClient::authentify()
{
  /* Prepare message to send */
  Value v;
  v[RequestField::Type]  = Auth;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  //Send request
  if (send(v.toStyledString()) == FAILURE)
    return FAILURE;
  if (receiveAuthAnswer() == FAILURE)
    return FAILURE;
  return SUCCESS;
}

int WsFsDaemonClient::receive(string& receivedData)
{
  message_t reply;
  try {
    m_sock->recv (&reply);
  } catch (zmq::error_t e) {
    LOG(ERROR) << "WsFsDaemonClient::receive() : Could not receive on socket" << endl;
    m_sendMutex->unlock();
    return FAILURE;
  }
  receivedData = rawDataToString(reply);
  m_sendMutex->unlock();
  return SUCCESS;
}

int WsFsDaemonClient::send(const string& s)
{
  try {
    m_sendMutex->lock();
    try {
      if (m_compress) {
        /* Compress data */
        char* data;
        long r = m_compressor->compress(s, &data);
        if (r == FAILURE)
          return FAILURE;
        message_t reply(r);
        /* Copy contents of the message */
        if (memcpy ((void*) reply.data (), data, r) == NULL) {
          LOG(ERROR) << "WsFsDaemonClient::send() : Could not memcpy in Server" << endl;
          m_sendMutex->unlock();
          return FAILURE;
        }
        /* Send the message */
        if (m_sock->send (reply) < 0) {
          LOG(ERROR) << "WsFsDaemonClient::send() : Could not send on socket " << endl;
          m_sendMutex->unlock();
          return FAILURE;
        }
      } else {
        /* Send without compression */
        message_t reply(s.length());
        if (memcpy ((void*) reply.data (), s.c_str(), s.length()) == NULL) {
          LOG(ERROR) << "WsFsDaemonClient::send() : Could not memcpy in Server" << endl;
          m_sendMutex->unlock();
          return FAILURE;
        }
        if (m_sock->send (reply) < 0) {
          LOG(ERROR) << "WsFsDaemonClient::send() : Could not send on socket " << endl;
          m_sendMutex->unlock();
          return FAILURE;
        }
      }
      return SUCCESS;
    } catch (zmq::error_t e) {
      LOG(ERROR) << "WsFsDaemonClienti::send():  Could not send on socket" << endl;
      m_sendMutex->unlock();
      return FAILURE;
    }
  } catch (boost::thread_resource_error e) {
    return FAILURE;
  }
  return SUCCESS;
}

int WsFsDaemonClient::receiveAuthAnswer()
{
  string resp;
  if (receive(resp) == FAILURE)
    return FAILURE;
  if (resp == RequestField::Failure || resp == "notlogged") {
    LOG(ERROR) << "WsFsDaemon::receiveAuthAnswer() : Could not authenticate on server " << m_uid;
    return FAILURE;
  }
  if (parse(resp) == FAILURE) {
    LOG(ERROR) << "WsFsDaemon::receiveAuthAnswer() : Could not parse server response ";
    return FAILURE;
  }
  Value v = m_root[RequestField::Uid];
  if (v != Value::null)
    m_uid = v.asString();
  v = m_root[RequestField::Name];
  if (v != Value::null)
    m_name = v.asString();
  v = m_root[RequestField::Surname];
  if (v != Value::null)
    m_surname = v.asString();
  v = m_root[RequestField::Email];
  if (v != Value::null)
    m_email = v.asString();
  return SUCCESS;
}

int WsFsDaemonClient::parse(const string& s)
{
  if (!m_reader.parse(s, m_root, false)) {
    LOG(ERROR) << "WsFsDaemonClient::parse() : Could not parse received input" << endl;
    return FAILURE;
  }
  return SUCCESS;
}

int WsFsDaemonClient::receiveSuccessCode()
{
  string resp;
  if (	receive(resp) == FAILURE)
    return FAILURE;
  if (resp == "success")
    return SUCCESS;
  return FAILURE;
}


int WsFsDaemonClient::clearServerCache()
{
  Value v;
  v[RequestField::Type] = ClearCache;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send(v.toStyledString()) == FAILURE)
    return FAILURE;
  if (receiveSuccessCode() == FAILURE)
    return FAILURE;
  return SUCCESS;
}

NodePtr WsFsDaemonClient::getMenuRoot(const bool& forceUpdate)
{
  /* Lock the mutex to avoid context error with zmq as 2 threads are accessing the same socket */
  if (!forceUpdate && m_menuRoot.get() != 0)
    return m_menuRoot;
  Value v;
  v[RequestField::Type] = MenuItems;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send(v.toStyledString()) == FAILURE)
    return NodePtr();
  return receiveMenuItems();
}

int WsFsDaemonClient::getLock(const std::string& path){
  Value v;
  v[RequestField::Type] = GetLock;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = path;
  if (send(v.toStyledString()) == FAILURE)
    return -1;
  return receiveInt();
}

int WsFsDaemonClient::isLocked(const std::string& path, std::string& uid)
{
  Value v;
  v[RequestField::Type] = IsLocked;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = path;
  if (send(v.toStyledString()) == FAILURE)
    return -1;
  std::string ret = receiveString();
  if( ret == "")
      return 1;
  else {
      uid = ret;
      return 0;
  }
}

int WsFsDaemonClient::putLock(const std::string& path){
  Value v;
  v[RequestField::Type] = PutLock;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = path;
  if (send(v.toStyledString()) == FAILURE)
    return -1;
  return receiveInt();
}

NodePtr WsFsDaemonClient::getAccessRoot( const bool& forceUpdate)
{
  /* Lock the mutex to avoid context error with zmq as 2 threads are accessing the same socket */
  if (!forceUpdate && m_accessRoot.get() != 0)
    return m_accessRoot;
  Value v;
  v[RequestField::Type] = AccessItems;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send( v.toStyledString()) == FAILURE)
    return NodePtr();
  return receiveAccessItems();
}

NodePtr WsFsDaemonClient::getMenuRoot(const set<string>& exclNames, const set<string>& exclExt)
{
  Value v;
  int i;
  set<string>::iterator it;
  v[RequestField::Type] = MenuItemsEx;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  for (it = exclNames.begin(), i = 0; it != exclNames.end(); ++it, ++i )
    v[RequestField::ExclNames][i] = *it;
  for (it = exclExt.begin(), i = 0; it != exclExt.end(); ++it, ++i )
    v[RequestField::ExclExt][i] = *it;
  if (send(v.toStyledString()) == FAILURE)
    return NodePtr();
  return receiveMenuItems();
}

const string WsFsDaemonClient::getRootPath()
{
  /* Avoid fetching path each time */
  if (m_rootPath.size() > 0)
    return m_rootPath;
  Value v;
  v[RequestField::Type] = RootPath;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send(v.toStyledString()) == FAILURE)
    return "";;
  m_rootPath = receiveString();
  return m_rootPath;
}

NodePtr WsFsDaemonClient::receiveMenuItems()
{
  string resp;
  if (	receive(resp) == FAILURE)
    return NodePtr();
  /* Check if no error occured server-side */
  if (resp == RequestField::Failure || resp == "notlogged")
    return NodePtr();
  /* Deserialize menu items */
  WsTreeDeserializer des(resp);
  if (des.deserialize() == FAILURE) {
    LOG(ERROR) << "WsFsDaemonClient::receiveMenuItems() : receiveMenuItems deserialize error " << endl;
    return NodePtr();
  }
  /* Set the new menuTree and it's stamp */
  m_menuRoot = des.getMenuRoot();
  m_menuTreeStamp = des.getStamp();
  return m_menuRoot;
}

NodePtr WsFsDaemonClient::receiveAccessItems()
{
  string resp;
  if (receive(resp) == FAILURE)
    return NodePtr();
  /* Check if no error occured server-side */
  if (resp == RequestField::Failure || resp == "notlogged")
    return NodePtr();
  /* Deserialize the tree */
  WsTreeDeserializer des(resp);
  if (des.deserialize() == FAILURE) {
    LOG(ERROR) << "WsFsDaemonClient::receiveAccessItems() : receiveMenuItems deserialize error " << endl;
    return NodePtr();
  }
  /* Set the new access tree and it's stamp */
  m_accessRoot = des.getMenuRoot();
  m_accessTreeStamp = des.getStamp();
  return m_accessRoot;
}

int WsFsDaemonClient::receivePermissions()
{
  string resp;
  if (receive( resp) == FAILURE)
    return GlobalConfig::NoAccess;
  if (resp == RequestField::Failure)
    return GlobalConfig::NoAccess;
  if (resp == "notlogged")
    return GlobalConfig::NotLogged;
  try {
    return boost::lexical_cast<int>(resp);
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsDaemonClient::receivePermissions() : Cannot cast received permissions to int. Possible transmission problem.";
    return GlobalConfig::NoAccess;
  }
  return GlobalConfig::NoAccess;
}

const WsNodeProperties* WsFsDaemonClient::receiveProperties()
{
  Value root;
  Reader reader;
  string resp;
  if (receive( resp) == FAILURE)
    return NULL;
  if (resp == RequestField::Failure || resp == "notlogged")
    return NULL;
  if (!reader.parse(resp, root, false))
    return NULL;
  WsNodeProperties* props = new WsNodeProperties(resp);
  return props;
}


const string WsFsDaemonClient::receiveProperty()
{
  Value root;
  Reader reader;
  string resp;
  if (receive( resp) == FAILURE)
    return "";
  if (resp == RequestField::Failure || resp == "notlogged")
    return "";
  return resp;
}

vector<WsResultItem> WsFsDaemonClient::receiveSearchResults()
{
  Value root;
  vector<WsResultItem> l;
  Reader reader;
  string resp;
  if (receive( resp) == FAILURE) {
    return l;
  }
  if (resp == RequestField::Failure || resp == "notlogged")
    return l;
  if (!reader.parse(resp, root, false))
    return l;
  /* Get the rootPath */
  path basePath(WsGlobalProperties::instance()->get("global", "root_path", "/"));
  /* For each item found create a ResultItem */
  for (int i = 0; i < root.size(); ++i) {
    path p(root[i][RequestField::Path].asString());
    /* Result item is like a WsNode, needs fullPath and rootPath */
    WsResultItem r(basePath / p, basePath, root[i][RequestField::Body].asString(), root[i][RequestField::Type].asString(), root[i][RequestField::Size].asInt());
    r.setModifyDate(root[i][RequestField::ModifyDate].asInt());
    l.push_back(r);
  }
  return l;
}

set<string> WsFsDaemonClient::receiveAllGroups()
{
  string resp;
  if (receive( resp) == FAILURE) {
    return set<string>();
  }
  WsArrayDeserializer s(resp);
  if ( s.deserialize() == FAILURE)
    return set<string>();
  return s.getContents();
}

bool WsFsDaemonClient::receiveBoolean()
{
  string resp;
  if (receive( resp) == FAILURE) {
    return false;
  }
  if (resp == "true")
    return true;
  return false;
}

int WsFsDaemonClient::receiveInt()
{
  string resp;
  if (receive( resp) == FAILURE) {
    return -1;
  }

  try{
    int ret = boost::lexical_cast<int>(resp);
    return ret;
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsDaemonClient::receiveInt() : Could not cast received data to int";
    return -1;
  }
}

const string WsFsDaemonClient::receiveString()
{
  Value root;
  Reader reader;
  string resp;
  if (receive(resp) == FAILURE)
    return "";
  if (resp == RequestField::Failure || resp == "notlogged")
    return "";
  return resp;
}


int WsFsDaemonClient::getPermissions(const string& p)
{
  Value v;
  v[RequestField::Type] = Perm;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  if (send(v.toStyledString()) == FAILURE)
    return GlobalConfig::NoAccess;
  return receivePermissions();
}

const WsNodeProperties* WsFsDaemonClient::getProperties(const string& p)
{
  Value v;
  v[RequestField::Type] = Properties;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  if (send(v.toStyledString()) == FAILURE) {
    LOG(DEBUG) << "WsAbstractProperties :: Could not send on socket";
    return NULL;
  }
  return receiveProperties();
}

string WsFsDaemonClient::getProperty(const std::string& section, const string& p, const string& prop)
{
  Value v;
  v[RequestField::Type] = Property;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  v[RequestField::Section] = section;
  v[RequestField::Property] = prop;
  if (send(v.toStyledString()) == FAILURE) {
    LOG(DEBUG) << "WsAbstractProperties::getProperty() : Could not send on socket";
    return NULL;
  }
  return receiveProperty();
}

const string& WsFsDaemonClient::getSurname() const
{
  return m_surname;
}

const string& WsFsDaemonClient::getFirstName() const
{
  return m_name;
}

const string& WsFsDaemonClient::getEmail() const
{
  return m_email;
}

vector<WsResultItem> WsFsDaemonClient::getSearchResults(const string& terms)
{
  Value v;
  v[RequestField::Type] = Search;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Terms] = terms;
  vector<WsResultItem> l;
  if (send( v.toStyledString()) == FAILURE)
    return l;
  return receiveSearchResults();
}

set<string> WsFsDaemonClient::getAllGroups()
{
  Value v;
  v[RequestField::Type] = AllGroups;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send(v.toStyledString()) == FAILURE)
    return set<string>();
  return receiveAllGroups();
}

int WsFsDaemonClient::saveProperties(WsNodeProperties* props, const string& p)
{
  Value v;
  v[RequestField::Type] = SaveProps;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  v[RequestField::Property] = props->getRoot();
  if (send( v.toStyledString()) == FAILURE)
    return FAILURE;
  return receiveSuccessCode();
}

int WsFsDaemonClient::saveProperty(const std::string& p, const std::string& section, const string& attr, const string& val)
{
  Value v;
  v[RequestField::Type] = SaveProp;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  v[RequestField::Section] = section;
  v[RequestField::Key] = attr;
  v[RequestField::Value] = val;
  if (send( v.toStyledString()) == FAILURE)
    return FAILURE;
  return receiveSuccessCode();
}

int WsFsDaemonClient::createNode(const string& p, int type)
{
  Value v;
  v[RequestField::Type] = CreateNode;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  v[RequestField::NodeType] = type;
  if (send( v.toStyledString()) == FAILURE)
    return FAILURE;
  if (receiveSuccessCode() == SUCCESS) {
    getAccessRoot(true);
    getMenuRoot( true);
    return SUCCESS;
  }
  return FAILURE;
}

int WsFsDaemonClient::deleteNode(const string& p)
{
  Value v;
  v[RequestField::Type] = DeleteNode;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  if (send(v.toStyledString()) == FAILURE)
    return FAILURE;
  if (receiveSuccessCode() == SUCCESS) {
    getAccessRoot(true);
    getMenuRoot(true);
    return SUCCESS;
  }
  return FAILURE;
}

int WsFsDaemonClient::renameNode(const string& p, const string& newPath)
{
  Value v;
  v[RequestField::Type] = RenameNode;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  v[RequestField::Path] = p;
  v[RequestField::NewPath] = newPath;
  if (send(v.toStyledString()) == FAILURE)
    return FAILURE;
  if (receiveSuccessCode() == SUCCESS) {
    getAccessRoot(true);
    getMenuRoot(true);
    return SUCCESS;
  }
}

bool WsFsDaemonClient::isEditor()
{
  if ( m_isEditor != -1)
    return m_isEditor == 0 ? false : true;
  Value v;
  v[RequestField::Type] = IsEditor;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send( v.toStyledString()) == FAILURE)
    return false;
  bool r = receiveBoolean();
  m_isEditor = r ? 1 : 0;
  return r;
}

bool WsFsDaemonClient::isAdministrator()
{
  if ( m_isAdmin != -1)
    return m_isAdmin == 0 ? false : true;
  Value v;
  v[RequestField::Type] = IsAdmin;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  if (send( v.toStyledString()) == FAILURE)
    return false;
  bool r = receiveBoolean();
  m_isAdmin = r ? 1 : 0;
  return r;
}

vector<string> WsFsDaemonClient::getTemplatesList(const string& path)
{
  //TODO
  return vector<string>();
}

string WsFsDaemonClient::rawDataToString(zmq::message_t& msg)
{
  string ret;
  if (m_compress) {
    m_decompressor->decompress(static_cast<char*>(msg.data()), msg.size(), ret);
  } else ret = string( static_cast<char*>(msg.data()), msg.size());
  return ret;
}

int WsFsDaemonClient::threadUpdate()
{
  Value v;
  v[RequestField::Type] = TreeVersion;
  v[RequestField::Uid] = m_uid;
  v[RequestField::Pass] = m_pass;
  v[RequestField::Ip] = m_ip;
  int delay;
  try {
    delay = boost::lexical_cast<int>(m_conf->get("global", "tree_check_delay", "120"));
  } catch (boost::bad_lexical_cast&) {
    LOG(ERROR) << "WsFsDaemonClient::threadUpdate() : Could not cast tree_check_delay to int. Assuming 120s";
    delay = 120;
  }
  //Loop and check if new tree exists every 'delay'. Retrieve it if updated
  while (m_listen) {
    try {
      //Socket is closed
      if (!m_sock || !m_listen)
        return SUCCESS;
      //Send the request
      if (send(v.toStyledString()) == FAILURE)
        return FAILURE;
      string v = receiveString();
      if ( v != m_accessTreeStamp )
        getAccessRoot(true);
      if ( v != m_menuTreeStamp )
        getMenuRoot(true);
      boost::this_thread::sleep(boost::posix_time::milliseconds(delay * 1000));
    } catch (std::exception& e) {
      LOG(ERROR) << "WsFsDaemonClient::threadUpdate :" << e.what();
      return FAILURE;
    }
  }
}

//#endif // HAS_DAEMON
