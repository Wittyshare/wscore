/**
 *
 *       @file  WsFsTreeClient.h
 *
 *      @brief  Interacts directly with the menuTree and the fsTree
 *
 *    @version  1.0
 *       @date  07/26/11 10:06:29
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_FS_TREE_CLIENT_H__
#define WS_FS_TREE_CLIENT_H__

#include <Updater/WsFsTreeUpdater.h>
#include <Tree/WsMenuTree.h>
#include <FsTreeOp/WsFsTreeOperations.h>
#include <Tree/WsAccessTree.h>
#include <Auth/WsAuthenticator.h>
#include <ConfigManager/WsGlobalProperties.h>
#include <Search/WsSearch.h>
#include "WsAbstractFsClient.h"


/**
 * @brief Interface between WsUser class and local tree
 *
 * Used when not using FsDaemon
 */
class WsFsTreeClient: public WsAbstractFsClient {
public:
  /**
   * @brief constructor
   * @param id the user id
   */
  WsFsTreeClient(const std::string& id, const std::string& pass, const std::string& ip);

  /**
   * @brief desctrutor
   */
  virtual ~WsFsTreeClient();

  /**
   * @copydoc WsAbstractFsClient::load()
   */
  int load();

  /**
   * @copydoc WsAbstractFsClient::getPermissions()
   */
  int getPermissions(const std::string& p);

  /**
   * @copydoc WsAbstractFsClient::getProperties().get()
   */
  const WsNodeProperties* getProperties(const std::string& p);

  /**
   * @copydoc WsAbstractFsClient::getProperty()
   **/
  std::string getProperty(const std::string& section, const std::string& p, const std::string& prop);

  /**
   * @copydoc WsAbstractFsClient::getSurname()
   */
  const std::string& getSurname() const;

  /**
   * @copydoc WsAbstractFsClient::getFirstName()
   */
  const std::string& getFirstName() const;

  /**
   * @copydoc WsAbstractFsClient::getEmail()
   */
  const std::string& getEmail() const;

  /**
   * @copydoc WsAbstractFsClient::getMenuRoot()
   */
  NodePtr getMenuRoot(const bool& forceUpdate = false);

  /**
   * @copydoc WsAbstractFsClient::getLock();
   */
  int getLock(const std::string& path);
  
  /**
   * @copydoc WsAbstractFsClient::putLock();
   */
  int putLock(const std::string& path);
  
  /**
   * @copydoc WsAbstractFsClient::isLocked();
   */
  int isLocked(const std::string& path, std::string& uid);

  /**
   * @copydoc WsAbstractFsClient::getAccessRoot()
   */
  NodePtr getAccessRoot( const bool& forceUpdate = false);


  /**
   * @copydoc WsAbstractFsClient::getMenuRoot()
   */
  NodePtr getMenuRoot(const std::set<string>& exclNames, const std::set<string>& exclExt);

  /**
   * @copydoc WsAbstractFsClient::getSearchResults()
   **/
  vector<WsResultItem> getSearchResults(const std::string& terms);

  /**
   * @copydoc WsAbstractFsClient::getAllGroups()
   */
  std::set<string> getAllGroups();

  /**
   * @copydoc WsAbstractFsClient::getRootPath()
   **/
  const std::string getRootPath();

  /**
   * @copydoc WsAbstractFsClient::saveProperties()
   **/
  int saveProperties(WsNodeProperties* props, const std::string& path);

  /**
   * @copydoc WsAbstractFsClient::createNode()
   */
  int createNode(const string& path, int type);

  /**
   * @copydoc WsAbstractFsClient::deleteNode()
   */
  int deleteNode(const string& path);

  /**
   * @copydoc WsAbstractFsClient::renameNode()
   */
  int renameNode(const string& path, const string& newPath);

  /**
   * @copydoc WsAbstractFsClient::saveProperty()
   **/
  int saveProperty(const std::string& path, const std::string& section, const std::string& attr, const std::string& val);

  /**
   * @copydoc WsAbstractFsClient::isEditor()
   **/
  bool isEditor();

  /**
   * @copydoc WsAbstractFsClient::isAdministrator()
   **/
  bool isAdministrator();

  /**
   * @copydoc WsAbstractFsClient::getTemplateList()
   */
  std::vector<std::string> getTemplatesList(const std::string& path);

private:
  /**
   * @brief authenticate the user and load it's params
   */
  int authentify();

  /**
   * Contains the groups of the user
   */
  std::set<string> m_groups;
  /**
   * uid of the user
   */
  std::string m_uid;
  /**
   * pass of the user
   */
  std::string m_pass;
  /**
   * password of the user
   */
  std::string m_ip;
  std::string m_sn;
  std::string m_givenName;
  std::string m_email;
  NodePtr m_menu;
  NodePtr m_accessTree;
  WsGlobalProperties* m_conf;
  WsFsTreeOperations* m_operation;
};

#endif
