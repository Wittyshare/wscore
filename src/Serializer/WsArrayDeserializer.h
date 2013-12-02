/**
 *
 *       @file  WsArrayDeserializer.h
 *
 *      @brief Deserializes contents of a directory sent
 *
 *    @version  1.0
 *       @date  07/29/11 12:16:29
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_ARRAY_DESERIALIZER_H__
#define WS_ARRAY_DESERIALIZER_H__

#include <FileSystemTree/WsDirNode.h>
#include <FileSystemTree/WsFileNode.h>

#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

#include <iostream>

/**
 * @brief Deserializes an array
 *
 * Takes Json as input representing the array serialized with WsArraySerializer and deserializes it
 */
class WsArrayDeserializer {
public:

  /**
   * @brief Constructor
   * @param Json the Json input to deserialize
   **/
  WsArrayDeserializer(const std::string& json);

  /**
   * @brief Deserialize contents serialized by WsArraySerialize::serialize
   * @return SUCCESS if succeeded, FAILURE otherwise
   */
  int deserialize();

  /**
   * @brief returns the set serialized  The method deserialize must be called prior to this method
   */
  const std::set<std::string> getContents();

private:
  /**
   * @brief parse the Json
   */
  int parse();

  /**
  * @brief Json code
  */
  std::string m_json;

  /**
   * @brief Json reader
   */
  Json::Reader m_reader;

  /**
   * @brief Root of the Json tree
   */
  Json::Value m_root;

  /**
   * Deserialized data
   */
  std::set<std::string> m_contents;



};

#endif
