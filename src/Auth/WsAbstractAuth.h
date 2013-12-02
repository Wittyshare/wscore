/*
 *
 *       Filename:  WsAbstractAuth.h
 *
 *    Description:  Abstract class used to authentificate in the application
 *
 *        Created:  02/10/12 15:33:40
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ABSTRACT_AUTH_H__
#define WS_ABSTRACT_AUTH_H__

#include <iostream>
#include <set>

/**
 * @brief Abstract class for authentification.
 *
 * Must be derived and virtual methods redefinied in the authentication module
 * This class is used to load the Authentication Module
 **/

class WsAbstractAuth {

public:

  /**
   * @brief authentify user with uid.
   * @return SUCCESS if authentication succeeded, FAILURE  otherwise
   * @param uid the user id used to retrieve user's infos
   * @param pass the password of the user
   * @param the ip of the user
   **/
  virtual int authentify(const std::string& uid, const std::string& pass = "", const std::string& ip = "") = 0;

  /**
   * @brief return the user's id
   **/
  virtual std::string getUid() = 0;

  /**
   * @brief return the user's name
   **/
  virtual std::string getFirstName();

  /**
   * @brief return the user's surname
   **/
  virtual std::string getSurname();

  /**
   * @brief return the user's email
   **/
  virtual std::string getEmail();

  /**
   * @brief return the groups of the user
   **/
  virtual std::set<std::string> getUserGroups();

  /**
   * @brief return all the possible groups
   */
  virtual std::set<std::string> getAllGroups();

};

#endif
