/**
 *
 *       @file  FileSystemTreeTest.h
 *
 *      @brief  File system tree test
 *
 *    @version  1.0
 *       @date  07/05/11 15:24:00
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef FILE_SYSTEM_TREE_TEST_H__
#define FILE_SYSTEM_TREE_TEST_H__
#include <FileSystemTree/WsFileSystemTree.h>
#include <iostream>
#include <list>
using namespace std;

class FileSystemTreeTest {
public:
  FileSystemTreeTest(string p);
  void printChildrenDirectories(WsAbstractNode* n);
  void printChildrenFiles(WsAbstractNode* n);
  // args: node, number of children, ordered list of expected children names, debug? ie do not assert but print
  void expectedDirectories(WsAbstractNode* n, int e, list<string> l, bool debug = false);
  void expectedFiles(WsAbstractNode* n, int e, list<string> l,  bool debug = false);
  void validateFileChildrenList(vector<WsAbstractNode*> children, list<string> expectedNames);
  void validateDirChildrenList(vector<WsAbstractNode*> children, list<string> expectedNames);
  void runTests();
private:
  string p;

};

#endif
