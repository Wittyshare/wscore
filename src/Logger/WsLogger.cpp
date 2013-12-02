/*
 *
 *       Filename:  WsLogger.cpp
 *
 *    Description:  Abstract class that handles logs
 *
 *        Created:  02/10/12 15:36:22
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsLogger.h"
#include <sstream>
using namespace std;

WsLogger::WsLogger():
  m_ss(stringstream::in | stringstream::out)
{
}

WsLogger::~WsLogger()
{
  WsLogWriter::instance()->write(m_level, m_ss.str());
}

stringstream& WsLogger::log(int level)
{
  m_level = level;
  return m_ss;
}

