/*
 *
 *       Filename:  WsAbstractMonitor.h
 *
 *    Description:
 *
 *        Created:  03/29/2012 09:19:53 AM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ABSTRACT_MONITOR_H__
#define WS_ABSTRACT_MONITOR_H__

#include <boost/thread.hpp>



/**
 * @brief Abstract class used for monitoring the filesystem
 *
 * This class must be inherited and methods overriden
 */
class WsAbstractMonitor {
public:

  /**
   * @brief Virtual destructor
   **/
  virtual ~WsAbstractMonitor() = 0;

  /**
   * @brief starts the monitoring process
   * @return the boost::thread of the monitoring process
   **/
  virtual boost::thread* start() = 0;

};

#endif
