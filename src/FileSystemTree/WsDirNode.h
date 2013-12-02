/**
 *
 *       @file  WsdirNode.h
 *
 *      @brief  A dir node
 *
 *    @version  1.0
 *       @date  01.07.2011 16:38:05
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_DIR_NODE_H__
#define WS_DIR_NODE_H__

#include "WsNode.h"
#include <ConfigManager/WsGlobalProperties.h>
#include <ConfigManager/WsNodeResources.h>

#include <iostream>

/**
 * @brief Reprensents a Directory on disk
 */
class WsDirNode: public WsNode {
public:
  /**
   * @brief Constructor for DirNode
   * @param fullPath the fullPath of the node
   * @param rootPath the rootPath (ie the directory of the site)
   */
  WsDirNode(const path& fullPath, const path& rootPath);

  WsDirNode();
  ~WsDirNode();

  /**
   * @copydoc WsAbstractNode::isAllowed()
   */
  bool isAllowed(const std::set<string>& gids);

  /**
   * @copydoc WsAbstractNode::getDisplayInParent()
   */
  bool getDisplayInParent();

  /**
   * @brief returns the list of the groups allowed to access this node
   * @return a set containing these groups
   */
  set<string> getGroups();

  /**
   * @brief get the resource of a node
   * Not implemented yet
   */
  std::vector<std::string> getResource(ResourceType t, bool recurse = false);

  void setResources(WsNodeResources* resources);

  /**
   * @brief return true
   **/
  bool isDirectory();

  /**
   * @brief return false
   **/
  bool isRegularFile();

  /* @brief if
  * the global property dir_size is set to true, the number of items in the node is returned (one level), otherwise 0
  **/
  const uintmax_t getSize();

private:
  /**
   * Contained the resources of the node
   */
  WsNodeResources* m_resources;

};
#endif
