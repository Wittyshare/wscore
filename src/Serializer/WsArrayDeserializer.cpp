/**
 *
 *       @file  WsArrayDeserializer.cpp
 *
 *      @brief Deserializes contents of a set sent
 *
 *    @version  1.0
 *       @date  07/29/11 12:16:29
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsArrayDeserializer.h"
using namespace Json;

WsArrayDeserializer::WsArrayDeserializer(const string& json):
  m_json(json)
{
}

int WsArrayDeserializer::deserialize()
{
  /* Parse the received Json */
  if (parse() == ErrorCode::Failure) {
    return ErrorCode::Failure;
  }
  /* Deserialize each item and put it in vector*/
  for (int i = 0; i < m_root["array"].size(); ++i) {
    m_contents.insert(m_root["array"][i].asString());
  }
  return ErrorCode::Success;
}

const set<string> WsArrayDeserializer::getContents()
{
  return m_contents;
}


int WsArrayDeserializer::parse()
{
  if (!m_reader.parse(m_json, m_root, false)) {
    LOG(ERROR) << "WsArrayDeserializer::parse() : Could not parse received input" << endl;
    return ErrorCode::Failure;
  }
  return ErrorCode::Success;
}
