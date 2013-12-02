/*
 *
 *       Filename:  WsFsDaemonLoader.h
 *
 *    Description:
 *
 *        Created:  02/15/12 14:07:18
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_FSDAEMON_LOADER_H__
#define WS_FSDAMEON_LOADER_H__

#include <iostream>
#include <getopt.h>
#include "WsFsDaemon.h"
#include <ConfigManager/WsGlobalProperties.h>
#include <signal.h>


/**
 * @brief  Loads the FsDaemon depending on the params passed as args
 */
class WsFsDaemonLoader {
public:
  /**
   * @brief Constructor
   * @param props  the properties used to lauch the server
   * @param daemon true if you want to run the server in background
   */
  WsFsDaemonLoader(WsGlobalProperties* props, bool daemon = false);

  /**
   * @brief destructor
   */
  ~WsFsDaemonLoader();

  /**
   * @brief launch the server
   * @return SUCCESS or FAILURE
   */
  int start();

private:

  /**
   * @brief daemonize the server, fork and put it in backgroud
   * @returns SUCCESS or FAILURE
   */
  int daemonize();

  /**
   * @brief daemonize or not
   */
  bool m_daemon;

  /**
   * Instance of WsGlobalProperties
   */
  WsGlobalProperties* m_props;

  /**
   * WsFsDaemon instance
   */
  WsFsDaemon* m_server;

};

#endif

