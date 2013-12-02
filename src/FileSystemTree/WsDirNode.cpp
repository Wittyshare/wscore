/**
 *
 *       @file  WsdirNode.cpp
 *
 *      @brief  A dir Node
 *
 *    @version  1.0
 *       @date  01.07.2011 16:38:14
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsDirNode.h"
#include <Include/WsGlobalConfig.h>

WsDirNode::WsDirNode(const path& fullPath, const path& rootPath):
  WsNode(fullPath, rootPath),
  m_resources(0)
{
}

WsDirNode::WsDirNode():
  WsNode(),
  m_resources(0)
{
}

WsDirNode::~WsDirNode()
{
  if (m_resources != 0)
    delete m_resources;
  m_resources = 0;
}

set<string> WsDirNode::getGroups()
{
  return m_properties.get()->getGroups();
}

bool WsDirNode::isAllowed(const set<string>& gids)
{
  WsGlobalProperties* props = WsGlobalProperties::instance();
  if (props->get("global", "public_site", "false") == "true") {
    /* Public site access granted */
    return true;
  }
  //lock the parent because we are using it to avoid free
  NodePtr parent = m_parent.lock();
  if ((m_properties.get() == 0 || m_properties.get()->getGroups().size() == 0) && parent.get() != 0)  {
    /* Check if inherit from parent is set to true in Global configuration */
    if (props->get("global", "inherit_rights_from_parent", "false") == "true") {
      /* Set to true, so Get right from parent */
      return parent.get()->isAllowed(gids);
      /* Check if inherit from parent set to true in Node conf */
    } else if (m_properties.get()->get("global", "inherit_rights_from_parent", "false") == "true") {
      if (!parent.get() == 0) {
        /* get right from parent if not null */
        return parent.get()->isAllowed(gids);
      } else return false;
    } else
      return false;
  } else {
    /* We need the rights from the current node */
    return m_properties.get()->isAllowed(gids);
  }
}

vector<string> WsDirNode::getResource(ResourceType t, bool recurse)
{
}

void WsDirNode::setResources(WsNodeResources* resources)
{
  m_resources = resources;
}

bool WsDirNode::getDisplayInParent()
{
  string s = m_properties.get()->get("global", "in_parent", "true");
  if (s == "true")
    return true;
  return false;
}

bool WsDirNode::isDirectory()
{
  return true;
}

bool WsDirNode::isRegularFile()
{
  return false;
}

const uintmax_t WsDirNode::getSize()
{
  if (WsGlobalProperties::instance()->get("global", "dir_size", "true") == "true")
    if (getAll().size() == 0)
      return m_size;
  return getAll().size();
}
