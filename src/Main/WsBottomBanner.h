/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsBottomBanner_H_
#define _WsBottomBanner_H_

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <ConfigManager/WsLayoutProperties.h>

class WsBottomBanner : public Wt::WContainerWidget {
public :
  WsBottomBanner(const std::string& rText, Wt::WContainerWidget* parent = 0);
  ~WsBottomBanner();

  const std::string&     text() const;
  void                   setText(const std::string& rText);

private :
  Wt::WText*             m_pWText;
  std::string            m_pString;
  bool                   m_bDebug;
};

#endif // ifdef _WsBottomBanner_H_

