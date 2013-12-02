/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#include <fstream>

#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>

#include <Include/WsGlobalConfig.h>
#include <Logger/WsLogger.h>
#include <ConfigManager/WsLayoutProperties.h>

#include "WsModulesLoader.h"

WsModulesLoader::WsModulesLoader()
{ }

WsModulesLoader::~WsModulesLoader()
{
  for (int iModule = 0; iModule < m_vModules.size(); ++iModule) {
    WsModule* curModule = m_vModules[iModule]->module;
    delete curModule;
  }
}

bool WsModulesLoader::load()
{
  Json::Value     root;
  Json::Reader    reader;
  std::ifstream   plugins(GlobalConfig::PluginsPropertiesPath.c_str(), std::ifstream::binary);
  bool bOk = reader.parse(plugins, root, false);
  if ( !bOk ) {
    LOG(DEBUG) << reader.getFormatedErrorMessages();
    return false;
  }
  //Json::Value          module = modules[iMod]; // crash
  Json::Value          modules = root["modules"];
  Json::Value::Members mbrs    = modules.getMemberNames();
  bool bDebug = false;
#if GDDEBUG == 1
  bDebug = true;
#endif
  for (int iMod = 0; iMod < mbrs.size(); ++iMod) {
    LOG(DEBUG) << "WsModulesLoader::load Loading module " << mbrs[iMod] << " debug mode = " << bDebug;
    Json::Value          module     = modules[mbrs[iMod]];
    Json::Value::Members attributes = module.getMemberNames();
    for (int iAtt = 0; iAtt < attributes.size(); ++iAtt) {
      if ( attributes[iAtt] == "soName" ) {
        std::string soName = module.get(attributes[iAtt], "").asString();
        if ( bDebug )
          boost::replace_first(soName, ".so", "d.so");
        // Important not RTLD_NOW because a derived module can also load a shared object
        //  and in this step is make in the dynlib initialization process (_init, _fini Ctor/Dtor)
        void* hndl = dlopen(soName.c_str(), RTLD_LAZY);
        if ( hndl == NULL ) {
          LOG(ERROR) << "WsModulesLoader::load dlopen error soName = " << soName <<  " error = " << dlerror();
          continue;
        }
        LOG(DEBUG) << "WsModulesLoader::load Loading module " << soName;
        pf_wsModule func = (pf_wsModule) dlsym(hndl, "buildModule");
        WsModule* pModule = func();
        WsModuleLoader* ml = new WsModuleLoader();
        ml->dlfcnHandler = hndl;
        ml->module = pModule;
        m_vModules.push_back(ml);
        pModule->setSoName(soName);
        //         std::string p = WApplication::instance()->appRoot() + WApplication::instance()->internalPath();
        //         pModule->setSysPath(p);
        for (int iAtt2 = 0; iAtt2 < attributes.size(); ++iAtt2) {
          if ( attributes[iAtt2] == "moduleName" ) {
            std::string moduleName = module.get(attributes[iAtt2], "").asString();
            pModule->setModuleName(moduleName);
          }
          if ( attributes[iAtt2] == "fileName" ) {
            std::string fileName = module.get(attributes[iAtt2], "").asString();
            pModule->setFileName(fileName);
          }
          if ( attributes[iAtt2] == "extension"  ||  attributes[iAtt2] == "extensions" ) {
            std::string extensions = module.get(attributes[iAtt2], "").asString();
            pModule->setExtensions(extensions);
          }
          if ( attributes[iAtt2] == "prefix" ) {
            std::string prefix = module.get(attributes[iAtt2], "").asString();
            pModule->setPrefix(prefix);
          }
          if ( attributes[iAtt2] == "hideImages" ) {
            std::string hideImages = module.get(attributes[iAtt2], "false").asString();
            if ( hideImages == "true" ) pModule->setHideImages(true);
          }
          if ( attributes[iAtt2] == "options" ) {
            Json::Value          options  =  module[attributes[iAtt2]];
            Json::Value::Members optAttrs =  options.getMemberNames();
            for (int iOpt = 0; iOpt < optAttrs.size(); ++iOpt) {
              Json::Value          val  = options[optAttrs[iOpt]];
              LOG(DEBUG) << "WsModuleLoader::load options " << optAttrs[iOpt] << " value = " << val.asString();
              if ( val.type() == Json::nullValue    ) continue;
              if ( val.type() == Json::intValue     )
                pModule->setOption(optAttrs[iOpt], val.asInt());
              if ( val.type() == Json::uintValue     )
                pModule->setOption(optAttrs[iOpt], val.asUInt());
              if ( val.type() == Json::realValue    )
                pModule->setOption(optAttrs[iOpt], val.asDouble());
              if ( val.type() == Json::stringValue  )
                pModule->setOption(optAttrs[iOpt], val.asString());
              if ( val.type() == Json::booleanValue )
                pModule->setOption(optAttrs[iOpt], val.asBool());
            }
          }
        }
        if ( module["loadOnStartup"] != Json::Value::null && module["loadOnStartup"].asBool()) {
          LOG(DEBUG) << "WsModuleLoader::load running Module " << pModule->moduleName();
          //fprintf(stderr, "WsModuleLoader :: running Module %s\n",pModule->moduleName().c_str());
          pModule->createContents();
          pModule->setLoaded();
        } else {
          LOG(DEBUG) << "WsModuleLoader::load running Module  else clause " <<  module["loadOnStartup"].asString();
          //fprintf(stderr, "WsModuleLoader ::else clause%s\n", module["loadOnStartup"].asString().c_str() );
        }
      }
    }
  }
  return true;
}

const std::vector<WsModuleLoader*>& WsModulesLoader::modules()
{
  return m_vModules;
}

const WsModule* WsModulesLoader::module(const std::string& moduleName)
{
  for (int iModule = 0; iModule < modules().size(); ++iModule) {
    WsModule* curModule = modules()[iModule]->module;
    if ( curModule->moduleName() != moduleName ) continue;
    return curModule;
  }
  return 0;
}

std::string WsModulesLoader::pathWithoutPrefix(const std::string& path)
{
  std::string remP = removePrefix(path);
  if ( remP == "" ) remP = "/";
  return remP;
}

std::string WsModulesLoader::removePrefix(const std::string& path)
{
  // TODO : en attendant de mettre ces éléménts en Modules
  std::string sRes(path);
  std::string searchPath = WsLayoutProperties::instance()->get("global", "search_path", "/Search");
  //  LOG(DEBUG)<<"WsModulesLoader::removePrefix() path = " << path;
  if ( path.compare(0,  searchPath.size(), searchPath ) == 0            ) return "";                                      // Search
  if ( path.compare(0,  5, "/Logo"                    ) == 0            ) return sRes.assign(path,  5, path.size() -  5);
  if ( path.compare(0,  8, "/SiteMap"                 ) == 0            ) return sRes.assign(path,  8, path.size() -  8);
  if ( path.compare(0,  5, "/Edit"                    ) == 0            ) return sRes.assign(path,  5, path.size() -  5);
  if ( path.compare(0, 11, "/FileUpload"              ) == 0            ) return sRes.assign(path, 11, path.size() - 11);
  if ( path.compare(0, 10, "/FolderNew"               ) == 0            ) return sRes.assign(path, 10, path.size() - 10);
  if ( path.compare(0, 13, "/FolderDelete"            ) == 0            ) return sRes.assign(path, 13, path.size() - 13);
  if ( path.compare(0, 11, "/FolderEdit"              ) == 0            ) return sRes.assign(path, 11, path.size() - 11);
  for (int iModule = 0; iModule < modules().size(); ++iModule) {
    WsModule* curModule = modules()[iModule]->module;
    if ( curModule->prefix().size() < 1 ) continue;
    if ( path.compare(0, curModule->prefix().size(), curModule->prefix() ) == 0)
      return curModule->pathWithoutPrefix(path);
  }
  return path;
}

std::string WsModulesLoader::checkPath(const std::string& path)
{
  std::string sRes(path);
  for (int iModule = 0; iModule < modules().size(); ++iModule) {
    WsModule* curModule = modules()[iModule]->module;
    sRes = curModule->checkPath(path);
    if ( sRes != path ) break;
  }
  return sRes;
}

