/**
 *
 *       @file  WsAbstractTree.h
 *
 *      @brief  Reprensents the  tree of a group.
 *      If a group does not have access to a Node, this Node will not show in this tree.
 *      Abstract Class
 *
 *    @version  1.0
 *       @date  07/08/11 13:28:14
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_ABSTRACT_TREE_H__
#define WS_ABSTRACT_TREE_H__

#include <TreeTraversal/WsTreeTraversal.h>
#include <FileSystemTree/WsFileSystemTree.h>

/**
 * @brief Abstract class representing an WsAccessTree or WsMenuTree
 *
 * This class should be derived and is used as parent for WsAccessTree and WsMenuTree
 */

class WsAbstractTree: public WsTreeTraversal {

protected:

  /**
   * @brief Constructor for the Tree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */

  WsAbstractTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the Tree class
   * @param fs the fileSystemTree to use
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */
  WsAbstractTree(FileSystemTreePtr fs, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);

  /**
   * @brief Constructor for the Tree class
   * @param n the Root node of the tree
   * @param gid a std::set of group names/ids of the user
   * @param md max depth default is no max depth (0)
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */
  WsAbstractTree(NodePtr n, const std::set<std::string>& gid, const int& md, const path& rootPath, const std::string& stamp);


  /**
   * @brief Constructor for the Tree class
   * @param n the root node
   * @param gid a std::set of group names/ids of the user
   * @param names the excluded names
   * @param ext the excluded extensions
   * @param rootPath the root path
   * @param stamp the unique stamp of the tree
   */
  WsAbstractTree(NodePtr n, const std::set<std::string>& gid, const int& md, const std::set<std::string>& names, const std::set<std::string>& ext, const path& rootPath, const std::string& stamp);

  /**
   * @brief Destructor
   */
  ~WsAbstractTree();

  /**
   * @copydoc WsTreeTraversal::beginTraverseDir(NodePtr)
   */
  virtual int beginTraverseDir(NodePtr n);

  /**
   * @copydoc WsTreeTraversal::endTraverseDir(NodePtr)
   */
  virtual int endTraverseDir(NodePtr n);

  /**
   * @copydoc WsTreeTraversal::traverseFile(NodePtr)
   */
  virtual int traverseFile(NodePtr n);

  /**
   * @brief the root of the tree
   */
  NodePtr m_root;

  /**
   * @brief The current node (the current level):w
   */
  NodePtr m_current;

  /**
   * @brief the gids of the user
   */
  std::set<std::string> m_gid;

  /**
   * @brief The excluded names
   */
  std::set<std::string> m_exclNames;

  /**
   * @brief The excluded extensions
   */
  std::set<std::string> m_exclExt;

  /**
   * @brief The rootPath
   */
  path m_rootPath;


  /**
   * @brief The current depth of the tree (ie number of levels)
   */
  int m_currentDepth;


  /**
   * @brief The max depth of the tree (ie depth we don't want to exceed
   */
  int m_maxDepth;


  /**
   * @brief The stamp of the tree
   */
  std::string m_stamp;

public:
  /**
   * @brief Parses the fileSystemTree and builds the Tree containing
   * only the nodes where the user has access
   * @return SUCCESS if the tree was built without errors, FAILURE otherwise
   */
  virtual int build();

  /**
   * @brief Returns the root node of the menuTree
   * @return Pointer to the root of the menuTree
   */
  virtual NodePtr getRoot();

  /**
   * @brief get the root path
   *
   * @return boost::filesystem::path representing the rootPath
   */
  virtual path& getRootPath();

  /**
   * @brief Get the stamp of the tree
   *
   * @return A std::string containing the stamp
   */
  virtual const std::string& getStamp();

};

#endif
