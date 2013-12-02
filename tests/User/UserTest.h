/**
 *
 *       @file  userTest.h
 *
 *      @brief  Test class for User
 *
 *    @version  1.0
 *       @date  07/11/11 10:46:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef USER_TEST_H__
#define USER_TEST_H__
#include <User/WsUser.h>
#include <assert.h>
class UserTest {
public:
  UserTest();
  void runTests();
private:
  WsUser* user;
};
#endif
