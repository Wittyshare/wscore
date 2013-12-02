/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsTemplate_H_
#define _WsTemplate_H_

#include <Wt/WTemplate>

#include <gdwtcore/gdToolbar.h>

#include <User/WsUser.h>

#include "Main/WsApplication.h"

class WsTemplate : public Wt::WTemplate {
public :
  WsTemplate(const std::string& templateFile, const std::string& initialPath = std::string(), Wt::WContainerWidget* parent = 0);
  ~WsTemplate();
  std::string                       matchTemplate(const std::string& templateFile);
  void      	                     resolveString (const std::string& varName, const std::vector< Wt::WString >& args, std::ostream& result);
  Wt::WWidget*                      resolveWidget(const std::string& varName);

private :
  std::string                       m_sInitialPath;
};

#endif // ifdef _WsTemplate_H_
