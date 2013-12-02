/**
 *
 *       @file  WsTreeDeserializer.cpp
 *
 *      @brief  Deserializes received contents on network
 *
 *    @version  1.0
 *       @date  07/28/11 15:06:28
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsTreeDeserializer.h"
using namespace Json;

WsTreeDeserializer::WsTreeDeserializer(const string& contents): m_contents(contents)
{
}

int WsTreeDeserializer::deserialize()
{
  if (parse() == FAILURE)
    return FAILURE;
  /* Get the rootPath */
  m_rootPath = m_root["root_path"].asString();
  m_stamp = m_root["stamp"].asString();
  /* Create the root node */
  m_rootNode = NodePtr(new WsDirNode(m_rootPath, m_rootPath));
  path basePath(WsGlobalProperties::instance()->get("global", "root_path", "/"));
  m_rootNode.get()->setProperties(NodePropertiesPtr(new WsNodeProperties(m_root["nodes"][basePath.string()]["properties"].toStyledString())));
  /* Add all the children of the root */
  return addSub(m_root["nodes"][m_rootPath.string()]["children"], m_rootNode);
}

int WsTreeDeserializer::addSub(const Value& v, NodePtr n)
{
  /* Iterate on nodes of current level in Json file and add it as children of node n */
  for (ValueIterator itr = v.begin() ; itr != v.end() ; ++itr) {
    Value v1 = *itr;
    path p(itr.key().asString());
    if (v1["type"].asString() == "DIRECTORY") {
      /* Create node */
      NodePtr temp = NodePtr(new WsDirNode(p, m_rootPath));
      n.get()->addChildDirectory(temp);
      /*  Set its properties */
      temp.get()->setProperties(NodePropertiesPtr(new WsNodeProperties(v1["properties"].toStyledString())));
      temp.get()->setModifyDate(v1["modifdate"].asDouble());
      temp.get()->setCreateDate(v1["creatdate"].asDouble());
      /* No children */
      if (v1["children"] == Value::null) {
        continue;
      }
      /* Add children because current node is directory*/
      addSub(v1["children"], temp);
    } else {
      /* current node is file, add only children to parent (n) */
      NodePtr temp = NodePtr(new WsFileNode(p, m_rootPath));
      n.get()->addChildFile(temp);
      temp.get()->setProperties(NodePropertiesPtr(new WsNodeProperties(v1["properties"].toStyledString())));
      temp.get()->setModifyDate(v1["modifdate"].asDouble());
      temp.get()->setCreateDate(v1["creatdate"].asDouble());
      temp.get()->setSize(v1["size"].asDouble());
    }
  }
  return SUCCESS;
}

const string& WsTreeDeserializer::getStamp()
{
  return m_stamp;
}


int WsTreeDeserializer::parse()
{
  if (!m_reader.parse(m_contents, m_root, false)) {
    LOG(ERROR) << "WsTreeDeserializer::parse() : Could not parse received input" << endl;
    return FAILURE;
  }
  return SUCCESS;
}

NodePtr WsTreeDeserializer::getMenuRoot()
{
  return m_rootNode;
}
