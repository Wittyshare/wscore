/*
 *
 *       Filename:  WsAbstractFsClient.h
 *
 *    Description:  File System Tree client
 *
 *        Version:  1.0
 *        Created:  07/25/2011 04:11:48 PM
 *
 */


#ifndef WS_ABSTRACT_FSCLIENT_H__
#define WS_ABSTRACT_FSCLIENT_H__
#include <iostream>
#include <vector>

#include <ConfigManager/WsNodeProperties.h>
#include <FileSystemTree/WsAbstractNode.h>
#include <Logger/WsLogger.h>
#include <Search/WsResultItem.h>

/**
 * @brief Abstract Interface between the WsUser class and the operation on the tree
 */
class WsAbstractFsClient {
public:

  virtual ~WsAbstractFsClient();

  /**
   * @brief loads the user info
   */
  virtual int load() = 0;
  /**
   * @copydoc WsUser::load()
   */
  virtual int getPermissions(const std::string& p) = 0;
  /**
   * @copydoc WsUser::getPermissions()
   */
  virtual const WsNodeProperties* getProperties (const std::string& p) = 0;

  /**
   * @copydoc WsUser::getProperties()
   **/
  virtual std::string getProperty(const std::string& section, const std::string& p, const std::string& prop) = 0;

  /**
   * @copydoc WsUser::getSurname()
   */
  virtual const std::string& getSurname() const = 0;

  /**
   * @copydoc WsUser::getFirstName()
   */
  virtual const std::string& getFirstName() const = 0;

  /**
   * @copydoc WsUser::getEmail()
   */
  virtual const std::string& getEmail() const = 0;

  /**
   * @copydoc WsUser::getLock()
   */
  virtual int getLock(const std::string& path) = 0;

  /**
   * @copydoc WsUser::putLock()
   */
  virtual int putLock(const std::string& path) = 0;

  /**
   * @copydoc WsUser::isLocked()
   */
  virtual int isLocked(const std::string& path, std::string& uid) = 0;

  /**
   * @copydoc WsUser::getAccessRoot()
   */
  virtual NodePtr getAccessRoot( const bool& forceUpdate = false) = 0;

  /**
   * @copydoc WsUser::getSearchResults()
   */
  virtual std::vector<WsResultItem> getSearchResults(const std::string& terms) = 0;

  /**
   * @copydoc WsUser::getAllGroups()
   */
  virtual std::set<string> getAllGroups() = 0;

  /**
   * @copydoc WsUser::getRootPath()
   **/
  virtual const std::string getRootPath() = 0;

  /**
   * @copydoc WsUser::saveProperties()
   **/
  virtual int saveProperties(WsNodeProperties* props, const std::string& path) = 0;

  /**
   * @copydoc WsUser::saveProperty()
   **/
  virtual int saveProperty(const std::string& path, const std::string& section, const std::string& attr, const std::string& val) = 0;

  /**
   * @copydoc WsUser::createNode()
   */
  virtual int createNode(const string& path, int type) = 0;

  /**
   * @copydoc WsUser::deleteNode()
   */
  virtual int deleteNode(const string& path) = 0;

  /**
   * @copydoc WsUser::renameNode()
   */
  virtual int renameNode(const string& path, const string& newPath) = 0;

  /**
   * @copydoc WsUser::isAdministrator()
   **/
  virtual bool isAdministrator() = 0;

  /**
   * @copydoc WsUser::isEditor()
   **/
  virtual bool isEditor() = 0;

  /**
   * @copydoc WsUser::getTemplateList()
   */
  virtual std::vector<std::string> getTemplatesList(const std::string& path) = 0;


};

#endif
