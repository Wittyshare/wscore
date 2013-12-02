/**
 *
 *       @file  WsTreeTraversal.h
 *
 *      @brief  Traverses the fileSystemTree
 *
 *    @version  1.0
 *       @date  07/08/11 10:18:34
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_TREE_TRAVERSAL_H__
#define WS_TREE_TRAVERSAL_H__
#include <FileSystemTree/WsAbstractNode.h>
#include <FileSystemTree/WsNode.h>
#include <FileSystemTree/WsFileNode.h>
#include <FileSystemTree/WsDirNode.h>

#include <list>
#include <iostream>



/**
 * @brief Used to traverse the tree.
 *
 * It uses DFS traversal
 */
class WsTreeTraversal {
public:
  /**
   * @brief constructor
   */
  WsTreeTraversal(NodePtr root);

  /**
   * @brief start traversing the Tree.
   */
  int start();
private:

  /**
   * @brief  Traverses a Node.
   * Calls the WsTreeTraversal::traverseDirNode() or WsTreeTraversal::traverseFileNode() depending on type of node
   *
   * @param n A NodePtr containing the Node
   *
   * @return SUCCESS or FAILURE
   */
  int traverse(NodePtr n);

  /**
   * @brief Traverses WsDirNodes recusively
   *
   * @param l
   *
   * @return
   */
  int traverseDirNodes(std::vector<NodePtr> l);


  /**
   * @brief Traverse a file and calls WsTreeTraversal::traverseFile() method
   *
   * @param l A NodePtr containing the WsFileNode
   *
   * @return SUCCESS or FAILURE
   */
  int traverseFileNodes(std::vector<NodePtr> l);

  /**
   * @brief Called when a directory is being traversed
   *
   * @param n the NodePtr representing the directory being traversed
   *
   * @return SUCCESS or FAILURE if an error occured
   */
  virtual int beginTraverseDir(NodePtr n) = 0;

  /**
   * @brief  Called when a directory has been completely traversed. ie all subnodes(recursively all sub nodes of subnodes etc..) have been traversed
   *
   * @param n The NodePtr representing the directory that has been completely traversed
   *
   * @return SUCCESS or FAILURE
   */
  virtual int endTraverseDir(NodePtr n) = 0;

  /**
   * @brief Called when traversing a fileNode
   *
   * @param n A NodePtr representing the file being traversed
   *
   * @return SUCCESS or FAILURE
   */
  virtual int traverseFile(NodePtr n) = 0;
protected:
  NodePtr m_root;


};

#endif
