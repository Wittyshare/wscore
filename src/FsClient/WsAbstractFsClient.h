/*
 *
 *       Filename:  WsAbstractFsClient.h
 *
 *    Description:  File System Tree client
 *
 *        Version:  1.0
 *        Created:  07/25/2011 04:11:48 PM
 *
 */


#ifndef WS_ABSTRACT_FSCLIENT_H__
#define WS_ABSTRACT_FSCLIENT_H__
#include <iostream>
#include <vector>

#include <ConfigManager/WsNodeProperties.h>
#include <FileSystemTree/WsAbstractNode.h>
#include <Logger/WsLogger.h>
#include <Search/WsResultItem.h>

/**
 * @brief Abstract Interface between the WsUser class and the operation on the tree
 */
class WsAbstractFsClient {
public:

  virtual ~WsAbstractFsClient();

  /**
   * @brief loads the user info
   */
  virtual int load() = 0;
  /**
   * @brief get permission on the path for
   * the used with which the client has been
   * initialised
   * @returns mask of permissions
   */
  virtual int getPermissions(const std::string& p) = 0;
  /**
   * @brief get all properties of the
   * path passed in argument
   * @param p the path
   * @return the properties of the node
   */
  virtual const WsNodeProperties* getProperties (const std::string& p) = 0;

  /**
   * @brief returns one property of a node
   * If the property does not exist, a empty std::string is returned.
   * @param section the section of the property in the Json tree
   * @param prop the key of the property to retrieve
   * @param p the path to the node (relative path)
   *
   * @return  a string containing the property of a empty string if access is denied or path non existent
   **/
  virtual std::string getProperty(const std::string& section, const std::string& p, const std::string& prop) = 0;

  /**
   * @brief get the surname of the user
   */
  virtual const std::string& getSurname() const = 0;

  /**
   * @brief get the Name of the user
   */
  virtual const std::string& getFirstName() const = 0;

  /**
   * @brief get the email of the user
   */
  virtual const std::string& getEmail() const = 0;

  /**
   * @brief get the root node of the menu
   * The root node contains as children all the menu items
   * @param forceUpdate Force the update of the tree or no
   * @return a NodePtr to the root or en empty one if no access, failure
   */
  virtual NodePtr getMenuRoot( const bool& forceUpdate = false) = 0;

  /**
   * @brief return the root node of the access tree
   * @return NULL if the user does not have access to the root or if an error occured
   * @param rootPath the path to the root
   * @param forceUpdate Force the update of the tree or no
   * @return a NodePtr to the root or en empty one if no access, failure
   */
  virtual NodePtr getAccessRoot( const bool& forceUpdate = false) = 0;

  /**
   * @brief returns the root of the menu. The root is an instance of WsDirNode. This is the same mehod as WsFsTreeClient::getMenuRoot() but it excluded specific nodes
   * @param exclNames the node name to excluded (name without path)
   * @param exclExt the node extension to exclude
   * @param rootPath the path to the root
   * The names and extensions are case sensitive
   * @return a NodePtr to the root or en empty one if no access, failure
   */
  virtual NodePtr getMenuRoot(const std::set<string>& exclNames, const std::set<string>& exclExt) = 0;


  /**
   * @brief  get search results,
   * passing the text that is searched
   * @param terms the term to search
   * @return a vector containing the results as WsResultItem or an empty vector if no results found
   */
  virtual std::vector<WsResultItem> getSearchResults(const std::string& terms) = 0;

  /**
   * @brief returns all possible groups
   */
  virtual std::set<string> getAllGroups() = 0;

  /**
   * @brief get the root path
   **/
  virtual const std::string getRootPath() = 0;

  /**
   * @brief save the properties of the node. The user must have access and edit rights for the node.
   * @param props the properties to save
   * @param path the path to the node (relative path)
   * @return SUCCESS if ok, FAILURE otherwise
   **/
  virtual int saveProperties(WsNodeProperties* props, const std::string& path) = 0;

  /**
   * @brief sets one property of the node and save it.
   * @param path the path to the node (relative path)
   * @param section the section of the property
   * @param attr the key of the property
   * @param val the value to set
   * @return SUCCESS if ok, FAILURE otherwise
   **/
  virtual int saveProperty(const std::string& path, const std::string& section, const std::string& attr, const std::string& val) = 0;

  /**
   * @brief create a directory or File. If the node is a WsDirNode than
   * it will be only accessible to the Admin and the editor who created the node
   * @param path the relative path starting from root
   * @param type a NodeType enum { File, Directory }
   * @return SUCCESS is ok FAILURE otherwise
   */
  virtual int createNode(const string& path, int type) = 0;

  /**
   * @brief delete a node. The user must be an Admin on editor to remove the node
   * In case of a WsDirNode, all the contents of the directory and the directory will be deleted
   * @param path the path to the node
   */
  virtual int deleteNode(const string& path) = 0;

  /**
   * @brief renames a node
   * In case of a WsDirNode, all the contents of the directory and the directory will be moved
   * @param path the path to the node from the root
   * @param newPath new path from the root
   */
  virtual int renameNode(const string& path, const string& newPath) = 0;

  /**
   * @return true if the actual user is an Administrator
   **/
  virtual bool isAdministrator() = 0;

  /**
   * @return true if actual user is an editor
   **/
  virtual bool isEditor() = 0;

  /**
   * @brief return all the templates contained in a directory and parents
   * If two templates have the same name, the template stored in the deepest node will appear
   */
  virtual std::vector<std::string> getTemplatesList(const std::string& path) = 0;


};

#endif
