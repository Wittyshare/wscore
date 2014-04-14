/*
 *
 *       Filename:  WsFsTreeConsultation.h
 *
 *    Description:
 *
 *        Created:  03/21/2013 04:04:50 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#ifndef WS_FSTREE_CONSULTATION_H__
#define WS_FSTREE_CONSULTATION_H__

#include <Tree/WsAccessTree.h>
#include <Search/WsResultItem.h>
#include <Updater/WsFsTreeUpdater.h>

#include <boost/thread/mutex.hpp>

/**
 * @brief Encapsulates consultation of tree (no modification)
 */
class WsFsTreeConsultation {

public:

  /**
   * @brief Constructor
   * @param updater the WsFsTreeUpdater instance
   */
  WsFsTreeConsultation(WsFsTreeUpdater* updater);

  /**
   * @copydoc WsFsTreeOperations::getPermissions()
   */
  int getPermissions( const std::set<std::string>& groups, const std::string& p) ;

  /**
   * @copydoc WsFsTreeOperations::getProperties()
   */
  WsNodeProperties* getProperties ( const std::set<std::string>& groups, const std::string& p) ;

  /**
   * @copydoc WsFsTreeOperations::getProperty()
   */
  std::string getProperty( const std::set<std::string>& groups, const std::string& section, const std::string& p, const std::string& prop) ;

  /**
   * @copydoc WsFsTreeOperations::getLock()
   */
  int getLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @copydoc WsFsTreeOperations::putLock()
   */
  int putLock(const std::set<std::string> groups, const std::string& uid, const std::string& path);

  /**
   * @copydoc WsFsTreeOperations::isLocked()
   */
  int isLocked(const std::set<std::string> groups, const std::string& uid, const std::string& path, std::string& id);

  /**
   * @copydoc WsFsTreeOperations::getAccessTree()
   */
  WsAccessTree* getAccessTree( const std::set<std::string>& groups) ;

  /**
   * @copydoc WsFsTreeOperations::getSearchResults()
   */
  std::vector<WsResultItem> getSearchResults( const std::set<std::string>& groups, const std::string& terms) ;
  const std::string getRootPath() ;

  /**
   * @copydoc WsFsTreeOperations::getTemplatesList()
   */
  std::vector<std::string> getTemplatesList( const std::set<std::string>& groups, const std::string& path) ;

  /**
   * @brief Get the stamp of the last WsFileSystemTree
   *
   * @return  a string containing the stamp
   */
  const std::string& getFsTreeStamp();

private:
  WsFsTreeUpdater* m_updater;
  WsGlobalProperties* m_conf;
  boost::mutex m_lockEditMutex;

  long getTimeMs();


};



#endif
