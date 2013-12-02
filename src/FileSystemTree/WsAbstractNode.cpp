/*
 *
 *       Filename:  WsAbstractNode.cpp
 *
 *    Description:
 *
 *        Created:  03/01/12 15:06:36
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsAbstractNode.h"

WsAbstractNode::~WsAbstractNode()
{
  // No need of this with shared_ptr
  /*
  for (int i = 0; i < m_combinedVect.size(); ++i) {
    if (m_combinedVect[i] != 0) {
      LOG(DEBUG) << "WsAbstractNode ::  Deleting child node " << m_combinedVect[i]->getName();
      delete m_combinedVect[i];
      m_combinedVect[i] = 0;
    }
  }
  */
}
