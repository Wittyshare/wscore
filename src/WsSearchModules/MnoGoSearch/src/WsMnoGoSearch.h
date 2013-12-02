/*
 *       Filename:  WsMnoGoSearch.h
 *
 *    Description:  Class used to search content thanks to mnogosearch
 *
 *        Version:  1.0
 *        Created:  07/11/2011 01:44:44 PM
 */

#ifndef WS_MNOGOSEARCH_H__
#define WS_MNOGOSEARCH_H__

#include <dlfcn.h>

extern "C" {
#include <udmsearch.h>
#include <udm_utils.h>
}

#include <Search/WsAbstractSearch.h>

typedef UDM_ENV*    (*pf_UdmEnvInit)        (UDM_ENV* Env);
typedef UDM_AGENT*  (*pf_UdmAgentInit)      (UDM_AGENT* Agent, UDM_ENV* Env, int id);
typedef int         (*pf_UdmAgentAddLine)   (UDM_AGENT* Agent, const char* line);
typedef UDM_RESULT* (*pf_UdmFind2)          (UDM_AGENT* Agent, const char* query);
typedef int         (*pf_UdmVarListFindInt) (UDM_VARLIST* vars, const char* name, const int defval);
typedef const char* (*pf_UdmVarListFindStr) (UDM_VARLIST* vars, const char* name, const char* defval);
typedef void        (*pf_UdmResultFree)     (UDM_RESULT* Result);
typedef void        (*pf_UdmAgentFree)      (UDM_AGENT* Agent);
typedef void        (*pf_UdmEnvFree)        (UDM_ENV* Env);

class WsMnoGoSearch: public WsAbstractSearch {
public:
  /**
   * @brief Constructor for the Search object
   * @param fst the file system tree in memory
   * @param query the search query to perform
   */
  WsMnoGoSearch();

  virtual ~WsMnoGoSearch();

  /**
   * @brief Loads all search results
   * @param fst the instance of WsFileSystemTree
   * @param query the terms we are looking for
   * @return the number of items found, -1 in case of error
   */
  int load(FileSystemTreePtr fst, string query);

  /**
   * @brief Loads all search results
   * @return the total number (unfiltered by access right) of document corresponding to the search query
   */
  int getSize();

  /**
   * @brief Get list of documents readable by person member of all groups passed in parameter
   * @param groups set of group names
   * @return an instance of list<ResultItem>
   */
  vector<WsResultItem> getResults(const set<string>& groups);

private:
  FileSystemTreePtr  m_fst;
  char               m_udmQuery[256];
  string             m_user;
  int                m_size;
  list<WsResultItem> m_results;

  /* mnogosearch variable */
  void*                m_hndlMno;
  pf_UdmEnvInit        mf_UdmEnvInit;
  pf_UdmAgentInit      mf_UdmAgentInit;
  pf_UdmAgentAddLine   mf_UdmAgentAddLine;
  pf_UdmFind2          mf_UdmFind2;
  pf_UdmVarListFindInt mf_UdmVarListFindInt;
  pf_UdmVarListFindStr mf_UdmVarListFindStr;
  pf_UdmResultFree     mf_UdmResultFree;
  pf_UdmAgentFree      mf_UdmAgentFree;
  pf_UdmEnvFree        mf_UdmEnvFree;

  UDM_ENV              m_env;
  UDM_AGENT            m_agent;
  UDM_RESULT*          m_res;
};

extern "C" {
  void WsMnoGoSearchInit(void) __attribute__((constructor));
  WsMnoGoSearch* buildModule()
  {
    return new WsMnoGoSearch();
  }
}

#endif
