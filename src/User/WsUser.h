/**
 *
 *       @file  WsUser.h
 *
 *      @brief  User class
 *
 *    @version  1.0
 *       @date  07/08/11 10:18:34
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef USER_H__
#define USER_H__

#include <iostream>
#include <set>

#include <FsClient/WsAbstractFsClient.h>
#include <FsClient/WsFsTreeClient.h>
#include <Logger/WsLogger.h>
#ifdef HAS_DAEMON
#include <FsClient/WsFsDaemonClient.h>
#endif //HAS_DAEMON



/**
 * @brief Interface that provides differents methods for accessing the FsTree as well as other features
 *
 * If the daemon is used it will instanciate a WsFsDaemonClient otherwise it will instanciate a WsFsTreeClient. All methods filter data accroding to user right before returning any data. (The test is not done in this class but in the instanciated class of WsAbstractFsClient)
 */
class WsUser {
public :
  enum NodeType { File, Directory };

  /**
   * @brief Constructor
   *
   * @param strUid the uid of the user
   * @param pass the password of the user
   * @param ip the ip of the user
   */
  WsUser(const std::string& strUid, const std::string& pass = "", const std::string& ip = "");

  /**
   * @brief Destructor.
   */
  ~WsUser();

  /**
   * @brief Load infos of the user
   *
   * Loads the name, surname, email, groups ..
   */
  int load();

  /**
   * @brief return the home page
   **/
  const std::string getHomePage();

  /**
   * @brief return the root node of the access tree starting from the root
   * @return NULL if the path is incorrect or if the user does not have access rights to it otherwise a NodePtr to the rootNode is returned
   */
  NodePtr getAccessRoot();

  /**
   * @brief return the root path of the filesystem tree, example : /var/www/demo_site
   */
  const std::string getRootPath();

  /**
   * @brief return the surname of the person
   * @return the surname or an empty string if not found
   */
  const std::string& getSurname() const;

  /**
   * @brief return the name of the person
   * @return the name or an empty string if not found
   */
  const std::string& getFirstName() const;

  /**
   * @brief return the email of the person
   * @return the email or an empty string if not found
   */
  const std::string& getEmail() const;

  /**
   * @brief returns the root of the menu. The root is an instance of WsDirNode
   * @param rootPath the path to the root
   * @return a NodePtr to the root or en empty one if no access, failure
   */
  NodePtr getMenuRoot();

  /**
   * @brief tries to acquire the lock for the path.
   * @ return 0 if the lock cannot be aquired because is detented by someone else. -1 if an error occured and a positive value with the duration in seconds of the lock is returned otherwise.
   */
  int getLock(const std::string& path);

  /**
   * @brief releases the lock for the path.
   * @return 0 if path cannot be unlocked, -1 if error and >0 if unlock successful
   */
  int putLock(const std::string& path);

  /**
   * @brief check is the path is already locked
   * @return 1 if the path is unlocked
   * @return 0 if the file is locked and the uid of the user who locked it will be stored in uid
   * return -1 if an error occured
   */
  int isLocked(const std::string& path, std::string& uid);

  /**
   * @brief returns the root of the menu. The root is an instance of WsDirNode. This is the same mehod as WsFsTreeClient::getMenuRoot() but it excluded specific nodes
   * @param exclNames the node name to excluded (name without path)
   * @param exclExt the node extension to exclude
   * The names and extensions are case sensitive
   * @param rootPath the path to the root
   * @return a NodePtr to the root or en empty one if no access, failure
   */
  NodePtr getMenuRoot(const std::set<std::string>& exclNames, const std::set<std::string>& exclExt);

  /**
   * @brief return the permissions on specific node
   * @param p the node path (relative)
   * @return READ if only read READ_WRITE if read write perm or NO_ACCESS
   */
  int getPermissions(const std::string& p);

  /**
   * @brief returns the properties of a node
   * @param p the path to the node
   * @return a NodePtr containing a  WsAbstractNode
   */
  const WsNodeProperties* getProperties(const std::string& p);

  /**
   * @brief returns one property of a node
   * If the property does not exist, a empty string is returned.
   * @param section the section of the property in the Json tree
   * @param attr the key of the property to retrieve
   * @param path the path to the node (relative path)
   * @param def the default value returned if not found
   *
   * @return  a string containing the property of a empty string if access is denied or path non existent
   **/
  std::string getProperty(const std::string& path, const std::string& section, const string& attr, const std::string& def);
  /**
   * @brief search for all matching results of terms using WsSearch class
   * @param terms the terms to match
   * results where found or if an error occured during the process.
   * @return a vector containing the results as WsResultItem or an empty vector if no results found
   **/
  std::vector<WsResultItem> getSearchResults(const std::string& terms);

  /*
   * @brief returns all the possible groups
   */
  std::set<std::string> getAllGroups();

  /**
   * @brief save the properties of the node on disk. The user must have access and edit rights for the node.
   * @param props the properties to save
   * @param path the path to the node (relative path)
   * @return SUCCESS if ok, FAILURE otherwise
   **/
  int saveProperties(WsNodeProperties* props, const std::string& path);

  /**
   * @brief sets one property of the node and save it on disk.
   * @param path the path to the node (relative path)
   * @param section the section of the property
   * @param attr the key of the property
   * @param val the value to set
   * @return SUCCESS if ok, FAILURE otherwise
   **/
  int saveProperty(const std::string& path, const std::string& section, const std::string& attr, const std::string& val);

  /**
   * @brief create a directory or File. If the node is a WsDirNode than
   * it will be only accessible to the Admin and the editor who created the node
   * @param path the relative path starting from root
   * @param type a NodeType enum { File, Directory }
   * @return SUCCESS if ok FAILURE if no access, path not found or error
   */
  int createNode(const string& path, NodeType type);

  /**
   * @brief delete a node. The user must be an Admin on editor to remove the node
   * In case of a WsDirNode, all the contents of the directory and the directory will be deleted
   * @param path the path to the node
   * @return SUCCESS if ok FAILURE if no access, path not found or error
   */
  int deleteNode(const string& path);

  /**
   * @brief renames a node
   * In case of a WsDirNode, all the contents of the directory and the directory will be moved
   * @param path the path to the node from the root
   * @param newPath new path from the root
   * @return SUCCESS if ok FAILURE if no access, path not found or error
   */
  int renameNode(const string& path, const string& newPath);

  /**
   * @return true if the actual user is an admin
   **/
  bool isAdministrator();

  /**
   * @returns true if actual user is an editor
   **/
  bool isEditor();

  /**
   * @brief return all the templates contained in a directory and parents
   * If two templates have the same name, the template stored in the deepest node will appear
   */
  std::vector<std::string> getTemplatesList(const std::string& path);

private:

  std::string cleanPath(const std::string& path);

  /**
   * @brief this will be an instance of WsFsTreeClient if not using daemon or WsFsDaemonClient if using daemon
   */
  WsAbstractFsClient* m_client;

  /**
   * @brief Use daemon or no
   */
  int m_isDaemon;
  std::string m_uid;
  std::string m_pass;
  std::string m_ip;
  WsGlobalProperties* m_conf;
};
#endif

