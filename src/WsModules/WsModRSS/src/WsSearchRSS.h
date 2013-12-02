/*
 *
 *       Filename:  WsSearchRSS.h
 *
 *    Description:
 *
 *        Created:  04/18/2012 12:34:02 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef __WsSearchRSS_H_
#define __WsSearchRSS_H_

#include <FileSystemTree/WsFileSystemTree.h>
#include <Tree/WsAccessTree.h>

class WsSearchRSS: public WsTreeTraversal {
public:
  WsSearchRSS(NodePtr mt);

  vector<NodePtr> getRSS(int nb);


private:
  int beginTraverseDir(NodePtr n);
  int endTraverseDir(NodePtr n);
  int traverseFile(NodePtr n);

  vector<NodePtr> m_vect;
  int m_maxSize;


};

#endif
