/**
 *
 *       Filename:  WsResultItem.h
 *
 *    Description:  One item corresponding to the search criteria
 *
 *        Version:  1.0
 *        Created:  07/11/2011 01:52:01 PM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_RESULT_ITEM__
#define WS_RESULT_ITEM__

#include <FileSystemTree/WsFileNode.h>

#include <iostream>

/**
 * @brief Represents a Result of a search
 *
 * It's a WsNode with a description added
 * @see WsNode
 */
class WsResultItem : public WsFileNode {
public:
  WsResultItem(const path& fullPath, const path& rootPath , std::string body, std::string type, int size);

  ~WsResultItem();

  /**
   * @brief return a short description of the node contents
   */
  string getBody();

  /**
   * @brief return the type of the node
   */
  string getType();

private:
  std::string m_type;
  std::string m_body;
};

#endif
