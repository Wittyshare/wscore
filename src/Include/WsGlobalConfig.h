/**
 *
 *       @file  WsGlobal.h
 *
 *      @brief  Global variables
 *
 *    @version  1.0
 *       @date  01.07.2011 14:22:26
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_GLOBAL_H__
#define WS_GLOBAL_H__


#include <iostream>

namespace ErrorCode {
    const int Success = 0;
    const int Failure = -1;
    const int NoAccess= -4;
    const int NotLogged = -5;
    const int NotFound = -3;
    const int Locked = -2;
}

namespace GlobalConfig {

const int Read                          =       2;
const int ReadWrite                     =       6;

const int On                            =       1;
const int Off                           =       0;
const int Delay                         =       30;
const std::string StdOut                     =       "STDOUT";
const std::string StdErr                     =       "STDERR";

const std::string GlobalPropertiesPath       =       "/etc/wt/wittyshare-global.json";
const std::string LayoutPropertiesPath       =       "/etc/wt/wittyshare-layout.json";
const std::string PluginsPropertiesPath       =       "/etc/wt/wittyshare-plugins.json";
const std::string CrashLogPath = "/tmp/crashLogWs.log";
const std::string PathToLockFile             =       "/.config/freezetree";
const std::string PathToConfig               =       "/.config/";
const std::string PathToNodeProperties       =       "/.config/nodes/";
const std::string PathToNodeLock             =       "/.config/locks/";
const std::string PathToDirProperties        =       "/.config/properties.json";
const std::string PathToImages               =       "/ws.res/images/";
const std::string PathToTemplates            =       "/.config/templates/";
const std::string PathToIcones               =       "/ws.res/icones/";
const std::string ConfExt                    =       ".json";
const int         NbItems                    =        3;
const std::string SubFolders[NbItems]        =       {PathToConfig, PathToNodeProperties, PathToNodeLock};
}


#endif
