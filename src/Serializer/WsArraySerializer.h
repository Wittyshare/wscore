/**
 *
 *       @file  WsArraySerializer.h
 *
 *      @brief  Serializes the array
 *
 *    @version  1.0
 *       @date  07/29/11 09:55:11
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ARRAY_SERIALIZER_H__
#define WS_ARRAY_SERIALIZER_H__

#include <FileSystemTree/WsFileSystemTree.h>

#include <set>

/**
 * @brief Serializes an array.
 *
 * Takes an array (std::vector) as input and outputs a Json tree
 */
class WsArraySerializer {
public:
  /**
   * @brief Constructor
   * @param tree the file system tree
   * @param groups the groups which the user belongs to
   * @param p the path of the dir to serialize
   **/
  WsArraySerializer(std::set<std::string>& vect);

  /**
   * @brief serializes the directory
   * @return SUCCESS if ok, FAILURE if an error occured
   **/
  int serialize();

  /**
   * @brief returns the serailized form of the directory
   **/
  const std::string getSerializedForm();

private:

  /**
   * @brief The array to serialize
   */
  std::set<std::string> m_array;

  /**
   * @brief the root of the Json tree
   */
  Json::Value m_root;
};

#endif
