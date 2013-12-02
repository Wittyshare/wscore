/*
 *
 *       Filename:  WsLogger.h
 *
 *    Description:  Abstract class that handles logs
 *
 *        Created:  02/10/12 15:36:22
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_LOGGER_H__
#define WS_LOGGER_H__
#include <iostream>
#include <sstream>
#include "WsLogWriter.h"

#ifdef LOG
#undef LOG
#endif
#define LOG WsLogger().log

#ifdef DEBUG
#undef DEBUG
#endif
#define DEBUG 0

#ifdef INFO
#undef INFO
#endif
#define INFO 1

#ifdef WARN
#undef WARN
#endif
#define WARN 2

#ifdef ERROR
#undef ERROR
#endif
#define ERROR 3

/**
 * @brief Logger used in FsDaemon
 */
class WsLogger {
public:
  /**
   * @brief constructor
   */
  WsLogger();

  /**
   * @brief append the text to the stream. On each object destruction, the text will be written in the log
   * @param level log level (DEBUG, INFO,  WARN, ERROR)
   */
  std::stringstream& log(int level = 1);

  /**
   * @brief destructor writes the text to the file
   */
  ~WsLogger();

private:
  std::stringstream m_ss;
  int m_level;
};

#endif
