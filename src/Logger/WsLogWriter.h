/*
 *
 *       Filename:  WsLogWriter.h
 *
 *    Description:  Writes log data to file using atomic operation
 *                  This class is a singleton and is Thread Safe
 *
 *        Created:  02/16/12 10:13:45
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */


#ifndef WS_LOG_WRITER_H__
#define WS_LOG_WRITER_H__

#include <iostream>
#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

class WsGlobalProperties;


/**
 * @brief LogWriter, writes logs to file or screen
 */
class WsLogWriter {
public:
  /**
   * @brief returns the instance of the WsLogWriter class.
   * If no instance is existing, a new instance will be returned
   **/
  static WsLogWriter* instance();

  /**
   * @brief writes the message to the log file
   * @brief message the message to write
   * @brief level the log level
   **/
  void write(const int& level, const std::string& message);

  /**
   * @destructor
   **/
  ~WsLogWriter();

protected:
  /**
   * @brief Constructor
   */

  WsLogWriter();
  /**
   * @brief Converts the log level to string
   * @param level the log level
   */
  std::string toString(int level);
  /**
   * @brief reads the conf needed to know log path and verbose level
   */
  void readConf();

private:
  static WsLogWriter* m_instance;
  pthread_mutex_t m_mutex;
  std::ofstream m_logfile;
  int m_logLevel;
  std::string m_path;
};

#endif
