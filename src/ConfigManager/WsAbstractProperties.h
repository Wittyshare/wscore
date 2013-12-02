/**
 *
 *       @file  WsAbstractProperties.h
 *
 *      @brief  Abstract class for properties
 *
 *    @version  1.0
 *       @date  07/18/11 10:17:35
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ABSTRACT_PROPERTIES_H__
#define WS_ABSTRACT_PROPERTIES_H__

#include <iostream>
#include <fstream>

#include <Logger/WsLogger.h>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

/**
 * @brief Abstract Properties class.
 *
 * must be derived and pure virtual methods redefined
 **/


class WsAbstractProperties {
public:
  /**
   * @brief Default constructor
   **/
  WsAbstractProperties();

  /**
   * @brief return the property id in section . If the property does not exist, def will be returned
   * @param section the section in the Json file
   * @param id the key in the Json file
   * @def the default value of not found
   * @return the value or def if not found
   **/
  virtual std::string get(const std::string& section, const std::string& id, const std::string& def) = 0;

protected:
  /**
   * @brief parse the Json config file
   * @param path the path to the configuration file
   * @return SUCCESS if file is ok, FAILURE otherwise
   **/
  int parse(const std::string& path);

  /**
   * Root of the Json tree where. This is where the contents of Json parsed file is stored
   */
  Json::Value m_root;

  /**
   * True or false whether the configuration file has already be parsed or no
   * This flag is changed by the WsAbstractProperties::parse method
   */
  bool m_parsed;
};

#endif
