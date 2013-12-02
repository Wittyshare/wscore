/*
 *
 *       Filename:  WsSearch.h
 *
 *    Description:
 *
 *        Created:  03/08/2012 02:35:21 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_SEARCH_H__
#define WS_SEARCH_H__


#include "WsResultItem.h"
#include "WsAbstractSearch.h"
#include <Logger/WsLogger.h>
#include <FileSystemTree/WsFileSystemTree.h>

#include <iostream>
#include <dlfcn.h>
#include <stdio.h>
#include <list>
#include <set>


/**
 * @brief Instanciates the search
 *
 * When a search is performed, this class is instanciated with the correct params, the search module is then loaded and search performed
 */
class WsSearch {
public:
  /**
   * @brief Constructor for the Search object
   * @param fst the file system tree in memory
   * @param query the search query to perform
   */
  WsSearch(FileSystemTreePtr fst, const std::string& query);

  ~WsSearch();

  /**
   * @brief Loads all search results
   * @return the number of items found, -1 in case of error
   */
  int load();
  /**
   * @brief Loads all search results
   * @return the total number (unfiltered by access right) of document corresponding to the search query
   *
   */
  int getSize();

  /**
   * @brief Get list of documents readable by person member of all groups passed in parameter
   * @param groups set of group names
   * @return an instance of list<ResultItem>
   *
   */
  vector<WsResultItem> getResults(const std::set<std::string>& groups);

  /**
   * @brief loads the search module.
   * @return -1 if failed, other value otherwise
   **/
  int loadModule();

private:
  WsAbstractSearch* m_searchImpl;
  FileSystemTreePtr m_fst;
  string m_query;


};
typedef WsAbstractSearch* (*pf_wsSearchMod) ();
#endif
