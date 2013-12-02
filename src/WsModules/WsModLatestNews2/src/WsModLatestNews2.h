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
#ifndef __WsModLatestNews2__H_
#define __WsModLatestNews2__H_

#include <Wt/WContainerWidget>

#include <Main/WsApplication.h>

class WsModLatestNews2 : public WsModule {

public :
  WsModLatestNews2();
  ~WsModLatestNews2();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool             saveEditor() const;
  std::string      description() const;
};

extern "C" {
  void WsModLatestNews2Init(void) __attribute__((constructor));

  WsModLatestNews2* buildModule()
  {
    return new WsModLatestNews2();
  }
}

#endif

