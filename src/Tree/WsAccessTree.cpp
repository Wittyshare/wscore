/**
 *
 *       @file  WsAccessTree.cpp
 *
 *      @brief  Reprensents the access tree of a user
 *      If a user does not have access to a Node, this Node will not show in this tree.
 *
 *    @version  1.0
 *       @date  07/08/11 13:28:14
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAccessTree.h"
#include <Include/WsGlobalConfig.h>

WsAccessTree::WsAccessTree(FileSystemTreePtr fs, const set<string>& gid, const int& md, const path& rootPath, const std::string& stamp):
  WsAbstractTree(fs, gid, md, rootPath, stamp)
{
}

WsAccessTree::WsAccessTree(FileSystemTreePtr fs, const set<string>& gid,  const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const std::string& stamp):
  WsAbstractTree(fs, gid, md, names, ext, rootPath, stamp)
{
}

WsAccessTree::WsAccessTree(NodePtr n, const set<string>& gid, const int& md, const path& rootPath, const std::string& stamp):
  WsAbstractTree(n, gid, md, rootPath, stamp)
{
}

WsAccessTree::WsAccessTree(NodePtr n, const set<string>& gid, const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const std::string& stamp):
  WsAbstractTree(n, gid, md, names, ext, rootPath, stamp)
{
}

