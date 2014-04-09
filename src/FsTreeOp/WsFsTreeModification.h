/*
 *
 *       Filename:  WsFsTreeModification.h
 *
 *    Description:
 *
 *        Created:  03/21/2013 04:18:54 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#ifndef WS_FSTREE_MODIFICATION_H__
#define WS_FSTREE_MODIFICATION_H__

#include <Tree/WsAccessTree.h>
#include <Tree/WsMenuTree.h>
#include <Search/WsResultItem.h>
#include <Updater/WsFsTreeUpdater.h>

/**
 * @brief Encapsulates modification operations on tree
 */
class WsFsTreeModification {
public:

  /**
   * @brief Constructor
   *
   * @param updater a WsFsTreeUpdater instance
   */
  WsFsTreeModification(WsFsTreeUpdater* updater);

  /**
   * @brief save the properties of the node. The user must have access and edit rights for the node.
   *
   * @param groups the groups of the user to test access
   * @param json a string containing the Json file contents
   * @param path the path to the node
   *
   * @return SUCCESS or FAILURE if no access, non exitent path, invalid Json
   */
  int saveProperties( const std::set<std::string>& groups, const std::string& json, const std::string& path) ;

  /**
   * @brief save a property of the node. The user must have access and edit rights for the node.
   *
   * @param groups the groups of the user to test access
   * @param path the path to the node
   * @param section the section of the property in the Json file
   * @param attr the key of the property
   * @param val the value to set
   *
   * @return SUCCESS or FAILURE if no access, path not found
   */
  int saveProperty( const std::set<std::string>& groups, const std::string& path, const std::string& section, const std::string& attr, const std::string& val) ;

  /**
   * @brief create a directory or File. If the node is a WsDirNode than
   * it will be only accessible to the Admin and the editor who created the node
   * @param groups the groups of the user to test access to parent dir
   * @param uid uid of user
   * @param path path to the new node
   * @param type type of node 0 for file 1 for directory
   *
   * @return  SUCCESS or FAILURE if no access, node already existing or failure
   */
  int createNode( const std::set<std::string>& groups, const std::string& uid, const string& path, int type) ;

  /**
   * @brief delete a node. The user must be an Admin on editor to remove the node
   *
   * @param groups the groups of the user
   * @param uid the uid of the user
   * @param path the path to the node
   *
   * @return SUCCESS or FAILURE if no access, invalid path
   */
  int deleteNode( const std::set<std::string>& groups, const std::string& uid, const string& path) ;

  /**
   * @brief renames a node
   *
   * @param groups the groups of the user
   * @param uid the uid of the user
   * @param path the path to the node
   * @param newPath the new path
   *
   * @return SUCCESS or FAILURE if no access, invalid path, invalid new path
   */
  int renameNode( const std::set<std::string>& groups, const std::string& uid, const string& path, const string& newPath) ;


  /**
   * @return true if the actual user is an Administrator
   * @param groups the groups of the user
   **/
  bool isAdministrator( const std::set<std::string>& groups);

  /**
   * @return true if actual user is an editor
   * @param groups the groups of the user
   **/
  bool isEditor( const std::set<std::string>& groups );


private:
  /**
   * @brief return true if the user can edit false otherwise
   * @return true if the user can edit false otherwise
   * @param node The node to test edit
   * @param groups the user's groups
   */
  bool canEdit(NodePtr node, std::set<std::string> groups);

  WsFsTreeUpdater* m_updater;
  WsGlobalProperties* m_conf;




};

#endif
