/**
 *
 *       @file  WsFileSystemTree.h
 *
 *      @brief  Structure representing all the fs tree from the root
 *
 *    @version  1.0
 *       @date  01.07.2011 15:27:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_FILESYSTEM_TREE_H__
#define WS_FILESYSTEM_TREE_H__
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <iostream>

#include "WsAbstractNode.h"
#include "WsNode.h"
#include "WsDirNode.h"
#include "WsFileNode.h"
#include <DirCrawler/WsDirectoryCrawler.h>

#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

class WsFileSystemTree;

typedef boost::shared_ptr<WsFileSystemTree> FileSystemTreePtr;


/**
 * @brief Represents the file structure on disk
 */

class WsFileSystemTree: public WsDirectoryCrawler {
public:
  /**
   * @brief Constructor for the FileSystemTree object
   * @param p the root path of the directory (rootPath)
   * @param m monitor or not the filesystemtree using Gamin ?
   */
  WsFileSystemTree(const std::string& p, bool m = false);

  ~WsFileSystemTree();

  /**
   * @brief crawls the diretory and sorts the children. It is called by WsDirectoryCrawler::start()
     @return success status
  */
  int start();

  /**
   * @brief returns the root of the WsFileSystemTree
   * @return NodePtr corresponding to the Root of the FileSystemTree
   */
  NodePtr getRoot();

  /**
   * @copydoc WsAbstractTree::eatPath()
   * @deprecated replace with WsAbractNode::eatPath()
   */
  NodePtr eatPath(const std::string& p);

  /**
   * @brief returns the number of users that are using this fileSystemTree version
   */
  int getUseCount();

  /**
   * @brief increments the number of uses of the tree by 1
   */

  void incrementUseCount();

  /**
   * @brief decrements the numbers of uses by one
   */
  void decrementUseCount();

  /**
   * @brief returns the boost::filesystem::path to the base directory where documents are stored
   * @return SUCCESS or FAILURE
   */
  const boost::filesystem::path& getRootPath() const;

  /**
   * @brief returns the std::vector containing the boost::filesystem::path that should be monitored by the WsUpdater Class
   **/
  std::vector<boost::filesystem::path>& getMonitorPaths();

  /**
   * @brief returns the Tree unique stamp
   **/
  const std::string& getStamp();

  /**
   * @brief inserts the node in the tree
   * The node is inserted on the right place on the tree depending on it's path
   * The node is not created in the filesystem.
   * If the node path is incorrect, the node is not inserted and FAILURE is returned. Otherwise SUCCESS is returned
   **/
  int insertNode(NodePtr newNode);

private:
  /**
   * @copydoc WsDirectoryCrawler::applyFile()
   * creates the node for a file
   */
  int applyFile(const boost::filesystem::path& filePath);

  /**
   * @copydoc WsDirectoryCrawler::applyDirectory()
   * creates the node for a directory
   */
  int applyDirectory(const boost::filesystem::path& dirPath);

  /**
   * @copydoc WsDirectoryCrawler::beginChild()
   */
  void beginChild(const boost::filesystem::path& p);

  /**
   * @copydoc WsDirectoryCrawler::endChild()
   */
  void endChild(const boost::filesystem::path& p);

  /**
   * @brief creates unique stamp for the tree
   */
  void createStamp();

  /**
   * The rootPath (the path of the root folder
   */
  boost::filesystem::path m_rootPath;

  boost::mutex m_lMutex;

  /**
   * Monitored paths by gamin
   */
  std::vector<boost::filesystem::path> m_monitorPaths;

  /**
   * WsFileSystemTree stamp
   */
  std::string m_stamp;

  /**
   * Number of process using the tree (currently 1 because no multithreading in fsdaemon
   */
  int m_useCount;

  /*
   * Monitor with Gamin ?
   */
  bool m_monitor;

  /**
   * Root Node
   */
  NodePtr m_root;

  /**
   * Current Node, used when building the tree to insert at the correct position
   */
  NodePtr m_current;
};

#endif
