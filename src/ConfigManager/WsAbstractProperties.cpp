/**
 *
 *       @file  WsAbstractProperties.cpp
 *
 *      @brief  Abstract class for properties
 *
 *    @version  1.0
 *       @date  07/18/11 14:36:35
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAbstractProperties.h"

#include <Include/WsGlobalConfig.h>
#include <Logger/WsLogger.h>

#include <fstream>

using namespace Json;

WsAbstractProperties::WsAbstractProperties():
  m_parsed(false)
{
}

int WsAbstractProperties::parse(const std::string& path)
{
  Reader reader;
  std::ifstream conf(path.c_str(), std::ifstream::binary);
  if (! conf.good()) {
    LOG(DEBUG) << "WsAbstractProperties::parse() : Conf file not found : " << path;
    m_parsed = true;
    return SUCCESS;
  }
  bool bOk = reader.parse(conf, m_root, false);
  conf.close();
  if ( !bOk ) {
    /* Parse failed */
    return FAILURE;
  } else {
    m_parsed = true;
    return SUCCESS;
  }
}

