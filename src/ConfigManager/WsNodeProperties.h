/**
 *
 *       @file  WsGlobalProperties.h
 *
 *      @brief  Global properties
 *
 *    @version  1.0
 *       @date  07/26/11 14:18:49
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_FILE_PROPERTIES_H__
#define WS_FILE_PROPERTIES_H__

#include <iostream>
#include <set>
#include <boost/filesystem.hpp>

#include "WsAbstractProperties.h"

/**
 * @brief Properties of a WsNode.
 **/

class WsNodeProperties: public WsAbstractProperties {
public:

  enum Type {File, Dir};

  /**
   * @brief Constructor
   * @param path the path to the node
   * @param t the Type
   */
  WsNodeProperties(boost::filesystem::path nodePath, Type t );

  /**
   * @brief Constructor. Will create the property class from the Json input
   * @param jsonInput the json input text
   **/
  WsNodeProperties(std::string jsonInput);

  ~WsNodeProperties();

  /**
   * @brief return the property id in section . If the property does not exist, def will be returned
   * @param section the section in the Json file
   * @param id the key in the Json file
   * @def the default value of not found
   * @return the value or def if not found
   **/
  std::string get(const std::string& section, const std::string& id, const std::string& def);

  /**
   * @brief Returns the groups that have access to the node
   * If no groups are found in the file, an empty set is returned
   * @return a set of gid or empty vector if no groups found
   **/
  std::set<std::string> getGroups();

  /**
   * @brief Compare the gids with the node groups.
   * If the user has access, true is returned, otherwise false is returned
   * @param gids a vector containing the gid
   **/
  bool isAllowed(std::set<std::string> gids);

  /**
   * @brief Return the root of the Json tree
   **/
  Json::Value getRoot();

  /**
   * @brief set the root of the Json tree
   * @param root the root as Value object
   */
  void setRoot(Json::Value root);

  /**
   * @brief set the root of the Json tree
   * @param jsonInput the Json as string
   */
  void setRoot(const std::string& jsonInput);

  /**
   * @brief Changes or sets the propety with key 'key' to value 'value'
   * If the property already exist, it will be overriden
   * This method does not however, update the config file
   **/
  void set(const std::string& section, const std::string& key, const std::string& value);

  /**
   * @brief Sets the groups of the node to grps. All groups will be overriden.
   * This method does not however, update the config file
   **/
  void setGroups(std::set<std::string> grps);

  /**
   * @brief Creates the directories for the properties
   * @return SUCCESS or FAILURE
   */
  int createPropertiesDirectories();

  /**
   * @brief save the properties to the disk
   * The path of the file is set in m_nodePath and is set when building this object
   * @return SUCCESS or FAILURE is failed
   */
  int save();

protected:

  /**
   * @brief Get path of the node which the configuration belongs to
   *
   * @return a string containing the relative path
   */
  std::string getPath();

private:
  boost::filesystem::path m_nodePath;
  Type m_type;

};
#endif
