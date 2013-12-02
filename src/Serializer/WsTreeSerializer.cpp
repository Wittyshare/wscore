/**
 *
 *       @file  WsMenuSerializer.cpp
 *
 *      @brief  Serialize the menuTree to send it over network
 *
 *    @version  1.0
 *       @date  07/28/11 11:30:00
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsTreeSerializer.h"

using namespace Json;


WsTreeSerializer::WsTreeSerializer(WsMenuTree* tree):
  m_rootNode(tree->getRoot()),
  m_rootPath(tree->getRootPath()),
  m_stamp(tree->getStamp())
{
}

WsTreeSerializer::WsTreeSerializer(WsAccessTree* tree):
  m_rootNode(tree->getRoot()),
  m_rootPath(tree->getRootPath()),
  m_stamp(tree->getStamp())
{
}

int WsTreeSerializer::serialize()
{
  /* Avoid Segfault if user has access to nothing */
  if (m_rootNode.get() == 0)
    return SUCCESS;
  /* Serialize the rootPath */
  m_root["root_path"] = m_rootPath.string();
  m_root["stamp"] = m_stamp;
  /* Serialize all the children of the root */
  return addSub(m_root["nodes"], m_rootNode);
}

int WsTreeSerializer::addSub(Value& v, NodePtr n)
{
  if (n.get()->isDirectory()) {
    /* Add type and properties */
    v[n.get()->getFullPath().string()]["type"] = "DIRECTORY";
    v[n.get()->getFullPath().string()]["properties"] = n.get()->getProperties().get()->getRoot();
    v[n.get()->getFullPath().string()]["modifdate"] = (double)n.get()->getModifyDate();
    v[n.get()->getFullPath().string()]["creatdate"] = (double)n.get()->getCreateDate();
    /* Add the Files */
    vector<NodePtr> childrenFiles = n.get()->getFiles();
    /* Iterate on children files and Serialize each node */
    for (int i = 0; i <  childrenFiles.size(); ++i) {
      /* Set the type of the node */
      v[n.get()->getFullPath().string()]["children"][childrenFiles[i].get()->getFullPath().string()]["type"] = "FILE";
      v[n.get()->getFullPath().string()]["children"][childrenFiles[i].get()->getFullPath().string()]["properties"] = childrenFiles[i].get()->getProperties().get()->getRoot();
      v[n.get()->getFullPath().string()]["children"][childrenFiles[i].get()->getFullPath().string()]["size"] = (double)childrenFiles[i].get()->getSize();
      v[n.get()->getFullPath().string()]["children"][childrenFiles[i].get()->getFullPath().string()]["modifdate"] = (double)childrenFiles[i].get()->getModifyDate();
      v[n.get()->getFullPath().string()]["children"][childrenFiles[i].get()->getFullPath().string()]["creatdate"] = (double)childrenFiles[i].get()->getCreateDate();
    }
    /* Add the dirs */
    vector<NodePtr> childrenDir = n.get()->getDirectories();
    for (int i = 0; i <  childrenDir.size(); ++i) {
      /* Recursivly add the children of each directory */
      addSub(v[n.get()->getFullPath().string()]["children"], childrenDir[i]);
    }
  } else {
    LOG(ERROR) << "WsTreeTraversal::addSub() : Node is a file ";
    return FAILURE;
  }
  return SUCCESS;
}

const string WsTreeSerializer::getSerializedForm()
{
  return m_root.toStyledString();
}
