/*
 *
 *       Filename:  WsAccessTreeDisplayer.cpp
 *
 *    Description:
 *
         Created:  02/23/12 09:20:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAccessTreeDisplayer.h"

WsAccessTreeDisplayer::WsAccessTreeDisplayer(string root)
{
  fs = FileSystemTreePtr(new WsFileSystemTree(root, false));
  fs->start();
}

WsAccessTreeDisplayer::~WsAccessTreeDisplayer()
{
}

string WsAccessTreeDisplayer::display(const string& uid, const string& pass)
{
  WsAuthenticator ldap;
  if (ldap.authentify(uid, pass, "127.0.0.1") == FAILURE)
    return "";
  WsAccessTree mt(fs, ldap.getUserGroups(), 0, fs->getRootPath(), fs.get()->getStamp());
  if (mt.build() == FAILURE)
    return "";
  WsTreeSerializer ser(&mt);
  if (ser.serialize() == FAILURE)
    return "";
  return ser.getSerializedForm();
}

string WsAccessTreeDisplayer::display(const set<string>& groups)
{
  WsAccessTree mt(fs, groups, 0, fs->getRootPath(), fs.get()->getStamp());
  if (mt.build() == FAILURE)
    return "";
  WsTreeSerializer ser(&mt);
  if (ser.serialize() == FAILURE)
    return "";
  return ser.getSerializedForm();
}

void usage()
{
  cout << "Usage: " << endl;
  cout << "[-u | --uid    ] value : user uid" << endl;
  cout << "[-g | --groups ] value : group " << endl;
  cout << "[-r | --Root   ] value : root path" << endl;
  cout << "[-h | --help   ]       : display this help message" << endl;
  exit(0);
}


int main(int argc, char** argv)
{
  static struct option long_options[] = {
    {"uid", optional_argument, 0, 'u'},
    {"uid", optional_argument, 0, 'p'},
    {"group", optional_argument, 0, 'g'},
    {"root", required_argument, 0, 'r'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };
  int option_index = 0;
  char c;
  string uid;
  string pass;
  string root;
  set<string> grps;
  while ((c = getopt_long (argc, argv, "u:p:r:g:", long_options, &option_index)) != -1) {
    switch (c) {
    case 'r':
      root = string((char*)optarg);
      break;
    case 'u':
      uid = string((char*)optarg);
      break;
    case 'p':
      pass = string((char*)optarg);
      break;
    case 'g':
      grps.insert(string((char*)optarg));
      break;
    case 'h':
      usage();
    default:
      usage();
    }
  }
  if (uid == "" && grps.size() == 0) {
    cout << "Please provide uid or groups" << endl;
    usage();
  }
  if (root == "") {
    root = WsGlobalProperties::instance()->get("global", "root_path", "/");
  }
  string ret;
  {
    WsAccessTreeDisplayer disp(root);
    if (grps.size() == 0)
      ret = disp.display(uid, pass);
    else
      ret = disp.display(grps);
  }
  cout << ret << endl;
}
