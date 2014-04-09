/*
 *
 *       Filename:  WsFsTreeConsultation.h
 *
 *    Description:
 *
 *        Created:  03/21/2013 04:04:50 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#ifndef WS_FSTREE_CONSULTATION_H__
#define WS_FSTREE_CONSULTATION_H__

#include <Tree/WsAccessTree.h>
#include <Tree/WsMenuTree.h>
#include <Search/WsResultItem.h>
#include <Updater/WsFsTreeUpdater.h>

#include <boost/thread/mutex.hpp>

/**
 * @brief Encapsulates consultation of tree (no modification)
 */
class WsFsTreeConsultation {

public:

  /**
   * @brief Constructor
   * @param updater the WsFsTreeUpdater instance
   */
  WsFsTreeConsultation(WsFsTreeUpdater* updater);

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
   * @brief Get the menu tree starting from rootPath
   *
   * @param groups the groups of the user to test access
   * @param rootPath the starting path of the menu, default is "/" and will return the menu tree starting from the root
   *
   * @return a WsMenuTree containing the nodes of the menu. if an error occured or no access, the root of the WsMenuTree will be NULL
   */
  WsMenuTree* getMenuTree( const std::set<std::string>& groups) ;


  /**
   * @brief Get the menu tree starting from rootPath and exclude files patterns and/or extensions
   *
   * @param groups the groups of the user to test access
   * @param rootPath the starting path of the menu, default is "/" and will return the menu tree starting from the root
   *
   * @return a WsMenuTree containing the nodes of the menu. if an error occured or no access, the root of the WsMenuTree will be NULL
   */
  WsMenuTree* getMenuTree( const std::set<std::string>& groups, const std::set<string>& exclNames, const std::set<string>& exclExt) ;


  /**
   * @brief tries to acquire the lock for the path. A lock file will be stored in the .config/locks of the current directory containing the timestamp of the lock and the uid of the user who locked it
   * @ return 0 if the lock cannot be aquired because is detented by someone else. -1 if an error occured and a positive value with the duration in seconds of the lock is returned otherwise.
   */
  int getLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @brief unlocks the file by deleting the .config/locks/filename.lock file
   * Only the owner of the lock can unlock the path.
   * @return 1 if the lock has been successfully released
   *         1 if no lock are currently hold on the path
   *         0 if the lock is owned by someone else
   *         -1 if an error occured
   */
  int putLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @brief check is the path is already locked
   * @return 1 if the path is unlocked
   * @return 0 if the file is locked and the uid of the user who locked it will be stored in uid
   * return -1 if an error occured
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
   * @brief Get the stamp of the last WsFileSystemTree
   *
   * @return  a string containing the stamp
   */
  const std::string& getFsTreeStamp();

private:
  WsFsTreeUpdater* m_updater;
  WsGlobalProperties* m_conf;
  boost::mutex m_lockEditMutex;

  long getTimeMs();


};



#endif
