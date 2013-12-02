/*
 *
 *       Filename:  WsSearch.cpp
 *
 *    Description:
 *
 *        Created:  03/08/2012 02:35:21 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsSearch.h"

WsSearch::WsSearch(FileSystemTreePtr fst, const string& query):
  m_fst(fst),
  m_searchImpl(0),
  m_query(query)
{
}

WsSearch::~WsSearch()
{
  delete m_searchImpl;
}

int WsSearch::load()
{
  /* Load module */
  if (loadModule() == -1)
    return -1;
  /* Load search results */
  return m_searchImpl->load(m_fst, m_query);
}

int WsSearch::getSize()
{
  return m_searchImpl->getSize();
}

vector<WsResultItem> WsSearch::getResults(const set<string>& groups)
{
  return m_searchImpl->getResults(groups);
}

int WsSearch::loadModule()
{
  LOG(INFO) << "WsSearch::loadModule() : Loading search module";
  /* Load libname */
  string libName = WsGlobalProperties::instance()->get("global", "search_libname", "libWsModMnoGoSearch.so");
#if GDDEBUG == 1
  /* If debug flag, load debug lib instead */
  boost::replace_first(libName, ".so", "d.so");
#endif
  /* Load module */
  void* hndl = dlopen(libName.c_str(), RTLD_LAZY);
  if ( hndl == NULL ) {
    LOG(ERROR) << "WsSearch::loadModule() : load ERROR " << dlerror();
    return -1;
  }
  LOG(DEBUG) << "WsSearch::loadModule() : Building module";
  /* Link function pointer to adequate function in module (buildModule function) */
  pf_wsSearchMod func = (pf_wsSearchMod) dlsym(hndl, "buildModule");
  LOG(DEBUG) << "WsSearch::loadModule() : Casting module to specified object";
  m_searchImpl = func();
  LOG(DEBUG) << "WsSearch::loadModule() : Done loading";
  return 0;
}
