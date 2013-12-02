/**
 *
 *       @file  main.cpp
 *
 *      @brief  Main class for MenuTreeTest
 *
 *    @version  1.0
 *       @date  07/08/11 13:42:12
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#include "MenuTreeTest.h"
#include <FileSystemTree/WsFileSystemTree.h>
#include <set>
#include <iostream>
using namespace std;
int main()
{
  set<string> s;
  s.insert("Guest");
  s.insert("group1");
  WsFileSystemTree* fs = new WsFileSystemTree("../directories");
  fs->start();
  MenuTreeTest* t = new MenuTreeTest(fs, s);
  t->runTests();
  delete fs;
  cout << "All menuTree tests succeeded" << endl;
  return 0;
}
