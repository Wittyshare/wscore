/**
 *
 *       @file  dirContentsSerializer.cpp
 *
 *      @brief  Serializes an array
 *
 *    @version  1.0
 *       @date  07/29/11 09:55:11
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsArraySerializer.h"
using namespace Json;

WsArraySerializer::WsArraySerializer(set<string>& vect):
  m_array(vect)
{
}

int WsArraySerializer::serialize()
{
  /* Serialize each item of the vector */
  int i = 0;
  for (set<string>::iterator it = m_array.begin(); it != m_array.end(); ++it, ++i) {
    m_root["array"][i] = *it;
  }
  return SUCCESS;
}

const string WsArraySerializer::getSerializedForm()
{
  return m_root.toStyledString();
}
