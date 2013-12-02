/**
 *
 *       @file  WsAccessTree.h
 *
 *      @brief  Reprensents the menu tree of a group.
 *      If a group does not have access to a Node, this Node will not show in this tree.
 *
 *    @version  1.0
 *       @date  07/08/11 13:28:14
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_ACCESS_TREE_H__
#define WS_ACCESS_TREE_H__

#include "WsAbstractTree.h"

/**
 * @brief Reprensents an access tree
 *
 * The access tree contains all the items the user has access to
 */
class WsAccessTree: public WsAbstractTree {
public:
  /**
   * @brief Constructor for the AccessTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   *
   */
  WsAccessTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the AccessTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   */
  WsAccessTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the AccessTree class
   * @param n the Root node
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   *
   */
  WsAccessTree(NodePtr n, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the AccessTree class
   * @param n the root node
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   */
  WsAccessTree(NodePtr n, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);
};

#endif
