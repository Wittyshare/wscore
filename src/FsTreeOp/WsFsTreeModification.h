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
   * @copydoc WsFsTreeOperations::saveProperties()
   */
  int saveProperties( const std::set<std::string>& groups, const std::string& json, const std::string& path) ;

  /**
   * @copydoc WsFsTreeOperations::saveProperty()
   */
  int saveProperty( const std::set<std::string>& groups, const std::string& path, const std::string& section, const std::string& attr, const std::string& val) ;

  /**
   * @copydoc WsFsTreeOperations::createNode()
   */
  int createNode( const std::set<std::string>& groups, const std::string& uid, const string& path, int type) ;

  /**
   * @copydoc WsFsTreeOperations::deleteNode()
   */
  int deleteNode( const std::set<std::string>& groups, const std::string& uid, const string& path) ;

  /**
   * @copydoc WsFsTreeOperations::renameNode()
   */
  int renameNode( const std::set<std::string>& groups, const std::string& uid, const string& path, const string& newPath) ;

  /**
   * @copydoc WsFsTreeOperations::isAdministrator()
   **/
  bool isAdministrator( const std::set<std::string>& groups);

  /**
   * @copydoc WsFsTreeOperations::isEditor()
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
