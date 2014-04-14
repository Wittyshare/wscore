/*
 *
 *       Filename:  WsFsTreeOperations.cpp
 *
 *    Description:
 *
 *        Created:  31/03/13 15:33:40
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_FSTREE_OPERATIONS
#define WS_FSTREE_OPERATIONS

#include <Tree/WsAccessTree.h>
#include <Search/WsResultItem.h>
#include "WsFsTreeConsultation.h"
#include "WsFsTreeModification.h"


/**
 * @brief Redirect operations to adequate class
 * @see WsFsTreeConsultation
 * @see WsFsTreeModification
 *
 */
class WsFsTreeOperations {

public:
  WsFsTreeOperations();


  /**
   * @brief Get the permissions for a path for a set of groups
   *
   * @param groups the set of gids to test
   * @param p the path to the node (relative path)
   *
   * @return READ READ_WRITE NO_ACCESS
   */
  int getPermissions( const std::set<std::string>& groups, const std::string& p) ;


  /**
   * @brief get properties of a node corresponding to the path
   *
   * @param groups the groups to see if access is granted to the node
   * @param p the path to the node (relative path)
   *
   * @return a WsNodeProperties correcponding to the properties of the node or a NULL object if access is not granted or path not found
   */
  WsNodeProperties* getProperties ( const std::set<std::string>& groups, const std::string& p) ;

  /**
   * @brief get a property for a node corresponding to the path
   *
   * @param groups the groups to see if access is granted to the path
   * @param section the section of the property in the Json tree
   * @param prop the key of the property to retrieve
   * @param p the path to the node (relative path)
   *
   * @return  a string containing the property of a empty string if access is denied or path non existent
   */
  std::string getProperty( const std::set<std::string>& groups, const std::string& section, const std::string& p, const std::string& prop) ;

  /**
   * @brief tries to acquire the lock for the path.
   * @ return ErrorCode::Locked if the lock cannot be aquired because is detented by someone else. 
   *          ErrorCode::Failure if an error occured 
   *          A positive value with the duration in seconds of the lock is returned otherwise.
   * @param groups the groups of the user
   * @param uid the uid of the user
   * @param path the path to the node
   *
   */
  int getLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @brief unlocks the file by deleting the .config/locks/filename.lock file
   * Only the owner of the lock can unlock the path.
   * @return ErrorCode::Success if the lock has been successfully released
   *         ErrorCode::Success if no lock are currently hold on the path
   *         ErrorCode::Locked if the lock is owned by someone else
   *         ErrorCode::Failure if an error occured
   * @param groups the groups of the user
   * @param uid the uid of the user
   * @param path the path to the node
   */
  int putLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @brief check is the path is already locked
   * @return 1 if the path is unlocked
   * @return ErrorCode::Locked if the file is locked and the uid of the user who locked it will be stored in uid
   * return ErrorCode::Failure if an error occured
   * @param groups the groups of the user
   * @param uid the uid of the user
   * @param path the path to the node
   * @param id an empty string
   */
  int isLocked(const std::set<std::string> groups, const std::string& uid, const std::string& path, std::string& id);

  /**
   * @brief Get the access tree starting from rootPath
   *
   * @param groups the groups of the user to test access
   * @param rootPath the starting path of the access tree, default is "/" and will return the access tree starting from the root
   *
   * @return a WsMenuTree containing the nodes of the access tree. if an error occured or no access, the root of the WsAccessTree will be NULL
   */
  WsAccessTree* getAccessTree( const std::set<std::string>& groups) ;

  /**
   * @brief Get the results for searching for "terms"
   *
   * @param groups groups of the user to test access
   * @param terms the terms of the search
   *
   * @return a vector of WsResultItem. Each WsResultItem is a node with a small description. If no results, an empty vector is returned
   */
  std::vector<WsResultItem> getSearchResults( const std::set<std::string>& groups, const std::string& terms) ;
  const std::string getRootPath() ;

  /**
   * @brief TODO
   *
   * @param groups
   * @param path
   *
   * @return
   */
  std::vector<std::string> getTemplatesList( const std::set<std::string>& groups, const std::string& path) ;

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

  /**
   * @brief Update the WsFileSystemTree
   *
   * @return SUCCESS or FAILURE if update failed
   */
  int update();

  /**
   * @brief Get the stamp of the last WsFileSystemTree
   *
   * @return  a string containing the stamp
   */
  const std::string& getFsTreeStamp();

private:
  WsFsTreeUpdater* m_updater;
  WsFsTreeConsultation* m_consultation;
  WsFsTreeModification* m_modification;
  WsGlobalProperties* m_conf;

};

#endif
