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
#include <User/WsUser.h>
#include <iostream>
#include <assert.h>
using namespace std;

const int nbUsers = 25;
vector<WsUser*> users;
/**
 * @brief Creates and loads the user
 *
 * @param nb
 *
 * @return
 */
void createUsers(int nb)
{
  for (int i = 0; i < nb; ++i) {
    string index = boost::lexical_cast<std::string>(i + 1);
    users.push_back( new WsUser("guest", "", "127.0.0." + index));
    users[i]->load();
  }
}


int subRoutine(int index)
{
  for (int i = index * 5; i < index * 5 + 5; ++i) {
    NodePtr accessTree = users[i]->getAccessRoot();
    assert(accessTree.get() != 0);
    assert(accessTree.get()->getPath() == "/");
    assert(users[i]->getMenuRoot() != 0);
    assert(users[i]->getProperties("Issues&Positions/Transport/Events") != 0);
    assert(users[i]->getPermissions("Issues&Positions/Transport/Events") == 2);
    assert(users[i]->getPermissions("not/found") == -3);
    assert(users[i]->getSearchResults("steel").size() > 0);
    assert(users[i]->isEditor() == false) ;
    assert(users[i]->isAdministrator() == false);
    delete users[i];
  }
}

int main()
{
  struct timeval start, end;
  long mtime, seconds, useconds;
  gettimeofday(&start, NULL);
  createUsers(nbUsers);
  boost::thread* tmp = 0;
  for (int i = 0; i < (nbUsers / 5); ++i) {
    cout << "Creating Thread " << i << endl;
    tmp = new boost::thread(boost::bind(subRoutine, i));
  }
  tmp->join();
  gettimeofday(&end, NULL);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds) * 1000 + useconds / 1000.0) + 0.5;
  cout << "Elapsed time: " << mtime << "  milliseconds" << endl;
  return 0;
}


