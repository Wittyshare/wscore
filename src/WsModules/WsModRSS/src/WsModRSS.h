/*
 *
 *       Filename:  WsModRSS.h
 *
 *    Description:  WsLatestNews with a Wt WTable Wittyshare plugin
 *
 *        Created:  05/01/2013 13:37:00
 *
 *         Author: Guy Deleeuw : guy.deleeuw@gmail.com
 *
 */
#ifndef __WsModRSS__H_
#define __WsModRSS__H_

#include <Wt/WContainerWidget>

#include <Main/WsApplication.h>

class WsModRSS : public WsModule {

public :
  WsModRSS();
  ~WsModRSS();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string      description() const;
  void             buildMenuBar(Wt::WContainerWidget* parent = 0);
};

extern "C" {
  void WsModRSSInit(void) __attribute__((constructor));

  WsModRSS* buildModule()
  {
    return new WsModRSS();
  }
}

#endif

