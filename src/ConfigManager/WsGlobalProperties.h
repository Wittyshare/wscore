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

#ifndef WS_GLOBAL_PROPERTIES_H__
#define WS_GLOBAL_PROPERTIES_H__
#include <iostream>

#include "WsAbstractProperties.h"

/**
 * @brief Global properties class
 *
 * Contains settings for running wittyshare
 **/

class WsGlobalProperties: public WsAbstractProperties {
public:


  static WsGlobalProperties* instance();

  /**
   * @brief return the property id in section . If the property does not exist, def will be returned
   * @param section the section in the Json file
   * @param id the key in the Json file
   * @def the default value of not found
   * @return the value or def if not found
   **/
  std::string get(const std::string& section, const std::string& id, const std::string& def);

  /**
   * @brief Changes or sets the propety with key 'key' to value 'value'
   * If the property already exist, it will be overriden
   * This method does not however, update the config file
   * @param key the key in the Json file
   * @param value the value assigned to the key
   **/
  void set(const std::string& section, std::string key, std::string value);

  /**
   * @brief Search for property id and replaces the $1 in the string with newVal.
   * If the property is not found, def will be returned
   * @param section the section in the Json file
   * @param id the key in the Json file
   * @param newVal the value that will replace the $1
   * @param def the default value returned if not found
   * @returns the value with $1 replaced with newVal or def otherwise
   **/
  std::string getAndReplace(const std::string& section, const std::string& id, const std::string& newVal, const std::string& def);

protected:
  /**
   * @brief Constructor
   */
  WsGlobalProperties();

private:
  static WsGlobalProperties* m_instance;

};
#endif
