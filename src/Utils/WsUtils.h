/*
 * =====================================================================================
 *
 *       Filename:  WsUtils.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/05/14 15:55:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benoit Daccache (ben.daccache@gmail.com)
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef WS_UTILS_H__
#define WS_UTILS_H__
#include <iostream>

class WsUtils {

    public:
  /**
   * @brief return true or false whether the ip is valid (ie match the mask in the configuration file
   * @param ip the ip
   */
    static bool ipValid(std::string ip);

    static bool ipMatch(std::string ip, std::string mask);

};

#endif
