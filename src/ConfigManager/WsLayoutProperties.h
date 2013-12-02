/*
 *
 *       Filename:  WsLayoutProperties.h
 *
 *    Description:
 *
 *        Created:  03/29/2012 03:47:05 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_LAYOUT_PROPERTIES_H__
#define WS_LAYOUT_PROPERTIES_H__

#include "WsAbstractProperties.h"

#include <iostream>

/**
 * @brief Layout Properties.
 *
 * Used for rendering the site
 **/

class WsLayoutProperties : public WsAbstractProperties {
public:

  /**
   * @brief Get the singleton instance
   */
  static WsLayoutProperties* instance();

  /**
   * @brief return the property id in section . If the property does not exist, def will be returned
   * @param section the section in the Json file
   * @param id the key in the Json file
   * @def the default value of not found
   * @return the value or def if not found
   **/
  std::string get(const std::string& section, const std::string& id, const std::string& def);

protected:
  /**
   * @brief Default Constructor
   **/
  WsLayoutProperties();

private:
  static WsLayoutProperties* m_instance;
};

#endif
