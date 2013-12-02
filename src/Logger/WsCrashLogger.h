/*
 * =====================================================================================
 *
 *       Filename:  WsCrashLogger.h
 *
 *    Description:  Logs Ws crashes
 *
 *        Version:  1.0
 *        Created:  27/11/13 09:57:10
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 * =====================================================================================
 */

#ifndef WS_CRASH_LOGGER_H__
#define WS_CRASH_LOGGER_H__
#include <stdio.h>


class WsCrashLogger {

public:
  static void handleSignal(int signum);
  static void printStackTrace(FILE* out = stderr, unsigned int maxFrames = 63);



};


#endif
