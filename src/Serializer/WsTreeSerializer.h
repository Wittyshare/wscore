/**
 *
 *       @file  WsTreeSerializer.h
 *
 *      @brief  Serialize the menuTree to send it over network
 *
 *    @version  1.0
 *       @date  07/28/11 11:30:00
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_MENU_TREE_SERAILIZER_H__
#define WS_MENU_TREE_SERAILIZER_H__

#include <TreeTraversal/WsTreeTraversal.h>

#include <iostream>

#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>


#include <Tree/WsMenuTree.h>
#include <Tree/WsAccessTree.h>

/**
 * @brief Serializes a tree (WsAccessTree, WsMenuTree).
 *
 * Takes the tree as input and outputs a Json tree
 * Use WsTreeDeserializer to deserialize it
 */
class WsTreeSerializer {

public:
  /**
   * @brief Constructor
   * @param tree the WsMenuTree to serialize
   **/
  WsTreeSerializer(WsMenuTree* tree);

  /**
   * @brief Constructor
   * @param tree the WsAccessTree to serialize
   **/
  WsTreeSerializer(WsAccessTree* tree);

  /**
   * @brief serialize the given data structur
   **/
  int serialize();

  /**
   * @brief returns the Json text representing the given structure
   **/
  const std::string getSerializedForm();

private:

  /**
   * @brief adds a SubNode to the tree corresponding to the Json entry
   */
  int addSub(Json::Value& v, NodePtr n);

  NodePtr m_rootNode;
  Json::Value m_root;
  Json::Value m_current;
  boost::filesystem::path m_rootPath;
  std::string m_stamp;

};

#endif
