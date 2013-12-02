/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef  _WsLogo_H_
#define  _WsLogo_H_

#include <Wt/WContainerWidget>
#include <Wt/WImage>

class WsLogo : public Wt::WImage {
public :
  WsLogo(Wt::WContainerWidget* parent = 0);
  ~WsLogo();

public slots :
  void           doLogoClicked();

};

#endif // ifdef _WsLogo_H_

