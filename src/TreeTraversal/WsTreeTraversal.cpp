/**
 *
 *       @file  WsTreeTraversal.cpp
 *
 *      @brief  Traverses the fileSystemTree
 *
 *    @version  1.0
 *       @date  07/08/11 10:18:34
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsTreeTraversal.h"

WsTreeTraversal::WsTreeTraversal(NodePtr node)
{
  m_root = node;
}

int WsTreeTraversal::start()
{
  /* If the root is a directory traverse otherwise it's a file and call traverse file */
  if (m_root.get()->isDirectory())
    return this->traverse(m_root);
  else return this->traverseFile(m_root);
}

int WsTreeTraversal::traverse(NodePtr n)
{
  if (n.get() == 0) {
    LOG(ERROR) << "TreeTraversal::traverse() : Could not traverse : Node is NULL" << endl;
    return FAILURE;
  }
  /* Start to traverse the directory */
  if (beginTraverseDir(n) == SUCCESS) {
    /* Traverse sub folders */
    int r1 = traverseDirNodes(n.get()->getDirectories());
    if (r1 == FAILURE) return FAILURE;
    /* Traverse sub files */
    int r2 = traverseFileNodes(n.get()->getFiles());
    if (r2 == FAILURE) return FAILURE;
    /* End traverse of the dir */
    return endTraverseDir(n);
  }
  return SUCCESS;
}

int WsTreeTraversal::traverseDirNodes(vector<NodePtr> l)
{
  /* Iterate on all the nodes and call traverse */
  for (vector<NodePtr>::iterator it = l.begin(); it != l.end(); it++) {
    int r = traverse(*it);
    if (r == FAILURE)
      return FAILURE;
  }
  return SUCCESS;
}

int WsTreeTraversal::traverseFileNodes(vector<NodePtr> l)
{
  /* Iterate on all the nodes and call traverseFile */
  /* A proper action to traverse file is implemented in the derived class */
  for (vector<NodePtr>::iterator it = l.begin(); it != l.end(); it++) {
    int r = traverseFile(*it);
    if (r == FAILURE)
      return FAILURE;
  }
  return SUCCESS;
}

