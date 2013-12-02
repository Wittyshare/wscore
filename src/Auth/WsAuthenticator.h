/*
 *
 *       Filename:  WsAuthenticator.h
 *
 *    Description: Handles connection of user
 *
 *        Created:  02/15/12 12:37:55
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_AUTHENTICATOR_H__
#define WS_AUTHENTICATOR_H__

#include <iostream>
#include <set>

#include "WsAbstractAuth.h"

/**
 * @brief Loads Authentification module and acts as an interface
 *
 * The module name must be 'libWsModAuth.so'
 **/
class WsAuthenticator {

public:

  /**
   * @brief Constructor
   **/
  WsAuthenticator();

  /**
   * @brief Destructor
   **/
  ~WsAuthenticator();

  /**
   * @brief authentify the user.
   * @param uid the user id
   * @param pass the password
   * @return SUCCESS if user is authentified, FAILURE otherwise
   * If the user is authentifed, his personnal info are retrieved
   **/
  int authentify(const std::string& uid, const std::string& pass = "", const std::string& ip = "");

  /**
   * @brief return the user uid
   * WsAuthenticator::authentify must be called before
   **/
  std::string getUid();

  /**
   * @brief return the user name
   * WsAuthenticator::authentify must be called before
   **/
  std::string getFirstName();

  /**
   * @brief return the user surname
   * WsAuthenticator::authentify must be called before
   **/
  std::string getSurname();

  /**
   * @brief return the user email
   * WsAuthenticator::authentify must be called before
   **/
  std::string getEmail();

  /**
   * @brief return the user groups
   * WsAuthenticator::authentify must be called before
   **/
  std::set<std::string> getUserGroups();

  /**
   * @brief return all the possible groups
   * WsAuthenticator::authentify must be called before
   */
  std::set<std::string> getAllGroups();

private:
  /**
   * @brief loads the module and casts it as WsAbstractAuth object
   * @return SUCCESS or FAILURE
   */
  int loadModule();

  WsAbstractAuth* m_auth;
  std::string m_uid;
  std::string m_pass;
  std::string m_ip;



};

typedef WsAbstractAuth* (*pf_wsAuthMod) ();
#endif
