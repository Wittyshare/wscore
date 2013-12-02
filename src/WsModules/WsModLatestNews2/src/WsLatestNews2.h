/*
 *
 *       Filename:  WsModLatestNews2.h
 *
 *    Description:  WsLatestNews with a Wt WTable Wittyshare plugin
 *
 *        Created:  05/01/2013 13:37:00
 *
 *         Author: Guy Deleeuw : guy.deleeuw@gmail.com
 *
 */
#ifndef __WsLatestNews2__H_
#define __WsLatestNews2__H_

#include <Wt/WContainerWidget>
#include <Wt/WTableView>

#include <Main/WsApplication.h>
#include <WsModule/WsModule.h>


class WsLatestNews2 : public Wt::WContainerWidget, public WsOptions {
public :
  WsLatestNews2(Wt::WContainerWidget* parent = 0);
  ~WsLatestNews2();
  void             load();
  void             setTable();

public slots:
  void onViewSelectionChanged();

private :
  Wt::WTableView*   m_pTableView;
};

#endif

