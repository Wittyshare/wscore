/**
 *
 *       @file  WsNodeResources.h
 *
 *      @brief  Node Resources
 *
 *    @version  1.0
 *       @date  11/03/13 14:18:49
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_NODE_RESOURCES_H__
#define WS_NODE_RESOURCES_H__

#include <iostream>
#include <set>
#include <boost/filesystem.hpp>

#include "WsAbstractProperties.h"

/**
 * @brief Resources of a WsNode.
 *
 * TODO not implemented yet
 **/

class WsNodeResources {
public:

  enum Type {Icones, Images, Templates};

  /**
   * @brief Constructor
   */
  WsNodeResources(boost::filesystem::path nodePath);

  const std::vector<std::string>& get(Type t);

  void add(Type, const std::string& value);

  int createResourcesDirectories();

private:
  std::vector<std::string> m_iconesUrl;
  std::vector<std::string> m_imagesUrl;
  std::vector<std::string> m_templatesUrl;
  boost::filesystem::path m_nodePath;


};
#endif
