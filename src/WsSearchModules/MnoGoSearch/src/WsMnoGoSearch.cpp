/**
 *
 *       Filename:  WsMnoGoSearch.cpp
 *
 *    Description:  Search the documents and filter according to user rights
 *
 *        Version:  1.0
 *        Created:  07/11/2011 01:56:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 */
#include "WsMnoGoSearch.h"

extern "C" {
  void WsMnoGoSearchInit(void)
  { }
}

WsMnoGoSearch::WsMnoGoSearch()
  : WsAbstractSearch()
{
  void* m_hndlMno = dlopen("libmnogosearch-3.3.so", RTLD_LAZY);
  if ( m_hndlMno == NULL ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : Cannot load libmnogosearch.so shared library! " << dlerror();
    return;
  }
  if ( !(mf_UdmEnvInit = (pf_UdmEnvInit) dlsym(m_hndlMno, "UdmEnvInit")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmEnvInit function " << dlerror();
    return;
  }
  if ( !(mf_UdmAgentInit = (pf_UdmAgentInit) dlsym(m_hndlMno, "UdmAgentInit")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmAgentInit function " << dlerror();
    return;
  }
  if ( !(mf_UdmAgentAddLine = (pf_UdmAgentAddLine) dlsym(m_hndlMno, "UdmAgentAddLine")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmAgentAddLine function " << dlerror();
    return;
  }
  if ( !(mf_UdmFind2 = (pf_UdmFind2) dlsym(m_hndlMno, "UdmFind2")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmFind2 function " << dlerror();
    return;
  }
  if ( !(mf_UdmVarListFindStr = (pf_UdmVarListFindStr) dlsym(m_hndlMno, "UdmVarListFindStr")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmVarListFindStr function " << dlerror();
    return;
  }
  if ( !(mf_UdmVarListFindInt = (pf_UdmVarListFindInt) dlsym(m_hndlMno, "UdmVarListFindInt")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmVarListFindInt function " << dlerror();
    return;
  }
  if ( !(mf_UdmResultFree = (pf_UdmResultFree) dlsym(m_hndlMno, "UdmResultFree")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmResultFree function " << dlerror();
    return;
  }
  if ( !(mf_UdmAgentFree = (pf_UdmAgentFree) dlsym(m_hndlMno, "UdmAgentFree")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmAgentFree function " << dlerror();
    return;
  }
  if ( !(mf_UdmEnvFree = (pf_UdmEnvFree) dlsym(m_hndlMno, "UdmEnvFree")) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : cannot dlsym on UdmEnvFree function " << dlerror();
    return;
  }
  if ( !mf_UdmEnvInit(&m_env) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : Cannot mf_UdmEnvInit";
    return;
  }
  if ( !mf_UdmAgentInit(&m_agent, &m_env, 0) ) {
    LOG(ERROR) << "WsMnoGoSearch::WsMnoGoSearch() : Cannot mf_UdmAgentInit";
    return;
  }
  //TODO FIX PATH HERE ??
  if (UDM_OK != mf_UdmAgentAddLine(&m_agent, "DBAddr sqlite3:///var/www/wt/resources/wsdb.sqlite/"))
    return;
  if (UDM_OK != mf_UdmAgentAddLine(&m_agent, "LocalCharset   UTF-8"))
    return;
  if (UDM_OK != mf_UdmAgentAddLine(&m_agent, "BrowserCharset UTF-8"))
    return;
  LOG(INFO) << "WsMnoGoSearch::WsMnoGoSearch() :  completed";
  return;
}

WsMnoGoSearch::~WsMnoGoSearch()
{
  mf_UdmResultFree(m_res);
  mf_UdmAgentFree(&m_agent);
  mf_UdmEnvFree(&m_env);
}

int WsMnoGoSearch::load(FileSystemTreePtr fst, string q)
{
  m_fst = fst;
  LOG(DEBUG) << "WsMnoGoSearch::load() : Loading results for " << q;
  snprintf(m_udmQuery, sizeof(m_udmQuery), "q=%s", q.c_str());
  LOG(INFO) << "WsMnoGoSearch::load() : Load m_udmQuery = " << m_udmQuery;
  if ( !(m_res = mf_UdmFind2(&m_agent, m_udmQuery)) ) {
    LOG(INFO) << "WsMnoGoSearch::load() : Load cannot UdmFind2 ";
    return -2;
  }
  LOG(DEBUG) << "WsMnoGoSearch::load() : Filesystem root : " << m_fst->getRootPath().string();
  for (int row = 0; row < m_res->num_rows; ++row) {
    UDM_DOCUMENT* Doc = &m_res->Doc[row];
    string url(mf_UdmVarListFindStr(&Doc->Sections, "URL", ""));
    boost::algorithm::replace_all(url, "%26",  "&");
    boost::algorithm::replace_all(url, "%20",  " ");
    // remove the file:// prefix
    //string test = url.substr(7 + m_fst->getRootPath().string().size());
    path fp(url.substr(7));
    LOG(DEBUG) << "WsMnoGoSearch::load() : PATH is " << fp.string();
    /* Create the result item with adequate informations */
    WsResultItem item(fp, m_fst->getRootPath(),
                      mf_UdmVarListFindStr(&Doc->Sections, "body", ""),
                      mf_UdmVarListFindStr(&Doc->Sections, "Content-Type", ""),
                      mf_UdmVarListFindInt(&Doc->Sections, "Content-Length", 0));
    m_results.push_back(item);
  }
  return m_res->num_rows;
}

int WsMnoGoSearch::getSize()
{
  return m_size;
}

vector<WsResultItem> WsMnoGoSearch::getResults(const set<string>& groups)
{
  string path;
  vector<WsResultItem> filteredResults;
  list<WsResultItem>::iterator it;
  NodePtr n;
  LOG(DEBUG) << "WsMnoGoSearch::getResults() : Total found results before filter " << m_results.size();
  for (it = m_results.begin(); it != m_results.end(); ++it) {
    /* remove file://tmp/directories from path*/
    path = (*it).getPath().string();
    /* Get the node associated with the result */
    n = m_fst->eatPath(path);
    if (n.get() != 0) {
      /* If it's a directory, skip it */
      if ( n.get()->isDirectory() ) continue;
      /* Check if user if allowed to access this node */
      if (n.get()->isAllowed(groups)) {
        string newPath = (*it).getFullPath().string();
        std::time_t t = boost::filesystem::last_write_time(newPath);
        (*it).setModifyDate(t);
        (*it).setSize(boost::filesystem::file_size(newPath));
        filteredResults.push_back(*it);
      }
    } else
      LOG(ERROR) << "WsMnoGoSearch::getResults() : Error finding node " << path << endl;
  }
  LOG(DEBUG) << "WsMnoGoSearch :: Total found results after filter " << m_results.size();
  return filteredResults;
}

