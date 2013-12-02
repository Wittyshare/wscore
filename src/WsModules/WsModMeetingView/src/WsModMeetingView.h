/*
 *
 *       Filename:  WsModMeetingView.h
 *
 *    Description:  WsModMeetingView : a module to render Asterisk running conference call
 *
 *        Created:  16/01/2013 11:07:15
 *
 *         Author: Guy Deleeuw : guy.deleeuw@gmail.com
 *
 */
#ifndef __WsModMeetingView_H__
#define __WsModMeetingView_H__

#include <Wt/WContainerWidget>


#include <WsModule/WsModule.h>

class WsMeetingView : public gdMeetingView, public WsOptions {
public :
  WsMeetingView(Wt::WContainerWidget* parent = 0);
  ~WsMeetingView();
  virtual void    load();
};





class WsModMeetingView : public WsModule {

public :
  WsModMeetingView();
  ~WsModMeetingView();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool             saveEditor() const;
  std::string      description() const;

};

extern "C" {
  void WsModMeetingViewInit(void) __attribute__((constructor));

  WsModMeetingView* buildModule()
  {
    return new WsModMeetingView();
  }
}

#endif

