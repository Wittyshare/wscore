/**
 *
 *       @file  WsMonitor.h
 *
 *      @brief  Monitors the FileSystem for any modification
 *
 *    @version  1.0
 *       @date  07/22/11 13:37:17
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_MONITOR_H__
#define WS_MONITOR_H__

#define BOOST_NO_DEPRECATED
#include <fam.h>
#include <vector>
#include <iostream>
#include <boost/thread.hpp>

#include <Logger/WsLogger.h>
#include "WsAbstractMonitor.h"

class WsFsTreeUpdater;

/**
 * @brief Monitors the filesystem and call the WsFsTreeUpdater::update() method accordignly
 */
class WsMonitor : public WsAbstractMonitor {

public:

  /**
   * @brief Constructor
   * @param u the WsFsTreeUpdater
   * @param v the path to monitor
   * @param delay the update delay
   **/
  WsMonitor(WsFsTreeUpdater* u, std::vector<path> v, unsigned int delay);

  /**
   * @brief Destructor
   **/
  ~WsMonitor();

  /**
   * @brief Starts the monitoring process
   * @return the thread in which the monitoring is launched
   **/
  boost::thread* start();

private:

  /**
   * @brief Starts monitoring the Files of the filesystemtree
   * Should be started in a separated thread
   * @return FAILURE if failed, otherwise endless loop
   */
  int startCallBack();

  /**
   * @brief Starts monitoring the config Files of the filesystemtree
   * Should be started in a separated thread
   * @return FAILURE if failed, otherwise endless loop
   */
  int startCallBackConfOnly();


  /**
   * @brief Starts loop that will monitor the m_updated variable every m_delay and update the tree if changed occured
   *
   * @return Nothing
   */
  int startChecker();

  /**
   * TODO DELETE ??
   */
  int addDirectory(const path& p);


  /**
   * @brief Matches an event code with it's name
   *
   * @param code the event code
   *
   * @return a char* containing the event name or unknown if no match found
   */
  const char* eventName(const int& code);

  /**
   * @brief Called when a change occurs on a file monitored.
   *
   * Sets the m_updated variable to true if event code is FamDeleted or FamCreated
   *
   * @param filename the path to the file changed
   * @param code the event code
   *
   * @return SUCCESS
   */
  int signal(const std::string& filename, const int& code);

  /**
   * @brief Called when a change occurs on a conf file monitored.
   *
   * Sets the m_updated variable to true if event code is FamDeleted or FamCreated
   *
   * @param filename the path to the conf file changed
   * @param code the event code
   *
   * @return SUCCESS
   */
  int signalConf(const std::string& filename, const int& code);

  /**
   * paths to monitor
   */
  std::vector<path> m_paths;

  /**
   * @brief pointer to the WsFsTreeUpdater class
   */
  WsFsTreeUpdater* m_updater;


  /**
   * @brief Changed occured ?
   */
  bool m_updated;
  unsigned int m_delay;

  FAMRequest* m_frp;
  FAMRequest* m_frp2;
  FAMConnection* m_fc;
  FAMConnection* m_fc2;
};

#endif

