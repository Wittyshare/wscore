/**
 *
 *       @file  WsMenuTree.h
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
#ifndef WS_MENU_TREE_H__
#define WS_MENU_TREE_H__

#include "WsAbstractTree.h"



/**
 * @brief Reprensents a MenuTree
 *
 * The tree will contain all the item the user has access to and that should be displayed in menu (ie getDisplayInMenu is true)
 */
class WsMenuTree: public WsAbstractTree {

public:

  /**
   * @brief Constructor for the MenuTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   *
   */
  WsMenuTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the MenuTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */
  WsMenuTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the MenuTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */
  WsMenuTree(NodePtr n, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the MenuTree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   *
   */
  WsMenuTree(NodePtr n, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);

private:

  /**
   * Adds the node to the tree if it's getDisplayInMenu is true
   *
   * @copydoc WsAbstractTree::beginTraverseDir(NodePtr)
   */
  int beginTraverseDir(NodePtr n);
  /**
   * Adds the node to the tree if it's getDisplayInMenu is true
   *
   * @copydoc WsAbstractTree::traverseFile(NodePtr)
   */
  int traverseFile(NodePtr n);

};

#endif
