/**
 *
 *       @file  Wsnode.cpp
 *
 *      @brief  Node structure, must be inherited
 *
 *    @version  1.0
 *       @date  01.07.2011 15:28:22
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsNode.h"
#include <Include/WsGlobalConfig.h>

WsNode::WsNode():
  WsAbstractNode()
{
  m_size = 0;
  m_relPath = "/";
}

WsNode::~WsNode()
{
}

WsNode::WsNode(const path& fullPath, const path& rootPath):
  WsAbstractNode()
{
  try {
    m_size = 0;
    m_fullPath = fullPath;
    string relp = fullPath.string().substr(rootPath.string().length());
    path rp(relp);
    if (rp.string() == "")
      m_relPath = path("/");
    else
      m_relPath = rp;
  } catch (std::out_of_range& exception) {
    LOG(ERROR) << "WsNode::WsNode() : Exception raised: " << exception.what();
  }
}


int WsNode::addChildDirectory(NodePtr f)
{
  NodePtr spThis = shared_from_this();
  f.get()->setParent(spThis);
  try {
    m_dirVect.push_back(f);
  } catch (bad_alloc e) {
    LOG(ERROR) << "WsNode::addChildDirectory() : " << e.what() << endl;
    return FAILURE;
  }
  return SUCCESS;
}

int WsNode::addChildFile(NodePtr f)
{
  NodePtr spThis = shared_from_this();
  f.get()->setParent(spThis);
  try {
    m_fileVect.push_back(f);
  } catch (bad_alloc e) {
    LOG(ERROR) << "WsNode::addChildFile() : " << e.what() << endl;
    return FAILURE;
  }
  return SUCCESS;
}

int WsNode::addChildNode(NodePtr n)
{
  try {
    m_combinedVect.push_back(n);
  } catch (bad_alloc e) {
    return FAILURE;
  }
  return SUCCESS;
}

void WsNode::setParent(NodePtr n)
{
  m_parent = n;
}

NodePtr WsNode::getParent()
{
  NodePtr parent = m_parent.lock();
  return parent;
}

const path& WsNode::getPath()
{
  return m_relPath;
}

const path& WsNode::getFullPath()
{
  return m_fullPath;
}

string WsNode::getName(const bool noExt)
{
  if ( noExt )
    return this->getPath().stem().string();
  return this->getPath().filename().string();
}

const string WsNode::getDisplayName(const bool noExt)
{
  if (m_properties.get()->get("global", "display_name", "") != "" )
    return m_properties.get()->get("global", "display_name", "");
  return getName(noExt);
}

const vector<NodePtr> WsNode::getFiles()
{
  sort();
  return m_fileVect;
}

const vector<NodePtr> WsNode::getDirectories()
{
  sort();
  return m_dirVect;
}

const vector<NodePtr> WsNode::getAll()
{
  if (m_combinedVect.size() == 0 && (m_fileVect.size() != 0 || m_dirVect.size() != 0)) {
    this->sort();
  }
  return m_combinedVect;
}

NodePtr WsNode::getNodeByName(const string& name)
{
  /* Check if node exist in the vector list */
  for (vector<NodePtr>::iterator it = m_dirVect.begin(); it != m_dirVect.end(); it++) {
    if ((*it).get()->getName() == name) {
      return (*it);
    }
  }
  /* Check if node exist in the filelist */
  for (vector<NodePtr>::iterator it2 = m_fileVect.begin(); it2 != m_fileVect.end(); it2++) {
    if ((*it2).get()->getName() == name) {
      return (*it2);
    }
  }
  LOG(DEBUG) << "WsNode::getNodeByName() : Node not found " << name;
  return NodePtr();
}

bool WsNode::getDisplayInMenu()
{
  string r = m_properties.get()->get("global", "in_menu", "false");
  if (r == "true")
    return true;
  return false;
}


void WsNode::setProperties(NodePropertiesPtr properties)
{
  m_properties = properties;
}

void WsNode::setSize(const uintmax_t& size)
{
  m_size = size;
}

NodePropertiesPtr WsNode::getProperties()
{
  return m_properties;
}

string WsNode::getProperty(const std::string& section, const std::string& id, const std::string& def, bool recurse)
{
  //TODO recursion ??
  string prop = m_properties.get()->get(section, id, "");
  if ( prop == "" && recurse) {
    NodePtr parent = m_parent.lock();
    if (parent.get() != 0) {
      return parent.get()->getProperty(section, id, def, recurse);
    } else return def;
  } else return prop != "" ? prop : def;
}

void WsNode::sort()
{
/*
  vector<NodePtr> all;
  all.reserve(m_fileVect.size() + m_dirVect.size());
  all.insert(all.end(), m_dirVect.begin(), m_dirVect.end());
  all.insert(all.end(), m_fileVect.begin(), m_fileVect.end());
  if (this->getProperty("global", "sort", "true") == "true") {
    std::sort(m_fileVect.begin(), m_fileVect.end(), compareNodes());
    std::sort(m_dirVect.begin(), m_dirVect.end(), compareNodes());
    std::sort(all.begin(), all.end(), compareNodes());
  }
  m_combinedVect = all;
*/
}

NodePtr WsNode::eatPath(const string& path)
{
  string name;
  string sub;
  //We are searching for this node
  if (path == "/") {
    NodePtr sThis = shared_from_this();
    return sThis;
  }
  //remove trailing '/'
  if (path[0] == '/')
    name = path.substr(1);
  else name = path;
  //get the next chunck of the name
  if (name.find("/") != string::npos) {
    sub = name.substr(0, name.find_first_of("/"));
    name = name.substr(name.find_first_of("/"));
  } else {
    sub = name;
    name = "";
  }
  NodePtr cur = this->getNodeByName(sub);
  if (cur.get() == 0) {
    LOG(ERROR) << "WsNode::eatPath() : No such path " << path;
    return NodePtr();
  }
  if (name == "" || name == "/") {
    return cur;
  }
  return cur.get()->eatPath(name);
}

const time_t& WsNode::getModifyDate()
{
  return m_modifyTime;
}

const time_t& WsNode::getCreateDate()
{
  return m_createTime;
}

void WsNode::setCreateDate(const time_t& t)
{
  m_createTime = t;
}

void WsNode::setModifyDate(const time_t& t)
{
  m_modifyTime = t;
}
