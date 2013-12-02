/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Tests for the directoryCrawler
 *
 *        Version:  1.0
 *        Created:  07/05/2011 11:26:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:
 *        Company:
 *
 * =====================================================================================
 */
#include "DirectoryCrawlerTests.h"
#include <boost/filesystem.hpp>
#include <assert.h>
int main()
{
  cout << "Testing count on directory test1" << endl;
  DirectoryCrawlerTests* tests = new DirectoryCrawlerTests("../directories/test1");
  tests->start();
  assert(tests->fileCount == 10);
  assert(tests->directoryCount == 12);
  cout << "Success" << endl ;
  delete tests;
  cout << "Testing count on directory test2 (hidden files)" << endl;
  tests = new DirectoryCrawlerTests("../directories/test2");
  tests->start();
  assert(tests->fileCount == 1);
  //cout << "Number of dirs: "<< tests->directoryCount << endl ;
  assert(tests->directoryCount == 2);
  cout << "Success" << endl ;
  delete tests;
}
