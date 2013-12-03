/**
 *
 *       @file  WsBasicAuth.h
 *
 *      @brief  Basic connector class
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

#include <Auth/WsAbstractAuth.h>

/**
 * @brief Basic authentication module
 */
class WsBasicAuth: public WsAbstractAuth {
public:

  /**
   * @brief Constructor
   */
  WsBasicAuth();

  /**
   * @brief authenticate the user 
   * this function is a basic function and will authenticate any user and add the group "Guest to his groups"
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
  /**
   * @brief return true or false whether the ip is valid (ie match the mask in the configuration file
   * @param ip the ip
   */
  bool ipValid(std::string ip);

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

};

extern "C" {
  void WsBasicAuthInit(void) __attribute__((constructor));
  WsBasicAuth* buildModule()
  {
    return new WsBasicAuth();
  }
}




#endif
