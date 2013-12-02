/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsModuleLoader_H_
#define _WsModuleLoader_H_

#include <dlfcn.h>

#include "../WsModule/src/WsModule.h"

class WsModuleLoader {
public :
  void*                                 dlfcnHandler;
  WsModule*                             module;
};

class WsModulesLoader {
public :
  WsModulesLoader();
  ~WsModulesLoader();
  bool                                  load();
  const std::vector<WsModuleLoader*>&   modules();
  const WsModule*                       module(const std::string& moduleName);
  std::string                           pathWithoutPrefix(const std::string& path);
  std::string                           checkPath(const std::string& path);

private :
  std::string                           removePrefix(const std::string& path);

private :
  std::vector<WsModuleLoader*>          m_vModules;
};

#endif // ifdef  _WsModuleLoader_H_

