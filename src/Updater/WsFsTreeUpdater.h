/**
 *
 *       @file WsFsTreeUpdater.h
 *
 *      @brief  Updates the tree and takes care of unconsistencies with the user
 *
 *    @version  1.0
 *       @date  07/13/11 15:50:19
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_FS_TREE_UPDATER_H__
#define WS_FS_TREE_UPDATER_H__

#define BOOST_NO_DEPRECATED

#include <FileSystemTree/WsFileSystemTree.h>

#include <list>

#include "WsAbstractMonitor.h"

#include <boost/thread.hpp>
class WsMonitor;


/**
 * @brief handles the update of the FileSystemTree
 *
 * Updates the FileSystemTree by launching Gamin or by regularly updating it
 */
class WsFsTreeUpdater {
public:
  /**
   * @brief constrictor
   * @param p the root path
   * @param m Monitor or not ?
   */
  WsFsTreeUpdater(const std::string& p, unsigned int delay, bool m = false);

  ~WsFsTreeUpdater();

  /**
   * @brief update the fsTree
   * @return SUCCESS or FAILURE
   */
  int update();

  /**
   * @brief returns the last created tree
   */
  FileSystemTreePtr getLastTree();

  /**
   * @brief checks whether the fs is the last version
   * @param fs the FilsSystemTree to check
   * @return true if is the same, false otherwise
   */
  bool isLastVersion(FileSystemTreePtr fs);
private:

  /**
   * @brief Starts the regular update of the tree.
   *
   * This method must be called in a thread and only if Gamin is disabled
   *
   * @return SUCCESS or FAILURE
   */
  int startRegularUpdate();

  /**
   * @brief Updates the tree in a thread.
   *
   * @return SUCCESS or FAILURE
   */
  int threadUpdate();

  /**
   * @brief RootPath
   */
  std::string m_rootPath;

  /**
   * @brief Use Gamin or no
   */
  bool m_isMonitor;

  /**
   * @brief Pointer to the file monitoring class used
   */
  WsAbstractMonitor* m_monitor;

  /**
   * @brief Delay after which the FileSystemTree will be updated if changed occured
   */
  unsigned int m_delay;

  FileSystemTreePtr m_fsTree;

  boost::mutex m_lMutex;
  boost::mutex m_updateMutex;
  bool m_isLoaded;
};

#endif
