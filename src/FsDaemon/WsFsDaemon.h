/**
 *
 *       @file  WsFsDaemon.h
 *
 *      @brief  Take charge of FS operations
 *
 *    @version  1.0
 *       @date  07/25/11 15:18:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_FSDAEMON_H__
#define WS_FSDAEMON_H__
#include <FileSystemTree/WsFileSystemTree.h>
#include <Updater/WsFsTreeUpdater.h>
#include <ConfigManager/WsGlobalProperties.h>
#include <Auth/WsAuthenticator.h>
#include <Compression/WsCompressor.h>
#include <Compression/WsDecompressor.h>
#include <FsTreeOp/WsFsTreeOperations.h>

#include "WsUserSession.h"

#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

#include <boost/lexical_cast.hpp>

#include <zmq.hpp>

#include <iostream>
#include <set>
#include <vector>

/**
 * @brief Main class of the FsDaemon
 *
 * Caches locally the FileSystemTree and wait for request and replies
 */
class WsFsDaemon {
public:

  enum DaemonStatus {
    Success,
    Failure,
    NotLogged,
    NoAccess
  };
  /**
   * @brief constructor for the server
   * @param props an instance of WsGlobalProperties
   */
  WsFsDaemon(WsGlobalProperties* props);

  /**
   * @brief destructor
   */
  ~WsFsDaemon();

  /**
   * @brief start listening on the port
   * @return DaemonStatus
   */
  DaemonStatus bind();

private:

  /**
   * @brief Sends data on the socket.
   * @param sock a zmq::socket_t
   * @param s the string to send
   * @return DaemonStatus
   **/
  DaemonStatus send(zmq::socket_t& sock, const std::string& s);

  /**
   * @brief read data from the socket
   * @param sock the zmq::socket_t
   * @param receivedData the string where the data read will be stored
   * @return DaemonStatus
   */
  DaemonStatus receive(zmq::socket_t& sock, std::string& receivedData);

  /**
   * @brief Parses the received data (json)
   * @param s the string to parse (Json)
   * @return DaemonStatus
   **/
  DaemonStatus parse(const std::string& s);

  /**
   * @brief Handle received request and redirect to correct method
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   **/
  DaemonStatus handleRequest(zmq::socket_t& sock);

  /**
   * @brief Authenticate user on server
   * Loads the groups which the user belongs to, uid, email, etc..
   * @see WsAbstractAuth
   **/
  DaemonStatus handleAuthRequest(zmq::socket_t& sock);

  /**
   * @brief Sends the menu to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleMenuRequest(zmq::socket_t& sock);

  /**
   * @brief Sends the menu to the client, the client requested a menu with exclusion of extensions
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleMenuRequestEx(zmq::socket_t& sock);

  /**
   * @brief Sends the accesstree to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleAccessTreeRequest(zmq::socket_t& sock);

  /**
   * @brief sends the permissions of a path to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handlePermRequest(zmq::socket_t& sock);

  /**
   * @brief sends a property of a node to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handlePropRequest(zmq::socket_t& sock);

  /**
   * @brief sends all the groups available to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleAllGroupsRequest(zmq::socket_t& sock);

  /**
   * @brief sends all the properties of a node
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handlePropsRequest(zmq::socket_t& sock);

  /**
   * @brief sends the result of a search request to the client. Only nodes where the user has access are returned
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleSearchRequest(zmq::socket_t& sock);

  /**
   * @brief clears the user from the cache
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleClearCache(zmq::socket_t& sock);

  /**
   * @brief sends the result of an admin request so that the client can know if he is an admin or not
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleIsAdminRequest(zmq::socket_t& sock);

  /**
   * @brief sends the result of an editor  request so that the client can know if he is an editor or not
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleIsEditorRequest(zmq::socket_t& sock);

  /**
   * @brief saves the properties received by the client for a node
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleSavePropertiesRequest(zmq::socket_t& sock);

  /**
   * @brief saves one property received by a client for a node
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleSavePropertyRequest(zmq::socket_t& sock);

  /**
   * @brief create a node requested by the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleCreateNodeRequest(zmq::socket_t& sock);

  /**
   * @brief rename a node
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleRenameNodeRequest(zmq::socket_t& sock);

  /**
   * @brief delete a node requested by the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleDeleteNodeRequest(zmq::socket_t& sock);

  /**
   * @brief send the rootPath to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleRootPathRequest(zmq::socket_t& sock);

  /**
   * @brief sends the last tree version to the client
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  DaemonStatus handleGetTreeVersion(zmq::socket_t& sock);

  /**
   * @brief converts raw data received on the socket to string
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  string rawDataToString(zmq::message_t& msg);

  /**
   * @brief returns true or false whether the section of a user is still active or not
   * @param sock the zmq::socket_t
   * @returns DaemonStatus
   */
  bool isActiveSession(const std::string& uid, const std::string& pass, const std::string& ip);

  /**
   * GlobalProperties
   */
  WsGlobalProperties* m_conf;

  /**
   * listening on socket ?
   */
  bool m_listening;

  /**
   * Contains the interface to all possible operations on tree
   */
  WsFsTreeOperations* m_operation;

  /**
   * Json root
   */
  Json::Value m_root;

  /**
   * Json reader
   */
  Json::Reader m_reader;

  /**
   * User's session map
   */
  map<string, WsUserSession* > m_userMap;

  /**
   * All groups available
   */
  set<string> m_allGroups;

  /**
   * Copress data or no ?
   */
  bool m_compress;

  /**
   * Object used for compression
   */
  WsCompressor* m_compressor;

  /**
   * Object used for decompressions
   */
  WsDecompressor* m_decompressor;
};

#endif
