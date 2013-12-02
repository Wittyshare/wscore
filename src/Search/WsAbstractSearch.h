/*
 *
 *       Filename:  WsAbstractSearch.h
 *
 *    Description:
 *
 *        Created:  03/08/2012 02:22:18 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_ABSTRACT_SEARCH_H__
#define WS_ABSTRACT_SEARCH_H__


#include "WsResultItem.h"
#include <Logger/WsLogger.h>
#include <FileSystemTree/WsFileSystemTree.h>

#include <iostream>
#include <stdio.h>
#include <list>
#include <set>


/**
 * @brief Abstract class for search.
 *
 * Search module must inherit this class.
 */
class WsAbstractSearch {

public:
  /**
   * @brief virtual destructor
   **/
  virtual ~WsAbstractSearch();

  /**
   * @brief load the results (execute the search query)
   * @return SUCCESS if ok, failure otherwise
   **/
  virtual int load(FileSystemTreePtr fs, std::string q) = 0;

  /**
   * @brief return the number of results found
   **/
  virtual int getSize() = 0;

  /**
   * @brief return the results found for the query filtered for the user
   * @param groups the groups which the user belongs to
   * @return a vector<WsResultItem> containing the results.
   * An empty vector is returned if no results found or if the
   * user don't have access to any found result
   **/
  virtual vector<WsResultItem> getResults(const std::set<std::string>& groups) = 0;

};

#endif


