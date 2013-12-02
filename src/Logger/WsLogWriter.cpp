/*
 *
 *       Filename:  WsLogWriter.cpp
 *
 *    Description:  Writes log data to file using atomic operation
 *                  This class is a singleton and is Thread Safe
 *
 *        Created:  02/16/12 10:13:45
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsLogWriter.h"
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <Include/WsGlobalConfig.h>
#include <ConfigManager/WsGlobalProperties.h>

using namespace std;

WsLogWriter* WsLogWriter::m_instance = 0;

WsLogWriter* WsLogWriter::instance()
{
  if (m_instance == 0)
    m_instance = new WsLogWriter();
  return m_instance;
}

WsLogWriter::WsLogWriter()
{
  pthread_mutex_init(&m_mutex, NULL);
  readConf();
  if (m_path != GlobalConfig::StdOut && m_path != GlobalConfig::StdErr)
    m_logfile.open (m_path.c_str(), ios::app);
  m_instance = 0;
}

WsLogWriter::~WsLogWriter()
{
  pthread_mutex_destroy(&m_mutex);
  if (m_path != GlobalConfig::StdOut && m_path != GlobalConfig::StdErr)
    m_logfile.close();
}

string WsLogWriter::toString(int level)
{
  switch (level) {
  case 0:
    return "debug";
  case 1:
    return "info";
  case 2:
    return "warn";
  case 3:
    return "error";
  default:
    return "unknown";
  }
}
void WsLogWriter::write(const int& level, const string& message)
{
  if (m_logLevel <= level) {
    if (m_path == GlobalConfig::StdOut) {
      pthread_mutex_lock(&m_mutex);
      string msg = message;
      replace(msg.begin(), msg.end(), '\n', ' ');
      cout << "[";
      cout << microsec_clock::local_time();
      cout << "] ";
      cout << (int)getpid() << " ";
      cout << "[" << toString(level) << "] ";
      cout << msg << endl;
      pthread_mutex_unlock(&m_mutex);
    } else if (m_path == GlobalConfig::StdErr) {
      pthread_mutex_lock(&m_mutex);
      string msg = message;
      replace(msg.begin(), msg.end(), '\n', ' ');
      cerr << "[";
      cerr << microsec_clock::local_time();
      cerr << "] ";
      cerr << (int)getpid() << " ";
      cerr << "[" << toString(level) << "] ";
      cerr << msg << endl;
      pthread_mutex_unlock(&m_mutex);
    } else {
      pthread_mutex_lock(&m_mutex);
      string msg = message;
      replace(msg.begin(), msg.end(), '\n', ' ');
      m_logfile << "[";
      m_logfile << microsec_clock::local_time();
      m_logfile << "] ";
      m_logfile << (int)getpid() << " ";
      m_logfile << "[" << toString(level) << "] ";
      m_logfile << msg << endl;
      pthread_mutex_unlock(&m_mutex);
    }
  }
}

void WsLogWriter::readConf()
{
  m_path = WsGlobalProperties::instance()->get("global", "log_path", "/dev/stdout");
  string level = WsGlobalProperties::instance()->get("global", "log_level", "info");
  if (level == "debug")
    m_logLevel = 0;
  else if (level == "info")
    m_logLevel = 1;
  else if (level == "warning")
    m_logLevel = 2;
  else if (level == "error")
    m_logLevel = 4;
  else
    try {
      m_logLevel = boost::lexical_cast<int>("info");
    } catch (boost::bad_lexical_cast&) {
      m_logLevel = 0;
    }
}

