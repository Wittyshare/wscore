/**
 *
 *       @file  WsFsDaemonClient.h
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
#ifndef WS_FS_DAEMON_CLIENT_H__
#define WS_FS_DAEMON_CLIENT_H__
#include "WsAbstractFsClient.h"
#include <zmq.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <Compression/WsCompressor.h>
#include <Compression/WsDecompressor.h>

#include <boost/thread.hpp>


/**
 * @brief Interface between the User class and the fsdaemon
 *
 * Used dynamcally when using FsDaemon
 */
class WsFsDaemonClient: public WsAbstractFsClient {
public:
  /**
   * @brief constructor
   * @param id: the user id
   */
  WsFsDaemonClient(const std::string& id, const std::string& pass, const std::string& ip);

  /**
   * @brief destructor
   **/
  ~WsFsDaemonClient();

  /**
   * @brief clears the server cache
   * (removes the ldap results cached
   */
  int clearServerCache();

  /**
   * @copydoc WsAbstractFsClient::load()
   */
  int load();

  /**
   * @copydoc WsAbstractFsClient::getPermissions()
   */
  int getPermissions(const std::string& p);

  /**
   * @copydoc WsAbstractFsClient::getProperties().get()
   */
  const WsNodeProperties* getProperties(const std::string& p);

  /**
   * @copydoc WsAbstractFsClient::getProperty()
   **/
  std::string getProperty(const std::string& section, const std::string& p, const std::string& prop);

  /**
   * @copydoc WsAbstractFsClient::getSurname()
   */
  const std::string& getSurname() const;

  /**
   * @copydoc WsAbstractFsClient::getFirstName()
   */
  const std::string& getFirstName() const;

  /**
   * @copydoc WsAbstractFsClient::getEmail()
   */
  const std::string& getEmail() const;

  /**
   * @copydoc WsAbstractFsClient::getMenuRoot()
   */
  NodePtr getMenuRoot(const bool& forceUpdate = false);
  
  /**
   * @copydoc WsAbstractFsClient::getLock();
   */
  int getLock(const std::string& path);
  
  /**
   * @copydoc WsAbstractFsClient::putLock();
   */
  int putLock(const std::string& path);

  /**
   * @copydoc WsAbstractFsClient::isLocked();
   */
  int isLocked(const std::string& path, std::string& uid);

  /**
   * @copydoc WsAbstractFsClient::getAccessRoot()
   */
  NodePtr getAccessRoot(const bool& forceUpdate = false);


  /**
   * @copydoc WsAbstractFsClient::getMenuRoot()
   */
  NodePtr getMenuRoot(const std::set<string>& exclNames, const std::set<string>& exclExti);


  /**
   * @copydoc WsAbstractFsClient::getSearchResults()
   **/
  vector<WsResultItem> getSearchResults(const std::string& terms);

  /**
   * @copydoc WsAbstractFsClient::getAllGroups()
   */
  std::set<string> getAllGroups();

  /**
   * @copydoc WsAbstractFsClient::getRootPath()
   **/
  const std::string getRootPath();

  /**
   * @copydoc WsAbstractFsClient::saveProperties()
   **/
  int saveProperties(WsNodeProperties* props, const std::string& path);

  /**
   * @copydoc WsAbstractFsClient::createNode()
   */
  int createNode(const string& path, int type);

  /**
   * @copydoc WsAbstractFsClient::deleteNode()
   */
  int deleteNode(const string& path);

  /**
   * @copydoc WsAbstractFsClient::renameNode()
   */
  int renameNode(const string& path, const string& newPath);

  /**
   * @copydoc WsAbstractFsClient::saveProperty()
   **/
  int saveProperty(const std::string& path, const std::string& section, const std::string& attr, const std::string& val);

  /**
   * @copydoc WsAbstractFsClient::isEditor()
   **/
  bool isEditor();

  /**
   * @copydoc WsAbstractFsClient::isAdministrator()
   **/
  bool isAdministrator();

  /**
   * @copydoc WsAbstractFsClient::getTemplateList()
   */
  std::vector<std::string> getTemplatesList(const std::string& path);


private:
  /**
   * @brief authenticate user. Sends an auth request to the daemon
   * @returns SUCCESS if user has been authenticated, FAILURE otherwise
   */
  int authentify();

  /**
   * @brief sends a string to the daemon
   * @returns SUCCESS if send succeeded, FAILURE otherwise
   * @param message the string to send
   */
  int send(const std::string&  message);

  /**
   * @brief receive a string from the daemon.
   * @return SUCCESS or FAILURE
   * @param receivedData a string where will be stored the received input
   */
  int receive(std::string& receivedData);

  /**
   * @brief receive the answer after performing an Auth request to the daemon
   * @return SUCCESS or FAILURE if user cannot be authenticated (due to network failure, software failure or if his not in the DB
   */
  int receiveAuthAnswer();

  /**
   * @brief receive the success code after a request that returns a SUCCESS or FAILURE answer only
   * @return SUCCESS or FAILURE
   */
  int receiveSuccessCode();

  /**
   * @brief receive the answer of a permission request
   * @see WsFsDaemonClient::getPermissions()
   * @return SUCCESS or FAILURE
   */
  int receivePermissions();

  /**
   * @brief receive a Boolean after a request that returns a boolean
   * @return true or false depending on the answer received
   */
  bool receiveBoolean();

  /**
   * @brief receive an int after a request that returns an int
   * @return an int
   */
  int receiveInt();

  /**
   * @brief receive a string after a request that returns a string
   * @return the received string or an empty string if failure
   */
  const std::string receiveString();

  /**
   * @brief receive the properties after a getProperties request
   * @see WsAbstractFsClient::getProperties().get().get()
   * @returns a WsNodeProperties object with the properties or a NULL object if failure
   */
  const WsNodeProperties* receiveProperties();

  /**
   * @brief receive a property after a getProperty request
   * @see WsAbstractFsClient::getProperty()
   * @returns the received property as a string or an empty string if failure
   */
  const std::string receiveProperty();

  /**
   * @brief receive the menu items after a getMenuRoot request
   * @see WsAbstractFsClient::getMenuRoot()
   */
  NodePtr receiveMenuItems();

  /**
   * @brief receive the access tree items after a getAccessRoot request
   * @see WsAbstractFsClient::getAccessRoot()
   */
  NodePtr receiveAccessItems();

  /**
   * @brief receive the search results after a getSearchResults() request
   * @see WsAbstractFsClient::getSearchResults()
   * @return a vector of result items or empty vector if no results
   */
  vector<WsResultItem> receiveSearchResults();

  /**
   * @brief receive all the groups after a getAllGroups() request
   * @see WsAbstractFsClient::getAllGroups()
   * @returns a set of gids or an empty set if no groups or failure
   */
  std::set<string> receiveAllGroups();

  /**
   * @brief Converts raw data received to string. This method should be used each time data is received from network to decode it and be sure everything has been read
   * @param msg the zmq::message_t receveived from the network
   * @returns the string decoded
   */
  std::string rawDataToString(zmq::message_t& msg);

  /**
   * @brief parse the received Json from the fsdaemon
   * @param s the received string from the fsdaemon
   * @return SUCCESS if parsed succeeded, FAILURE otherwise
   */
  int parse(const std::string& s);

  /**
   * @brief Launch the routine that will retrieve new AccessTree from fsdaemon
   * This method should be called in a different thread
   */
  int threadUpdate();

  std::string m_uid;
  std::string m_pass;
  std::string m_ip;
  std::string m_email;
  std::string m_name;
  std::string m_surname;

  std::string m_host;
  std::string m_port;
  std::string m_proto;
  WsGlobalProperties* m_conf;
  Json::Reader m_reader;
  Json::Value m_root;

  bool m_compress;
  NodePtr m_accessRoot;
  NodePtr m_menuRoot;
  std::string m_accessTreeStamp;
  std::string m_menuTreeStamp;
  std::string m_rootPath;
  int m_isAdmin;
  int m_isEditor;

  bool m_listen;
  zmq::socket_t* m_sock;
  WsCompressor* m_compressor;
  WsDecompressor* m_decompressor;
  static zmq::context_t* m_context;
  boost::mutex* m_sendMutex;
  boost::thread* m_updateThread;

};

#endif
//#endif
