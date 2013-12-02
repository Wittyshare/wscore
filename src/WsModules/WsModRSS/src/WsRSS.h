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
#ifndef __WsRSS__H_
#define __WsRSS__H_

#include <Wt/WContainerWidget>

#include <Main/WsApplication.h>
#include <WsModule/WsModule.h>


class WsRSS : public Wt::WContainerWidget, public WsOptions {
public :
  WsRSS(Wt::WContainerWidget* parent = 0);
  ~WsRSS();
  void             load();

public slots:
  void             buildRSS();
};

#endif

