/*
*       @file  fileSystemTreeTest.cpp
*
*      @brief  File system tree test
*
*    @version  1.0
*       @date  07/05/11 15:24:00
*
*     Author: Benoit Daccache, ben.daccache@gmail.com
*
*/
#include "FileSystemTreeTest.h"

FileSystemTreeTest::FileSystemTreeTest(string p)
{
  this->p = p;
}

void FileSystemTreeTest::runTests()
{
  bool dbg = false;
  this->p = "../directories/test1";
  WsFileSystemTree t(p);
  t.start();
  WsAbstractNode* n = t.getRoot();
  assert(n->getPath() == "/");
  assert(n->getFullPath() == "../directories/test1");
  list<string> expectedNames;
  list<string> emptyList;
  // check properties are read for root node
  assert(((WsDirNode*)n)->getGroups().size() == 1);
  assert(((WsDirNode*)n)->getGroups().count("Guest") == 1);
  // put children directories list in l
  vector<WsAbstractNode*> l = n->getDirectories();
  // initialize iterator on vector
  vector<WsAbstractNode*>::iterator it = l.begin();
  vector<WsAbstractNode*>::iterator it2;
  vector<WsAbstractNode*> tmpList;
  // add true as third argument to print the vector of children
  //about directory
  expectedNames.push_back("company");
  expectedNames.push_back("countries");
  expectedNames.push_back("team");
  expectedDirectories(*it, 3, expectedNames, dbg);
  expectedNames.clear();
  expectedNames.push_back("index.html");
  expectedFiles(*it, 1, expectedNames, dbg);
  expectedNames.clear();
  // check properties are taken from directory itself
  assert(((WsDirNode*)(*it))->getGroups().size() == 2);
  assert(((WsDirNode*)(*it))->getGroups().count("group1") == 1);
  assert(((WsDirNode*)(*it))->getGroups().count("group3") == 1);
  assert(((WsDirNode*)(*it))->getDisplayInParent());
  assert(((WsDirNode*)(*it))->getDisplayInMenu());
  tmpList = (*it)->getDirectories();
  it2 = tmpList.begin();
  //company
  expectedDirectories((*it2), 0, emptyList, dbg);
  expectedNames.clear();
  expectedNames.push_back("index.html");
  expectedFiles((*it2), 1, expectedNames, dbg);
  // check properties are taken from directory itself
  assert(((WsDirNode*)(*it2))->getGroups().size() == 3);
  assert(((WsDirNode*)(*it2))->getGroups().count("group1") == 1);
  assert(((WsDirNode*)(*it2))->getGroups().count("group3") == 1);
  it2++;
  //countries
  expectedNames.clear();
  expectedNames.push_back("belgium");
  expectedNames.push_back("france");
  expectedNames.push_back("germany");
  expectedDirectories((*it2), 3, expectedNames, dbg);
  expectedNames.clear();
  expectedFiles((*it2), 0, emptyList, dbg);
  // check properties are taken from directory itself
  assert(((WsDirNode*)(*it2))->getGroups().size() == 1);
  assert(((WsDirNode*)(*it2))->getGroups().count("group1") == 1);
  //team
  it2++;
  expectedDirectories((*it2), 0, emptyList, dbg);
  expectedNames.clear();
  expectedNames.push_back("françois.html");
  expectedNames.push_back("herman.html");
  expectedFiles((*it2), 2, expectedNames, dbg);
  // check properties are taken from directory itself
  assert(((WsDirNode*)(*it2))->getGroups().size() == 2);
  assert(((WsDirNode*)(*it2))->getGroups().count("group1") == 1);
  assert(((WsDirNode*)(*it2))->getGroups().count("group3") == 1);
  it++;
  // products directory
  expectedDirectories(*it, 0, emptyList, dbg);
  expectedNames.clear();
  expectedNames.push_back("index.html");
  expectedFiles(*it, 1, expectedNames);
  expectedNames.clear();
  // check properties are taken from ancestor directory
  assert(((WsDirNode*)(*it))->getGroups().size() == 1);
  assert(((WsDirNode*)(*it))->getGroups().count("Guest") == 1);
  it++;
  assert((*it)->getPath().filename() == "references");
  tmpList = (*it)->getDirectories();
  it2 = tmpList.begin();
  //from
  // Check it goes to parent for rights and groups
  expectedDirectories((*it2), 0, emptyList, dbg);
  assert(((WsDirNode*)(*it2))->getName() == "from");
  assert(((WsDirNode*)(*it2))->getGroups().size() == 1);
  assert(((WsDirNode*)(*it2))->getGroups().count("Guest") == 1);
  //to
  it2++;
  expectedDirectories((*it2), 0, emptyList, dbg);
  assert(((WsDirNode*)(*it2))->getGroups().size() == 1);
  assert(((WsDirNode*)(*it2))->getGroups().count("Guest") == 1);
  set<string> groups;
  groups.insert("Guest");
  assert(((WsDirNode*)(*it2))->isAllowed(groups));
  it2++;
  assert(it2 == tmpList.end());
  it++;
  assert(it == l.end());
  //printNodeSub(t.getRoot());
  WsFileSystemTree* tree = new WsFileSystemTree("../directories");
  tree->start();
  WsAbstractNode* temp;
  assert(tree->eatPath("test1", &temp) ==  SUCCESS);
  assert(temp->getPath() == "/test1");
  assert(temp->getFullPath() == "../directories/test1");
  assert(tree->eatPath("test1/about/", &temp) ==  SUCCESS);
  assert(temp->getPath() == "/test1/about");
  assert(temp->getFullPath() == "../directories/test1/about");
  assert(tree->eatPath("test1/about/team", &temp) ==  SUCCESS);
  assert(temp->getPath() == "/test1/about/team");
  assert(temp->getFullPath() == "../directories/test1/about/team");
  assert(tree->eatPath("test1/about/team/herman.html", &temp) ==  SUCCESS);
  assert(temp->getPath() == "/test1/about/team/herman.html");
  assert(temp->getFullPath() == "../directories/test1/about/team/herman.html");
  assert(tree->eatPath("test1/about/team/herman2.html", &temp) ==  FAILURE);
  assert(tree->eatPath("test1/about/", &temp) ==  SUCCESS);
  vector<WsAbstractNode*> v = temp->getAll();
  assert(v.size() == 4);
  delete tree;
}


void FileSystemTreeTest::printChildrenDirectories(WsAbstractNode* n)
{
  vector<WsAbstractNode*> l = n->getDirectories();
  vector<WsAbstractNode*>::iterator it;
  for ( it = l.begin() ; it != l.end(); it++ ) {
    cout << (*it)->getPath() << endl;
  }
}
void FileSystemTreeTest::printChildrenFiles(WsAbstractNode* n)
{
  vector<WsAbstractNode*> l = n->getFiles();
  vector<WsAbstractNode*>::iterator it;
  for ( it = l.begin() ; it != l.end(); it++ ) {
    cout << (*it)->getPath() << endl;
  }
}
void FileSystemTreeTest::expectedDirectories(WsAbstractNode* n, int e, list<string> expectedNames, bool debug)
{
  if (debug) {
    cout << "Expected " << e << " children :" << endl;
    printChildrenDirectories(n);
  }
  assert( (n)->getDirectories().size() == e );
  validateDirChildrenList(n->getDirectories(), expectedNames);
}
void FileSystemTreeTest::expectedFiles(WsAbstractNode* n, int e, list<string> expectedNames, bool debug)
{
  if (debug) {
    cout << "Expected " << e << " children for :" << n->getName() << endl;
    printChildrenFiles(n);
  }
  assert( (n)->getFiles().size() == e );
  cout << ((WsDirNode*)n)->getGroups().size() << endl;
  cout << ((WsDirNode*)n)->getPath().string() << endl;
  validateFileChildrenList(n->getFiles(), expectedNames);
}
void FileSystemTreeTest::validateFileChildrenList(vector<WsAbstractNode*> children, list<string> expectedNames)
{
  list<string> names;
  // iterator on directories found
  vector<WsAbstractNode*>::iterator it = children.begin();
  // iterator on expected names
  list<string>::iterator it_x = expectedNames.begin();
  // extract directories names
  assert(children.size() == expectedNames.size());
  for (; it != children.end(); it++) {
    names.push_back((*it)->getName());
  }
  // sort names
  names.sort();
  // compare with expected names
  list<string>::iterator it_n = names.begin();
  for (; it_n != names.end(); it_n++, it_x++) {
    //cout << (*it_n) << "==?" << (*it_x) << endl;
    assert((*it_n) == (*it_x));
  }
}

void FileSystemTreeTest::validateDirChildrenList(vector<WsAbstractNode*> children, list<string> expectedNames)
{
  list<string> names;
  // iterator on directories found
  vector<WsAbstractNode*>::iterator it = children.begin();
  // iterator on expected names
  list<string>::iterator it_x = expectedNames.begin();
  /*
  // For debugging
  for (; it != children.end(); it++)
  {
  cout << "received name " << (*it)->getName() << endl;
      names.push_back((*it)->getName());
  }
  */
  /*
  // for debug
  list<string>::iterator it_d = expectedNames.begin();
  for (; it_d != expectedNames.end(); it_d++)
  {
  cout << "expected name " << (*it_d) << endl;
  }
  */
  // extract directories names
  assert(children.size() == expectedNames.size());
  // sort names
  names.sort();
  // compare with expected names
  list<string>::iterator it_n = names.begin();
  for (; it_n != names.end(); it_n++, it_x++) {
    //cout << (*it_n) << "==" << (*it_x) << endl;
    assert((*it_n) == (*it_x));
  }
}
