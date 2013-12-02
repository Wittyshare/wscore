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
#include <Search/WsSearch.h>

int main()
{
  int n;
  //search for software and list results for different groups to check
  WsFileSystemTree* fst;
  // we put our test fs in /tmp/directories
  fst = new WsFileSystemTree("/var/www/wt/resources/wittyshare/demo_site");
  fst->start();
  // search documents containing the word software
  string terms = "software";
  WsSearch s(fst, terms);
  n = s.load();
  // complete list has 4 elements
  assert(n == 4);
  vector<WsResultItem>::iterator it;
  list<string>::iterator path_it;
  set<string> user_groups;
  // A user with no group has access to no file.
  vector<WsResultItem> l = s.getResults(user_groups);
  assert(l.size() == 0);
  // Admin group has access to 1 file
  user_groups.insert("Admin");
  l = s.getResults(user_groups);
  set<string> paths;
  assert(l.size() == 1);
  for (it = l.begin(); it != l.end(); it++) {
    paths.insert((*it).getPath().string());
  }
  assert(paths.count("/communication/deployment.pdf") == 1);
  assert(paths.count("/participants/software.pdf") == 0);
  assert(paths.count("/EWG/software_development.pdf") == 0);
  assert(paths.count("/participants/rms.pdf") == 0);
  // members of both Admin and EWGReport should see 2 documents
  user_groups.insert("EWGReport");
  l = s.getResults(user_groups);
  assert(l.size() == 2);
  //check that we get the correct paths
  for (it = l.begin(); it != l.end(); it++) {
    paths.insert((*it).getPath().string());
  }
  assert(paths.count("/communication/deployment.pdf") == 1);
  assert(paths.count("/participants/software.pdf") == 0);
  assert(paths.count("/EWG/software_development.pdf") == 1);
  assert(paths.count("/participants/rms.pdf") == 0);
  // people in all groups  RawMaterialsQuest Admin and EWGReport should see 4 documents
  user_groups.insert("RawMaterialsQuest");
  l = s.getResults(user_groups);
  assert(l.size() == 4);
  //check that we get the correct paths
  for (it = l.begin(); it != l.end(); it++) {
    paths.insert((*it).getPath().string());
  }
  assert(paths.count("/communication/deployment.pdf") == 1);
  assert(paths.count("/participants/software.pdf") == 1);
  assert(paths.count("/EWG/software_development.pdf") == 1);
  assert(paths.count("/participants/rms.pdf") == 1);
  //cout << "got results" << endl;
  //for (it=l.begin();it!=l.end();it++){
  //	cout << "result url: " << it->getPath() << endl;
  //	cout << "result body: " << it->getBody() << endl;
  //	cout << "result size: " << it->getSize() << endl;
  //}
  cout << "search tests Succeeded" << endl;
}
