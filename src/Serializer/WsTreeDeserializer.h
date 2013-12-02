/**
 *
 *       @file  WsTreeDeserializer.h
 *
 *      @brief  Deserializes received contents on network
 *
 *    @version  1.0
 *       @date  07/28/11 15:06:28
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_MENU_TREE_DESERIALIZER_H__
#define WS_MENU_TREE_DESERIALIZER_H__
#include <Tree/WsMenuTree.h>

#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>


/**
 * @brief Deserializes a tree serialized with WsTreeSerializer
 * @see WsTreeSerializer
 */
class WsTreeDeserializer {
public:

  /**
   * @brief Constructor
   * @param contents the tree Json input to deserialize
   **/
  WsTreeDeserializer(const std::string& contents);

  /**
   * @brief deserialize the Json input
   * @return SUCCESS if no problem occured and Json input is Valid
   * FAILURE otherwise
   **/
  int deserialize();

  /**
   * @brief returns the menu root
   **/
  NodePtr getMenuRoot();

  /**
   * @brief Get the Stamp of the serialized tree
   *
   * @return a string containing the stamp
   */
  const std::string& getStamp();
private:
  /**
   * @brief parse the Json tree
   */
  int parse();

  /**
   * @brief adds a SubNode to the tree corresponding to the Json entry
   */
  int addSub(const Json::Value& v, NodePtr n);

  /**
   * @brief Root Node
   */
  NodePtr m_rootNode;

  /**
   * @brief root of Json Tree
   */
  Json::Value m_root;

  /**
   * @brief Json reader
   */
  Json::Reader m_reader;

  /**
   * @brief stamp of the tree
   */
  std::string m_stamp;

  /**
   * @brief  Serialized data that will be deserialized
   */
  std::string m_contents;


  /**
   * @brief The root path
   */
  boost::filesystem::path m_rootPath;



};

#endif
