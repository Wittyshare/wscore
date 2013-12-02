/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef  _WsSearchObject_H_
#define  _WsSearchObject_H_

#include <Wt/WContainerWidget>

#include <gdwtcore/gdWSearch.h>

class WsSearchObject : public gdWSearch {
public :
  WsSearchObject(Wt::WContainerWidget* parent = 0);
  ~WsSearchObject();

public slots :
  void           doSearch(Wt::WString sSearch);
  void           doFocussed(Wt::WString sSearch);
  void           doBlurred(Wt::WString sSearch);

private :
  std::string        m_searchPath;
};

#endif // ifdef _WsSearchObject_H_

