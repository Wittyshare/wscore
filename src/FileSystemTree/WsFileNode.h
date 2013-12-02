/*
 *
 *       @file  WsfileNode.h
 *
 *      @brief  A file Node
 *
 *    @version  1.0
 *       @date  01.07.2011 16:37:11
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_FILE_NODE_H__
#define WS_FILE_NODE_H__
#include "WsNode.h"
#include "WsDirNode.h"
#include <ConfigManager/WsNodeProperties.h>

/**
 * @brief Reprensents a File on disk
 */

class WsFileNode: public WsNode {
public:

  /**
   * @brief Constructor for FileNode
   * @param fullPath the fullPath of the node
   * @param rootPath the rootPath (ie the directory of the site)
   */
  WsFileNode(const path& fullPath, const path& rootPath);

  WsFileNode();


  /**
   * @brief Return true or false whether the gids are allowed or no
   *
   * @param gids the gids
   *
   * @return true or false
   */
  bool isAllowed(const std::set<string>& gids);

  /**
   * @brief returns the list of the groups allowed to access this node
   * @return a set containing these groups
   */
  const set<string> getGroups();

  /**
   * @copydoc WsNode::getDisplayInMenu()
   */
  bool getDisplayInMenu();

  /**
   * @brief returns the publish date of the file
   * @return a string containing this date. The publish date is reprensetend is iso format (seconds from epoch)
   */
  const string getPublishDate();

  /**
   * @brief returns the author name.
   * @return a string containing this name
   */
  const string getAuthor();

  /**
   * @brief return the size of the file
   **/
  const uintmax_t getSize();

  /**
   * @returns false because it's a File
   **/
  bool isDirectory();

  /**
   * @returns true because it's a File
   **/
  bool isRegularFile();


};

#endif
