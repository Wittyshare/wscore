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
#include <FileSystemTree/WsFileSystemTree.h>
#include <iostream>
using namespace std;

int main()
{
  while (1) {
    WsFileSystemTree* tree = new WsFileSystemTree("/var/www/web03");
    sleep(2);
    tree->start();
    delete tree;
  }
  cout << "FileSystemTree tests Succeeded" << endl;
}
