/**
 *
 *       @file  menuTreeTest.cpp
 *
 *      @brief  Test class for MenuTree
 *
 *    @version  1.0
 *       @date  07/08/11 13:42:44
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "MenuTreeTest.h"

MenuTreeTest::MenuTreeTest(WsFileSystemTree* fs, set<string> gid)
{
  this->fsTree = fs;
  menuTree = new WsMenuTree(fs, gid, 0);
}

void MenuTreeTest::runTests()
{
  menuTree->build();
  WsTreeSerializer mts(menuTree);
  assert(mts.serialize() == SUCCESS);
  WsTreeDeserializer mtds(mts.getSerializedForm());
  assert(mtds.deserialize() == SUCCESS);
  cout << "Path for root in " << mtds.getMenuRoot()->getFullPath() << endl;
  assert(mtds.getMenuRoot()->getPath() == "/");
  assert( mtds.getMenuRoot()->getDirectories().size() == 2);
  WsDirNode* root = static_cast<WsDirNode*>(mtds.getMenuRoot());
  WsDirNode* test1 = static_cast<WsDirNode*>(root->getDirectories().front());
  assert(test1->getName() == "test1");
  assert(test1->getDirectories().size() == 3);
  WsDirNode* test3 = static_cast<WsDirNode*>(root->getDirectories().back());
  assert(test3->getName() == "test3");
  assert(test3->getDirectories().size() == 0);
  set<string> groups;
  groups.insert("Guest");
  string p("test1/");
  WsDirContentsSerializer dcs(fsTree , groups, p );
  assert(dcs.serialize() == SUCCESS);
  WsDirContentsDeserializer dcds(dcs.getSerializedForm());
  dcds.deserialize();
  assert(dcds.getContents().size() == 2);
  WsDirNode* products = static_cast<WsDirNode*>(dcds.getContents().front());
  assert( products->getName() == "products");
  WsDirNode* references = static_cast<WsDirNode*>(dcds.getContents().back());
  assert( references->getName() == "references");
  assert(products->getDirectories().size() == 0);
}

