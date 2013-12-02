/*
 * Copyright (C) 2006-Today Guy Deleeuw
 *
 * See the LICENSE file for terms of use.
*/

#ifndef _WsBreadCrumb_H_
#define _WsBreadCrumb_H_

#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>

class WsBreadCrumb : public Wt::WContainerWidget, public WsOptions {
private:
  class breadCrump {
  public :
    breadCrump();
    ~breadCrump();
    std::string                   title;
    std::string                   path;
    NodePtr               node;
  };

public :
  WsBreadCrumb(Wt::WContainerWidget* parent = 0);
  ~WsBreadCrumb();
  virtual void                    load();
  void                            setDebugging(bool bDebug);

public slots :
  void                            doPathChanged(std::string newPath);
  void                            onBreadCrumbSelected(Wt::WMenuItem* pMenuItem);

private :
  std::vector<breadCrump>         m_vBreadCrumb;
  Wt::WMenu*                      m_pMenu;
  bool                            m_bDebug;
};

#endif // ifdef _WsBreadCrumb_H_

