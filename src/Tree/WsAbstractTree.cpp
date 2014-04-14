/**
 *
 *       @file  WsAbstractTree.cpp
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

#include "WsAbstractTree.h"
#include <Include/WsGlobalConfig.h>

WsAbstractTree::WsAbstractTree(FileSystemTreePtr fs, const set<string>& gid, const int& md, const path& rootPath, const string& stamp):
  WsTreeTraversal(fs->getRoot()),
  m_gid(gid),
  m_rootPath(rootPath),
  m_maxDepth(md),
  m_currentDepth(0),
  m_stamp(stamp)
{
}

WsAbstractTree::WsAbstractTree(FileSystemTreePtr fs, const set<string>& gid, const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const string& stamp):
  WsTreeTraversal(fs->getRoot()),
  m_gid(gid),
  m_exclNames(names),
  m_exclExt(ext),
  m_rootPath(rootPath),
  m_maxDepth(md),
  m_currentDepth(0),
  m_stamp(stamp)
{
}

WsAbstractTree::WsAbstractTree(NodePtr n, const set<string>& gid, const int& md,  const path& rootPath, const string& stamp):
  WsTreeTraversal(n),
  m_gid(gid),
  m_rootPath(rootPath),
  m_maxDepth(md),
  m_currentDepth(0),
  m_stamp(stamp)
{
}

WsAbstractTree::WsAbstractTree(NodePtr n, const set<string>& gid, const int& md, const set<string>& names, const set<string>& ext, const path& rootPath, const string& stamp):
  WsTreeTraversal(n),
  m_gid(gid),
  m_exclNames(names),
  m_exclExt(ext),
  m_rootPath(rootPath),
  m_maxDepth(md),
  m_currentDepth(0),
  m_stamp(stamp)
{
}

WsAbstractTree::~WsAbstractTree()
{
}

int WsAbstractTree::build()
{
  WsTreeTraversal::start();
}

NodePtr WsAbstractTree::getRoot()
{
  return m_root;
}

int WsAbstractTree::beginTraverseDir(NodePtr n)
{
  path rootPath = this->m_rootPath;
  /* First iteration, no root node */
  if (m_root.get() == 0 ) {
    /* Check if user has access to root node */
    if (!dynamic_cast<WsDirNode*>(n.get())->isAllowed(m_gid)) {
      return ErrorCode::Failure;
    }
    /* We create a new node because children are not necessarly the same for each user, depends on rights */
    m_root = NodePtr(new WsDirNode(WsTreeTraversal::m_root->getFullPath(), rootPath));
    m_root.get()->setProperties(dynamic_cast<WsDirNode*>(WsTreeTraversal::m_root.get())->getProperties());
    m_current = m_root;
    return ErrorCode::Success;
  }
  if (m_maxDepth == 0 || m_currentDepth < m_maxDepth) {
    /* Check if user can access this node */
    if (dynamic_cast<WsDirNode*>(n.get())->isAllowed(m_gid)) {
      /* Check if this name is excluded */
      if (m_exclNames.count(n.get()->getFullPath().stem().string()) > 0) {
        LOG(DEBUG) << "WsAbstractTree::beginTraverseDir() : Excluding node " << n.get()->getFullPath();
        return ErrorCode::Failure;
      }
      NodePtr nn(new WsDirNode(n.get()->getFullPath(), rootPath));
      nn.get()->setProperties(n.get()->getProperties());
      if (m_current->addChildDirectory(nn) == ErrorCode::Failure) {
        LOG(DEBUG) << "WsAbstractTree::beginTraverseDir() :  Excluding node " << n.get()->getFullPath();
        return ErrorCode::Failure;
      }
      nn.get()->setParent(m_current);
      m_current = nn;
    }
    /* The user cannot access this node, return ErrorCode::Failure
     * to stop recursion inside subdirs */
    else {
      LOG(DEBUG) << "WsAbstractTree::beginTraverseDir() : Not allowed to access " << n->getPath();
      return ErrorCode::Failure;
    }
    m_currentDepth++;
  } else
    /* Return Failure to stop recursion into subdirs */
    return ErrorCode::Failure;
  return ErrorCode::Success;
}

int WsAbstractTree::endTraverseDir(NodePtr n)
{
  if (m_current.get() != 0 && m_current.get()->getParent() != 0) {
    m_current = m_current.get()->getParent();
  }
  return ErrorCode::Success;
}

int WsAbstractTree::traverseFile(NodePtr n)
{
  path rootPath = this->m_rootPath;
  /* Check if the filename or extension is excluded */
  if (m_exclNames.count(n.get()->getFullPath().stem().string()) > 0) {
    LOG(DEBUG) << "WsAbstractTree::traverseFile() :  Excluding node " << n.get()->getFullPath();
    return ErrorCode::Success;
  }
  if (m_exclExt.count(n.get()->getFullPath().extension().string()) > 0) {
    LOG(DEBUG) << "WsAbstractTree::traverseFile() : Excluding node " << n.get()->getFullPath();
    return ErrorCode::Success;
  }
  NodePtr nn(new WsFileNode(n.get()->getFullPath(), rootPath));
  nn.get()->setProperties(n.get()->getProperties());
  nn.get()->setCreateDate(dynamic_cast<WsFileNode*>(n.get())->getCreateDate());
  nn.get()->setModifyDate(dynamic_cast<WsFileNode*>(n.get())->getModifyDate());
  nn.get()->setSize(dynamic_cast<WsFileNode*>(n.get())->getSize());
  /* Check if there is no root */
  if (m_root.get() == 0) {
    m_root = nn;
    return ErrorCode::Success;
  }
  if (m_current->addChildFile(nn) == ErrorCode::Failure)
    return ErrorCode::Failure;
  nn->setParent(m_current);
  return ErrorCode::Success;
}

path& WsAbstractTree::getRootPath()
{
  return this->m_rootPath;
}

const string& WsAbstractTree::getStamp()
{
  return m_stamp;
}
