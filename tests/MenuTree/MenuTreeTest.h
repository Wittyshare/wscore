/**
 *
 *       @file  MenuTreeTest.h
 *
 *      @brief  Test class for MenuTree
 *
 *    @version  1.0
 *       @date  07/08/11 13:42:45
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef MENU_TREE_TEST_H__
#define MENU_TREE_TEST_H__
#include <MenuTree/WsMenuTree.h>
#include <MenuTree/WsTreeSerializer.h>
#include <MenuTree/WsTreeDeserializer.h>
#include <MenuTree/WsDirContentsSerializer.h>
#include <MenuTree/WsDirContentsDeserializer.h>

class MenuTreeTest {
public:
  MenuTreeTest(WsFileSystemTree* fs, set<string> gid);
  void runTests();
private:
  WsMenuTree* menuTree;
  WsFileSystemTree* fsTree;
};

#endif
