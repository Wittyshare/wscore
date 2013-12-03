/**
 *
 *       @file  WsFsDaemon.cpp
 *
 *      @brief  Take charge of FS operations
 *
 *    @version  1.0
 *       @date  07/25/11 15:18:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFsDaemon.h"

#include <Include/WsGlobalConfig.h>
#include <Tree/WsMenuTree.h>
#include <Tree/WsAccessTree.h>
#include <Serializer/WsArraySerializer.h>
#include <Serializer/WsTreeSerializer.h>
#include <Logger/WsLogger.h>
#include <Search/WsSearch.h>
#include <Include/WsRequestType.h>

using namespace boost;
using namespace zmq;
using namespace Json;

WsFsDaemon::WsFsDaemon(WsGlobalProperties* props):
  m_listening(true),
  m_conf(props)
{
  if (m_conf->get("global", "enable_compression", "true") == "true") {
    m_compress = true;
    m_compressor = new WsCompressor();
    m_decompressor = new WsDecompressor();
    LOG(INFO) << "WsFsDaemon::WsFsDaemon() : Compression is enabled";
  } else m_compress = false;
}

WsFsDaemon::~WsFsDaemon()
{
  /* Stop listenning */
  m_listening = false;
  delete m_operation;
  delete WsLogWriter::instance();
}

WsFsDaemon::WsFsDaemon::DaemonStatus WsFsDaemon::bind()
{
  /* Instanciate object used for operations on tree */
  m_operation = new WsFsTreeOperations();
  if ( m_operation->update() == FAILURE) {
    LOG(ERROR) << "WsFsDaemon::bind() : Could not update WsFileSystemTree";
    return Failure;
  }
  /* Init socket related data and socket */
  context_t context(1);
  socket_t sock(context, ZMQ_REP);
  string protocol = m_conf->get("global", "protocol", "tcp");
  string host = m_conf->get("global", "host", "127.0.0.1");
  string port = m_conf->get("global", "port", "5555");
  try {
    LOG(INFO) << "WsFsDaemon::bind() : Binding server using " << protocol << " on " << host << ":" << port;
    sock.bind((protocol + "://" + host + ":" + port).c_str());
  } catch (zmq::error_t err) {
    LOG(ERROR) << "WsFsDaemon::bind() : Cannot bind socket : " << err.what();
    return Failure;
  }
  LOG(INFO) << "WsFsDaemon::bind() : Binding server Success !";
  /* Load All groups */
  WsAuthenticator auth;
  m_allGroups = auth.getAllGroups();
  /* Start infinite loop that will wait for connections */
  while (m_listening) {
    try {
      string data;
      /* Receive data from socket */
      if (receive(sock, data) == Failure)
        return Failure;
      /* Parse data (Json) */
      if (parse(data) == Failure) {
        send(sock, RequestField::Failure);
        continue;
      }
      /* handle the request depending on type */
      int ret = handleRequest(sock);
      /* If return code is error or not logged send a message */
      switch (ret) {
      case Failure:
        send(sock, RequestField::Failure);
        break;
      case NotLogged:
        send(sock, "notlogged");
        break;
      }
    } catch (zmq::error_t err) {
      LOG(ERROR) << "WsFsDaemon::bind() : Error listening " << err.what();
    }
  }
}

WsFsDaemon::DaemonStatus WsFsDaemon::send(socket_t& sock, const string& s)
{
  try {
    /* Compress data ? */
    if (m_compress) {
      char* data;
      long r = m_compressor->compress(s, &data);
      if (r == FAILURE)
        return Failure;
      /* Create message */
      message_t reply(r);
      if (memcpy ((void*) reply.data (), data, r) == NULL) {
        LOG(ERROR) << "WsFsDaemon::send() : Could not memcpy in Server";
        return Failure;
      }
      delete[] data;
      /* Send message */
      if (sock.send (reply) < 0) {
        LOG(ERROR) << "WsFsDaemon::send() : Could not send on socket ";
        return Failure;
      }
    } else {
      /* No compression, create and send message */
      message_t reply(s.length());
      if (memcpy ((void*) reply.data (), s.c_str(), s.length()) == NULL) {
        LOG(ERROR) << "WsFsDaemon::send() : Could not memcpy in Server";
        return Failure;
      }
      if (sock.send (reply) < 0) {
        LOG(ERROR) << "WsFsDaemon::send() : Could not send on socket ";
        return Failure;
      }
    }
  } catch (zmq::error_t err) {
    LOG(ERROR) << "WsFsDaemon::send() : Error sending " << err.what();
    return Failure;
  }
  return Success;
}

WsFsDaemon::DaemonStatus WsFsDaemon::receive(socket_t& sock, string& receivedData)
{
  try {
    message_t request;
    if (sock.recv (&request) < 0) {
      LOG(ERROR) << "WsFsDaemon::receive() : Could not receive on socket";
      return Failure;
    }
    receivedData = rawDataToString(request);
  } catch (zmq::error_t err) {
    LOG(ERROR) << "WsFsDaemon::send() : Error sending " << err.what();
    return Failure;
  }
  /* Parse the received request */
  return Success;
}

WsFsDaemon::DaemonStatus WsFsDaemon::parse(const string& s)
{
  //Clear old Json tree
  m_root.clear();
  if (!m_reader.parse(s, m_root, false)) {
    LOG(ERROR) << "WsFsDaemon::parse() : Could not parse received input";
    return Failure;
  }
  return Success;
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleRequest(socket_t& sock)
{
  Value v = m_root[RequestField::Type];
  if (v == Value::null) {
    LOG(ERROR) << "WsFsDaemon::handleRequest() : Could not find request type";
    return Failure;
  }
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string pass = m_root[RequestField::Pass].asString();
  string ip = m_root[RequestField::Ip].asString();
  int requestType = v.asInt();
  LOG(INFO) << "WsFsDaemon::handleRequest() : Received request : " << requestType << " from user " << uid << " ip " << ip;
  /* Authenticate request */
  if (requestType == Auth)
    return handleAuthRequest(sock);
  /* Check if user is logged because all other requests needs authenticated user*/
  if (!isActiveSession(uid, pass, ip)) {
    LOG(ERROR) << "WsFsDaemon::handleRequest() :  User is not logged " << uid << " ip " << ip << " request : " << requestType;
    return NotLogged;
  }
  switch (requestType) {
    /* Permission request */
  case Perm:
    return handlePermRequest(sock);
    /* Get all groups request */
  case AllGroups:
    return handleAllGroupsRequest(sock);
    /* Get properties of a node request */
  case Properties:
    return handlePropsRequest(sock);
    /* Get a property of a node request */
  case Property:
    return handlePropRequest(sock);
    /* Search request */
  case Search:
    return handleSearchRequest(sock);
    /* Clear cache request */
  case ClearCache:
    return handleClearCache(sock);
    /* Get menu item request */
  case MenuItems:
    return handleMenuRequest(sock);
    /* Get menu item with exclusions request */
  case MenuItemsEx:
    return handleMenuRequestEx(sock);
    /* Get access tree items */
  case AccessItems:
    return handleAccessTreeRequest(sock);
    /* Get isEditor request */
  case IsEditor:
    return handleIsEditorRequest(sock);
    /* Get isAdmin request */
  case IsAdmin:
    return handleIsAdminRequest(sock);
  case RootPath:
    return handleRootPathRequest(sock);
    /* Get tree version request */
  case TreeVersion:
    return handleGetTreeVersion(sock);
    /* Save properties of a node request */
  case SaveProps:
    return handleSavePropertiesRequest(sock);
    /* Save a property of a node request */
  case SaveProp:
    return handleSavePropertyRequest(sock);
    /* Create a node request */
  case CreateNode:
    return handleCreateNodeRequest(sock);
    /* Delete a node request */
  case DeleteNode:
    return handleDeleteNodeRequest(sock);
    /* Rename a node request */
  case RenameNode:
    return handleRenameNodeRequest(sock);
    /* get RootPath request */
  default:
    LOG(ERROR) << "WsFsDaemon::handleRequest() : Undefined request type " << requestType;
    return Failure;
  }
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleAuthRequest(socket_t& sock)
{
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string pass = m_root[RequestField::Pass].asString();
  string ip = m_root[RequestField::Ip].asString();
  /* Check if there's already an active session for the user */
  if (isActiveSession(uid, pass, ip)) {
    /* If yes retrieve it from the cache */
    WsUserSession* usess = m_userMap[uid];
    LOG(DEBUG) << "WsFsDaemon::handleAuthRequest() : Session active for " << uid;
    Value answer;
    answer[RequestField::Uid] = uid;
    answer[RequestField::Name] = usess->getName();
    answer[RequestField::Surname] = usess->getSurname();
    answer[RequestField::Email] = usess->getEmail();
    return send(sock, answer.toStyledString());
  }
  LOG(INFO) << "WsFsDaemon::handleAuthRequest() : Received authentification request from user " << uid;
  /* No active session, we need to authenticate the user */
  WsAuthenticator c;
  if (c.authentify(uid, pass, ip) == FAILURE)
    return Failure;
  /* Create message */
  Value answer;
  answer[RequestField::Uid] = c.getUid();
  answer[RequestField::Name] = c.getFirstName();
  answer[RequestField::Surname] = c.getSurname();
  answer[RequestField::Email] = c.getEmail();
  set<string> groups = c.getUserGroups();
  /* Insert uid in groups User is part of his group*/
  groups.insert(c.getUid());
  WsUserSession* sess =  new WsUserSession(c.getUid(), pass, ip);
  sess->setName(c.getFirstName());
  sess->setSurname(c.getSurname());
  sess->setEmail(c.getEmail());
  sess->setGroups(c.getUserGroups());
  m_userMap[c.getUid()] = sess;
  LOG(INFO) << "WsFsDaemon::handleAuthRequest() : new uid is " << c.getUid();
  return send(sock, answer.toStyledString());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleMenuRequest(socket_t& sock)
{
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  set<string> groups = m_userMap[uid]->getGroups();
  /* Create menuTree */
  WsMenuTree* tree = m_operation->getMenuTree(groups);
  if (tree == 0)
    return Failure;
  /* Transform menuTree to json */
  WsTreeSerializer serializer(tree);
  serializer.serialize();
  delete tree;
  return send(sock, serializer.getSerializedForm());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleMenuRequestEx(socket_t& sock)
{
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  Value names;
  Value ext;
  set<string> groups = m_userMap[uid]->getGroups();
  /* Get the 2 vectors */
  set<string> exclNames, exclExt;
  names = m_root[RequestField::ExclNames];
  for (int i = 0; i < names.size(); ++i)
    exclNames.insert(names[i].asString());
  ext = m_root[RequestField::ExclExt];
  for (int i = 0; i < ext.size(); ++i)
    exclExt.insert(ext[i].asString());
  /* Create menuTree */
  WsMenuTree* tree = m_operation->getMenuTree(groups, exclNames, exclExt);
  if (tree == 0)
    return Failure;
  /* Transform menuTree to json */
  WsTreeSerializer serializer(tree);
  serializer.serialize();
  delete tree;
  return send(sock, serializer.getSerializedForm());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleAccessTreeRequest(socket_t& sock)
{
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  /* Get the groups of the user */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Create AccessTree */
  WsAccessTree* tree = m_operation->getAccessTree(groups);
  if (tree == 0)
    return Failure;
  /* Transform menuTree to json */
  WsTreeSerializer serializer(tree);
  serializer.serialize();
  delete tree;
  return send(sock, serializer.getSerializedForm());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handlePermRequest(socket_t& sock)
{
  /* get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  /* get users groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Get permissions on node for user */
  int perms = m_operation->getPermissions(groups, path);
  if ( perms != FAILURE)
    return send(sock, boost::lexical_cast<string>(perms));
  return Failure;
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleAllGroupsRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  /* Serialize the groups */
  WsArraySerializer s(m_allGroups);
  if (send(sock, s.getSerializedForm()) == FAILURE)
    return Failure;
  return Success;
}

WsFsDaemon::DaemonStatus WsFsDaemon::handlePropsRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  int i;
  WsNodeProperties* answer;
  /* Get groups of user */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Get properties of node */
  answer = m_operation->getProperties(groups, path);
  if (answer == 0)
    return Failure;
  return send(sock, answer->getRoot().toStyledString());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handlePropRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string p = m_root[RequestField::Path].asString();
  string section = m_root[RequestField::Section].asString();
  string prop = m_root[RequestField::Property].asString();
  int i;
  string answer;
  set<string> groups = m_userMap[uid]->getGroups();
  /* Get the node */
  answer = m_operation->getProperty(groups, section, p, prop);
  return send(sock, answer);
}
WsFsDaemon::DaemonStatus WsFsDaemon::handleSearchRequest(socket_t& sock)
{
  /* Get credentials */
  Value answer;
  string uid = m_root[RequestField::Uid].asString();
  string terms = m_root[RequestField::Terms].asString();
  /* Get the groups of user */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Get the search results */
  vector<WsResultItem> l = m_operation->getSearchResults(groups, terms);
  vector<WsResultItem>::iterator it;
  int i = 0;
  /* Build the Json answer */
  for (it = l.begin(); it != l.end(); ++it, ++i) {
    answer[i][RequestField::Path] = (*it).getPath().string();
    answer[i][RequestField::Body] = (*it).getBody();
    answer[i][RequestField::Type] = (*it).getType();
    answer[i][RequestField::Size] = (double)(*it).getSize();
    answer[i][RequestField::ModifyDate] = (double)(*it).getModifyDate();
  }
  return send(sock, answer.toStyledString());
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleClearCache(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  if (m_userMap.count(uid) > 0)
    delete m_userMap[uid];
  m_userMap.erase(uid);
  return send(sock, RequestField::Success);
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleIsEditorRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  /* Get user groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Check if he is editor and send result */
  if (m_operation->isEditor(groups))
    return send(sock, "true");
  else
    return send(sock, "false");
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleIsAdminRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  /* Get the groups of the user */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Check if he is an admin and send result */
  if (m_operation->isAdministrator(groups))
    return send(sock, "true");
  else
    return send(sock, "false");
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleSavePropertiesRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  /* Get groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Save */
  if (m_operation->saveProperties(groups, m_root[RequestField::Property].toStyledString(), path) != FAILURE)
    return send(sock, RequestField::Success);
  return Failure;
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleSavePropertyRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  string section = m_root[RequestField::Section].asString();
  string key = m_root[RequestField::Key].asString();
  string value = m_root[RequestField::Value].asString();
  /* get groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Save */
  if (m_operation->saveProperty(groups, path, section, key, value) != FAILURE)
    return send(sock, RequestField::Success);
  return Failure;
}


WsFsDaemon::DaemonStatus WsFsDaemon::handleCreateNodeRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  int type = m_root[RequestField::NodeType].asInt();
  /* Get groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* Create node */
  if (m_operation->createNode(groups, uid, path, type) == FAILURE)
    return Failure;
  return send(sock, RequestField::Success);
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleDeleteNodeRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  /* Get groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* delete node */
  if (m_operation->deleteNode(groups, uid, path) == FAILURE)
    return Failure;
  return send(sock, RequestField::Success);
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleRenameNodeRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string path = m_root[RequestField::Path].asString();
  string newPath = m_root[RequestField::NewPath].asString();
  /* Get groups */
  set<string> groups = m_userMap[uid]->getGroups();
  /* rename node */
  if (m_operation->renameNode(groups, uid, path, newPath) == FAILURE)
    return Failure;
  return send(sock, RequestField::Success);
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleRootPathRequest(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  /* get rootPath */
  string root = m_operation->getRootPath();
  return send(sock, root);
}

WsFsDaemon::DaemonStatus WsFsDaemon::handleGetTreeVersion(socket_t& sock)
{
  /* Get credentials */
  string uid = m_root[RequestField::Uid].asString();
  string version = m_operation->getFsTreeStamp();
  return send(sock, version);
}

string WsFsDaemon::rawDataToString(zmq::message_t& msg)
{
  string ret;
  if (m_compress) {
    m_decompressor->decompress(static_cast<char*>(msg.data()), msg.size(), ret);
  } else ret =  string(static_cast<char*>(msg.data()), msg.size());
  return ret;
}

bool WsFsDaemon::isActiveSession(const std::string& uid, const std::string& pass, const std::string& ip)
{
  if ( m_userMap.count(uid) > 0)
    return m_userMap[uid]->isValidSession(uid, pass, ip);
  return false;
}
