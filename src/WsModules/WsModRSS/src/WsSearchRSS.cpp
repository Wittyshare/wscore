/*
 *
 *       Filename:  WsSearchRSS.cpp
 *
 *    Description:
 *
 *        Created:  04/18/2012 12:34:02 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsSearchRSS.h"


WsSearchRSS::WsSearchRSS(NodePtr mt):
  WsTreeTraversal(mt)
{
}

vector<NodePtr> WsSearchRSS::getRSS(int nb)
{
  m_maxSize = nb;
  start();
  return m_vect;
}

int WsSearchRSS::beginTraverseDir(NodePtr n)
{
  return SUCCESS;
}

int WsSearchRSS::endTraverseDir(NodePtr n)
{
  return SUCCESS;
}

int WsSearchRSS::traverseFile(NodePtr n)
{
  WsFileNode* fn = dynamic_cast<WsFileNode*>(n.get());
  if ( fn->getProperties().get()->get("global", "in_view", "true") == "false"   ) return SUCCESS;
  // TODO : put this next line as options
  std::string curPath = fn->getPath().string();
  bool bOk = false;
  if ( curPath.compare(0, 17, "/Issues&Positions")          == 0 ) bOk = true;
  if ( curPath.compare(0, 26, "/News&Media/Press releases") == 0 ) bOk = true;
  if ( !bOk ) return SUCCESS;
  if ( !(
         fn->getPath().extension().string() == ".fhtml" ||
         fn->getPath().extension().string() == ".pdf"   ||
         fn->getPath().extension().string() == ".rpg"   ||
         fn->getPath().extension().string() == ".zip"
       ) )
    return SUCCESS;
  //  LOG(DEBUG) << "WsSearchRSS :: file modification date is " << fn->getPublishDate() << " for " << fn->getPath();
  /* Vect empty, add the element */
  if (m_vect.size() == 0) {
    LOG(DEBUG) << "WsSearchRSS :: vect size was 0 adding " << fn->getPath();
    m_vect.push_back(n);
    return SUCCESS;
  }
  /* File is a latest news, add is at the right position */
  else {
    vector<NodePtr>::iterator it;
    for (it = m_vect.begin(); it != m_vect.end(); ++it) {
      if (fn->getPublishDate() > dynamic_cast<WsFileNode*>((*it).get())->getPublishDate()) {
        m_vect.insert(it, n);
        if (m_vect.size() > m_maxSize)
          m_vect.pop_back();
        return SUCCESS;
      }
    }
    if (m_vect.size() < m_maxSize)
      m_vect.push_back(n);
    return SUCCESS;
  }
}

