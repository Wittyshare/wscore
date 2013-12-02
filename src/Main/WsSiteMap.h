/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef  _WsSiteMap_H_
#define  _WsSiteMap_H_

#include <Wt/WContainerWidget>

class WsSiteMap : public Wt::WContainerWidget {
public :
  WsSiteMap(Wt::WContainerWidget* parent = 0);
  ~WsSiteMap();

public slots :
  void           doSiteMapClicked();

};

#endif // ifdef _WsSiteMap_H_

