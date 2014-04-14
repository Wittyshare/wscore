/**
 *
 *       @file  WsFsTreeUpdater.cpp
 *
 *      @brief  Updates the tree and takes care of unconsistencies with the user
 *
 *    @version  1.0
 *       @date  07/13/11 15:50:19
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsFsTreeUpdater.h"
#include "WsMonitor.h"
#include <Include/WsGlobalConfig.h>

WsFsTreeUpdater::WsFsTreeUpdater(const string& p, unsigned int delay, bool m)
  : m_rootPath(p),
    m_isMonitor(m),
    m_delay(delay),
    m_monitor(0),
    m_isLoaded(false)
{
}

WsFsTreeUpdater::~WsFsTreeUpdater()
{
  if (m_monitor != 0)
    delete m_monitor;
  LOG(DEBUG) << "WsFsTreeUpdater::~WsFsTreeUpdater() : Freeing memory completed";
}

FileSystemTreePtr WsFsTreeUpdater::getLastTree()
{
  LOG(DEBUG) << "WsFsTreeUpdater::getLastTree() : Getting LastTree";
  boost::mutex::scoped_lock lock(m_lMutex);
  {
    /* No trees yet, create one */
    if ( m_fsTree.get() == 0)
      update();
  }
  return m_fsTree;
}

int WsFsTreeUpdater::update()
{
  if (m_updateMutex.try_lock()) {
    LOG(DEBUG) << "WsFileSystemTree::update() : Updating FsTree";
    /*  If there is already active trees, build the new one in a thread
     *  to avoid server block
     */
    if (m_fsTree.get()) {
      new boost::thread(boost::bind(&WsFsTreeUpdater::threadUpdate, this));
      m_updateMutex.unlock();
      return ErrorCode::Success;
    }
    /* Create the new Tree */
    WsFileSystemTree* newTree = new WsFileSystemTree(m_rootPath, m_isMonitor);
    if (newTree->start() == ErrorCode::Failure) {
      m_updateMutex.unlock();
      return ErrorCode::Failure;
    }
    /* Start the monitor is activated */
    if (m_isMonitor && m_monitor == 0) {
      LOG(INFO) << "WsFsTreeUpdater::update() : Launching monitor";
      m_monitor = new WsMonitor(this, newTree->getMonitorPaths(), m_delay);
      boost::thread* t = m_monitor->start();
    }
    /* No monitor activated, so we update the tree every x seconds */
    if (!m_isMonitor && !m_isLoaded) {
      boost::thread* updater = new boost::thread(boost::bind(&WsFsTreeUpdater::startRegularUpdate, this));
      m_isLoaded = true;
    }
    /* Put it on the top of the stack */
    m_fsTree = FileSystemTreePtr(newTree);
    m_updateMutex.unlock();
    return ErrorCode::Success;
  } else
    LOG(DEBUG) << "WsFsTreeUpdater::update() : Update already in progress";
  return ErrorCode::Failure;
}

bool WsFsTreeUpdater::isLastVersion(FileSystemTreePtr fs)
{
  return m_fsTree.get() == fs.get();
}

int WsFsTreeUpdater::threadUpdate()
{
  /* Create new tree */
  WsFileSystemTree* newTree = new WsFileSystemTree(m_rootPath, m_isMonitor);
  if (newTree->start() == ErrorCode::Failure)
    return ErrorCode::Failure;
  m_fsTree = FileSystemTreePtr(newTree);
  return ErrorCode::Success;
}

int WsFsTreeUpdater::startRegularUpdate()
{
  if (m_delay == 0) {
    return ErrorCode::Success;
  }
  LOG(INFO) << "WsFsTreeUpdater::startRegularUpdate() : Monitoring is on. Will update the tree every " << m_delay << " s.";
  /* Endless loop that will update the tree every m_delay seconds */
  while (1) {
    string rootpath = WsGlobalProperties::instance()->get("global", "root_path", ".");
    string temp = rootpath +  GlobalConfig::PathToLockFile;
    temp = boost::filesystem::path(temp).string();
    ifstream fconf(temp.c_str());
    if (fconf.good()) {
      continue;
    }
    sleep(m_delay);
    this->update();
  }
}
