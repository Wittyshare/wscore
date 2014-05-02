/**
 *
 *       @file  WsLdapAuth.h
 *
 *      @brief  Ldap connector class
 *
 *    @version  1.0
 *       @date  07/15/11 09:25:47
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_LDAP_CONNECTOR_H__
#define WS_LDAP_CONNECTOR_H__
#include <set>

#include <dlfcn.h>

#include <ConfigManager/WsGlobalProperties.h>
#include <gdcore/gdLdap.h>

#include <Auth/WsAbstractAuth.h>

/**
 * @brief Ldap authentication module
 */
class WsLdapAuth: public WsAbstractAuth {
public:
  enum loadStatus { notLoaded = 1, loaded, onError };

  /**
   * @brief Constructor
   */
  WsLdapAuth();

  /**
   * @brief authenticate the user by retrieving his info from ldap
   *
   * @param uid the uid of the user
   * @param pass the password of the user
   * @param ip the ip of the user
   *
   * @return SUCCESS or FAILURE
   */
  int authentify(const std::string& uid, const std::string& pass = "", const std::string& ip = "");

  /**
   * @brief Get the uid of the user
   *
   * @return a std::string containing the uid
   */
  std::string getUid();

  /**
   * @brief Get the first name of the user
   *
   * @return a std::string containing the name
   */
  std::string getFirstName();

  /**
   * @brief Get the surname of the user
   *
   * @return  the surname of the user
   */
  std::string getSurname();

  /**
   * @brief Get the email of the user
   */
  std::string getEmail();

  /**
   * @brief get the groups of the user
   */
  std::set<std::string> getUserGroups();

  /**
   * @brief get all the groups in the ldap
   */
  std::set<std::string> getAllGroups();

protected:

private:
  std::string m_uid;
  std::string m_name;
  std::string m_surname;
  std::string m_email;
  std::string m_dn;
  std::string m_ip;
  std::set<std::string> m_groups;
  std::set<std::string> m_allGroups;
  WsGlobalProperties* m_properties;
  loadStatus m_ldapStatus;

};

extern "C" {
  void WsLdapAuthInit(void) __attribute__((constructor));
  WsLdapAuth* buildModule()
  {
    return new WsLdapAuth();
  }
}




#endif
