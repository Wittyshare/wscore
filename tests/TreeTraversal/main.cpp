/**
 *
 *       @file  main.cpp
 *
 *      @brief Tests main class
 *
 *    @version  1.0
 *       @date  07/08/11 10:53:59
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#include "TreeTraversalTest.h"
#include <FileSystemTree/WsFileSystemTree.h>

int main()
{
  WsFileSystemTree* f = new WsFileSystemTree("../directories");
  f->start();
  TreeTraversalTest t(f->getRoot());
  t.runTests();
  delete f;
  cout << "All Tree Traversal Tests succeeded" << endl;
  return 0;
}
