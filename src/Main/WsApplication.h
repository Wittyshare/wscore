/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsApplication_H_
#define _WsApplication_H_

#include <Wt/WTemplate>

#include <User/WsUser.h>

#include <gdwtcore/gdApplication.h>
#include <gdwtcore/gdToolbar.h>

#include "Main/WsModulesLoader.h"
#include "Main/WsContentButtonsBar.h"
#include "Main/WsContent.h"

#include <Include/WsGlobalConfig.h>

class WsApplication : public gdApplication {
public :
  WsApplication(const Wt::WEnvironment& env);
  ~WsApplication();
  void                         hideImages(bool bHide = true);

  WsUser*                      wsUser();
  WsModulesLoader&             WsModules();
  void                         setTemplate(const std::string& sPage);
  void                         setContent(WsContent* pContent);
  void                         setContentButtonsBar(WsContentButtonsBar* pButtonsBar);
  WsContentButtonsBar*         contentButtonsBar();
  const std::string&           homePage();

  /*! \brief Facility function to access the application object.  */
  static WsApplication*  wsInstance() {
    return (WsApplication*) instance();
  };

public slots :
  void                         doPathChanged(std::string newPath);
  void                         doEndDialogLogon(std::string sUid, std::string pPassword);
  void                         googleAnalyticsLogger(std::string newPath);

private :
  WsUser*                      m_pUser;

  WsContentButtonsBar*         m_pButtonsBar;
  WsContent*                   m_pContent;
  Wt::WWidget*                 m_pImages;
  WsModulesLoader              m_ml;
  std::string                  m_sHomePage;
  std::string                  m_sTemplate;

};

/*! \brief Define a shortcut to the application instance */
#define WsApp     WsApplication::wsInstance()

#endif // ifdef _WsApplication_H_

