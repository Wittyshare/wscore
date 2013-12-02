/*
 * =====================================================================================
 *
 *       Filename:  TestLogger.cpp
 *
 *    Description:  a
 *
 *        Created:  02/15/12 16:17:47
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 * =====================================================================================
 */

#include <Logger/WsLogger.h>
#include <pthread.h>

void* thread1(void* arg)
{
  int i = 0;
  while (i < 10) {
    LOG(DEBUG) << "this is thread 1" << " " << endl;
    LOG(INFO) << "this is thread 1" << " " << endl;
    LOG(WARN) << "this is thread 1" << " " << endl;
    LOG(ERROR) << "this is thread 1" << " " << endl;
    i++;
  }
  return NULL;
}

void* thread2(void* arg)
{
  int i = 0;
  while (i < 10) {
    LOG(DEBUG) << "this is thread 2" << " " << endl;
    LOG(INFO) << "this is thread 2" << " " << endl;
    LOG(WARN) << "this is thread 2" << " " << endl;
    LOG(ERROR) << "this is thread 2" << " " << endl;
    i++;
  }
  return NULL;
}

int main()
{
  pthread_t t1, t2;
  pthread_create(&t1, NULL, thread1, NULL);
  pthread_create(&t2, NULL, thread2, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}


