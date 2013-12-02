/**
 *
 *       @file  userTest.cpp
 *
 *      @brief  Test class for User
 *
 *    @version  1.0
 *       @date  07/11/11 10:46:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "UserTest.h"


UserTest::UserTest()
{
  user = new WsUser("angelique");
}

void UserTest::runTests()
{
  assert(user->load() == SUCCESS);
  assert(user->getEmail().size() != 0);
  assert(user->getSurname().size() != 0);
  assert(user->getName().size() != 0);
  WsAbstractNode* root = user->getMenuRoot();
  WsAbstractNode* usertestdir;
  assert(root->getNodeByName("usertest_dir", &usertestdir) == SUCCESS);
  assert(usertestdir->getAll().size() == 3);
  assert(usertestdir->getFiles().size() == 1);
  assert(usertestdir->getDirectories().size() == 2);
  WsAbstractNode* temp;
  assert(usertestdir->getNodeByName("dir1", &temp) == SUCCESS);
  assert(usertestdir->getNodeByName("dir2", &temp) == SUCCESS);
  assert(usertestdir->getNodeByName("dir3", &temp) == FAILURE);
  assert(usertestdir->getNodeByName("file1", &temp) == SUCCESS);
  WsAbstractNode* dir1;
  assert(usertestdir->getNodeByName("dir1", &dir1) == SUCCESS);
  assert(dir1->getAll().size() == 1);
  assert(dir1->getFiles().size() == 1);
  assert(dir1->getDirectories().size() == 0);
  WsAbstractNode* dir2;
  assert(usertestdir->getNodeByName("dir2", &dir2) == SUCCESS);
  assert(dir2->getAll().size() == 4);
  assert(dir2->getFiles().size() == 3);
  assert(dir2->getDirectories().size() == 1);
  WsAbstractNode* subdir1;
  assert(dir2->getNodeByName("subdir1", &subdir1) == SUCCESS);
  assert(subdir1->getAll().size() == 2);
  assert(subdir1->getFiles().size() == 2);
  assert(subdir1->getDirectories().size() == 0);
  delete user;
}
