/*
 *
 *       Filename:  WsAccessTreeDisplayer.h
 *
 *    Description:
 *
 *        Created:  02/23/12 09:20:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ACCESS_TREE_DISPLAYER_H__
#define WS_ACCESS_TREE_DISPLAYER_H__

#include <FileSystemTree/WsFileSystemTree.h>
#include <Serializer/WsTreeSerializer.h>
#include <Tree/WsAccessTree.h>
#include <Auth/WsAuthenticator.h>
#include <ConfigManager/WsGlobalProperties.h>

#include <iostream>
#include <set>
using namespace std;

#include <getopt.h>


class WsAccessTreeDisplayer {
public:

  WsAccessTreeDisplayer(string root);
  ~WsAccessTreeDisplayer();

  string display(const string& uid, const string& pass);
  string display(const set<string>& groups);

private:
  FileSystemTreePtr fs;
};

#endif

