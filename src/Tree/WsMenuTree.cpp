/*
*       @file  WsMenuTree.cpp
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

#include "WsMenuTree.h"

WsMenuTree::WsMenuTree(FileSystemTreePtr fs, const set<string>& gid, const int& md, const path& rootPath, const std::string& stamp):
  WsAbstractTree(fs, gid, md, rootPath, stamp)
{
}

WsMenuTree::WsMenuTree(FileSystemTreePtr fs, const set<string>& gid, const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const std::string& stamp):
  WsAbstractTree(fs, gid, md, names, ext, rootPath, stamp)
{
}

WsMenuTree::WsMenuTree(NodePtr n, const set<string>& gid, const int& md, const path& rootPath, const std::string& stamp):
  WsAbstractTree(n, gid, md, rootPath, stamp)
{
}

WsMenuTree::WsMenuTree(NodePtr n, const set<string>& gid, const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const std::string& stamp):
  WsAbstractTree(n, gid, md, names, ext, rootPath, stamp)
{
}

int WsMenuTree::beginTraverseDir(NodePtr n)
{
  /* First iteration, no root node */
  if (!dynamic_cast<WsDirNode*>(n.get())->getDisplayInMenu())
    return FAILURE;
  return WsAbstractTree::beginTraverseDir(n);
}

int WsMenuTree::traverseFile(NodePtr n)
{
  if (!dynamic_cast<WsFileNode*>(n.get())->getDisplayInMenu())
    return SUCCESS;
  return WsAbstractTree::traverseFile(n);
}
