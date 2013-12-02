/**
 *
 *       @file  WsfileNode.cpp
 *
 *      @brief  A file Node
 *
 *    @version  1.0
 *       @date  01.07.2011 16:37:55
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFileNode.h"

WsFileNode::WsFileNode(const path& fullPath, const path& rootPath): WsNode(fullPath, rootPath)
{
}

WsFileNode::WsFileNode(): WsNode()
{
}

const set<string> WsFileNode::getGroups()
{
  /* Get the groups from parent because this is a WsFileNode */
  if ( !m_parent.expired() ) {
    NodePtr parent = m_parent.lock();
    return dynamic_cast<WsDirNode*>(parent.get())->getGroups();
  }
  return set<string>();
}

bool WsFileNode::isAllowed(const set<string>& gids)
{
  /* Currently, access to a file is allowed if access is granted to its */
  /*  containing directory */
  if ( !m_parent.expired() ) {
    NodePtr parent = m_parent.lock();
    return dynamic_cast<WsDirNode*>(parent.get())->isAllowed(gids);
  }
  return false;
}

/**
 * @deprecated
 * TODO remove
 */
bool WsFileNode::getDisplayInMenu()
{
  string val = m_properties.get()->get("global", "in_menu", "true");
  if (val == "true")
    return true;
  return false;
}

const string WsFileNode::getPublishDate()
{
  string date = m_properties.get()->get("global", "publish_date", "");
  return date == "" ? boost::lexical_cast<string>(m_modifyTime) : date;
}

const string WsFileNode::getAuthor()
{
  return m_properties.get()->get("global", "author", "");
}

const uintmax_t WsFileNode::getSize()
{
  return m_size;
}

bool WsFileNode::isDirectory()
{
  return false;
}

bool WsFileNode::isRegularFile()
{
  return true;
}
