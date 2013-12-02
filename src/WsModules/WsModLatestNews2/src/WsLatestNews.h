/*
 *
 *       Filename:  WsLatestNews.h
 *
 *    Description:
 *
 *        Created:  04/18/2012 12:34:02 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_LATEST_NEWS_H__
#define WS_LATEST_NEWS_H__

#include <FileSystemTree/WsFileSystemTree.h>
#include <Tree/WsAccessTree.h>

class WsLatestNews: public WsTreeTraversal {
public:
  WsLatestNews(NodePtr mt);

  vector<NodePtr> getLatestNews(int nb);


private:
  int beginTraverseDir(NodePtr n);
  int endTraverseDir(NodePtr n);
  int traverseFile(NodePtr n);

  vector<NodePtr> m_vect;
  int m_maxSize;


};

#endif
