/**
 *
 *       @file  main.cpp
 *
 *      @brief  Main class for tests
 *
 *    @version  1.0
 *       @date  07/11/11 10:46:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 *
 */
#include "UserTest.h"
int main()
{
  UserTest test;
  test.runTests();
  cout << "All user tests succeded" << endl;
  return 0;
}
