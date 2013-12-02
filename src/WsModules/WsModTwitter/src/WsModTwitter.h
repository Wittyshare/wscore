/*
 *
 *       Filename:  WsModTwitter.h
 *
 *    Description:  Twitter Wittyshare plugin
 *
 *        Created:  03/05/2013 12:54:15
 *
 *         Author: Guy Deleeuw, Guy.Deleeuw@gmail.com
 *
 */
#ifndef __WsModTwitter__
#define __WsModTwitter__

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WCheckBox>

#include <WsModule/WsModule.h>
#include <Main/WsApplication.h>

class WsTwitterEditorWidget : public WsEditorWidget {
public :
  WsTwitterEditorWidget(Wt::WContainerWidget* parent = 0);
  ~WsTwitterEditorWidget();
  virtual void          load();
  virtual bool          saveEditor();

private :
  Wt::WCheckBox*        m_cb;
  bool                  m_cbState;
};





class WsModTwitter : public WsModule {
public :
  WsModTwitter();
  ~WsModTwitter();
  Wt::WWidget*          createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  virtual void          destroyContentsMenuBar();
  Wt::WWidget*          createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*       createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*          createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string           description() const;

public slots:
  void                  handleTwitterClick();

private :
  Wt::WWidget*          buildMenuBar();
  Wt::WPushButton*      m_buttonReTweet;
};

extern "C" {
  void WsModTwitterInit(void) __attribute__((constructor));

  WsModTwitter* buildModule()
  {
    return new WsModTwitter();
  }
}

#endif //  __WsModTwitter__

