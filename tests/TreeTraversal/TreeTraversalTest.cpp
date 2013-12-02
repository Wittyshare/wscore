/**
 *
 *       @file  treeTraversalTest.cpp
 *
 *      @brief  Test class for treeTraversal
 *
 *    @version  1.0
 *       @date  07/08/11 10:53:59
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "TreeTraversalTest.h"

TreeTraversalTest::TreeTraversalTest(WsAbstractNode* f): WsTreeTraversal(f)
{
  nbDir = 0;
  nbDirF = 0;
  nbFile = 0;
}

void TreeTraversalTest::runTests()
{
  start();
  assert(nbDir == 15);
  assert(nbFile == 10);
  assert(nbDir - nbDirF == 0);
}

int TreeTraversalTest::beginTraverseDir(WsAbstractNode* n)
{
  if (n->getParent()  != NULL) {
    assert(n->getFullPath().parent_path().string() == n->getParent()->getFullPath().string());
    nbDir++;
  }
  return SUCCESS;
}

int TreeTraversalTest::endTraverseDir(WsAbstractNode* n)
{
  if (n->getParent()  != NULL) {
    assert(n->getFullPath().parent_path().string() == n->getParent()->getFullPath().string());
    nbDirF++;
  }
  return SUCCESS;
}

int TreeTraversalTest::traverseFile(WsAbstractNode* n)
{
  nbFile++;
  return SUCCESS;
}
