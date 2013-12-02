/**
 *
 *       @file  TreeTraversalTest.h
 *
 *      @brief  Test class for TreeTraversal
 *
 *    @version  1.0
 *       @date  07/08/11 10:53:59
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef TREE_TRAVERSAL_TEST_H__
#define TREE_TRAVERSAL_TEST_H__
#include <TreeTraversal/WsTreeTraversal.h>

class TreeTraversalTest: public WsTreeTraversal {
public:
  TreeTraversalTest(WsAbstractNode* f);
  void runTests();
private:
  int beginTraverseDir(WsAbstractNode* n);
  int endTraverseDir(WsAbstractNode* n);
  int traverseFile(WsAbstractNode* n);
  int nbDir;
  int nbDirF;
  int nbFile;
};
#endif
