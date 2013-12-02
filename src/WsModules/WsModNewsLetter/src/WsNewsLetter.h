/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsNewsLetter_H__
#define WsNewsLetter_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

#include <WsModule/WsModule.h>

#include "EmailToken.h"

using namespace Wt;

/*!
   \file WsNewsLetter.h
   \brief a wittyShare module

 */

class WsNewsLetter : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief descripte the result of the Ldap query */
  enum emailAvailability { ldapError, emailNotAvailable, emailAvailable, emailMultiple };

  /*! \brief CTor. */
  WsNewsLetter(Wt::WContainerWidget* parent = 0);
  ~WsNewsLetter();
  void                   load();
  void                   initDB();
  emailAvailability      isEmailExistInLdap(const std::string& email);
  void                   emailNotInLdap(const std::string& email);
  void                   multipleEmail(const std::string& email);
  void                   sendEmail(const std::string& email, const std::string& sSubject, const std::string& sBody, const std::string& sBodyPlain);
  std::string            checkPath(const std::string& currentPath);
  void                   registerValidEmail(const std::string& email);

public slots :
  void                   doLineEditChanged();
  void                   doSubscribe();

private :
  Wt::WLineEdit*         m_pLineEdit;
  Wt::WPushButton*       m_pButSubscribe;
  dbo::Session           session_;
  dbo::backend::Sqlite3* sqlite3_;
};

#endif // ifndef WsNewsLetter_H__

