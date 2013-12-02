/**
 *
 *       @file  main.cpp
 *
 *      @brief  Main files for tests
 *
 *    @version  1.0
 *       @date  07/05/11 14:53:03
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#include "NodeTest.h"
#include "FileSystemTreeTest.h"
#include <iostream>
using namespace std;

int main()
{
  NodeTest t;
  t.runTests();
  cout << "Node tests Succeeded" << endl;
  FileSystemTreeTest ft("");
  ft.runTests();
  cout << "FileSystemTree tests Succeeded" << endl;
}
