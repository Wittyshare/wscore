/**
*       Filename:  resultItem.cpp
*
*    Description:  One search result item.
*
*        Version:  1.0
*        Created:  07/11/2011 04:38:00 PM
*
*/

#include "WsResultItem.h"

WsResultItem::WsResultItem(const path& fullPath, const path& relPath, string body, string type, int size):
  WsFileNode(fullPath, relPath),
  m_body(body),
  m_type(type)
{
  WsFileNode::setSize(size);
}

WsResultItem::~WsResultItem() {}

string WsResultItem::getBody()
{
  return m_body;
}
string WsResultItem::getType()
{
  return m_type;
}
