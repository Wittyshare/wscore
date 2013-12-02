/*
 *
 *       Filename:  WsUserSession.h
 *
 *    Description:
 *
 *        Created:  03/21/2013 01:47:11 AM
 *       Revision:  none
 *
 *         Author:  Benoit Daccache (BD), ben.daccache@gmail.com
 *        Company:
 *
 */

#ifndef WS_USER_SESSION_H__
#define WS_USER_SESSION__H__

#include <iostream>
#include <set>


/**
 * @brief Reprensents a User session.
 *
 * This class is instanciated when a user first connects to the daemon to authenticate
 */
class WsUserSession {

public:
  /**
   * @brief Constructor
   * @param uid the user id
   * @param pass optionnal password
   * @param ip optionnal ip of the user
   */
  WsUserSession(const std::string& uid, const std::string& pass = "", const std::string& ip = "");

  /**
   * @brief get the groups belonging to the user
   * @return a set of gid or empty set if no groups
   */
  const std::set<std::string>& getGroups();

  /**
   * @return the user's name
   */
  const std::string& getName();

  /**
   * @return the user's surname
   */
  const std::string& getSurname();

  /**
   * @returns the user's email
   */
  const std::string& getEmail();

  /**
   * @brief sets the groups of a user
   * @param groups a set of gids
   */
  void setGroups(const std::set<std::string>& groups);

  /**
   * @brief set the name of the user
   * @param name the name
   */
  void setName(const std::string& name);

  /**
   * @brief set the surname of the user
   * @param surname the surname
   */
  void setSurname(const std::string& surname);

  /**
   * @brief set the email of the user
   * @param email the email
   */
  void setEmail(const std::string& email);

  /**
   * @brief returns true or false whether the session is valid
   * @return true if session valid, false otherwise
   * @param uid user id
   * @param pass the password
   * @param ip the ip
   */
  bool isValidSession(const std::string& uid, const std::string& pass, const std::string& ip);

protected:
  std::string m_uid;
  std::string m_pass;
  std::string m_ip;
  std::string m_name;
  std::string m_surname;
  std::string m_email;
  std::set<std::string> m_groups;
  long m_timestamp;

private:
  /**
   * @brief returns the time.now()
   */
  long getNowTimestamp();









};


#endif
