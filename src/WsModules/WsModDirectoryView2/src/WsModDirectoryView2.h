/*
 *
 *       Filename:  WsModDirectoryView2.h
 *
 *    Description:  WsLatestNews with a Wt WTable Wittyshare plugin
 *
 *        Created:  05/01/2013 13:37:00
 *
 *         Author: Guy Deleeuw : guy.deleeuw@gmail.com
 *
 */
#ifndef __WsModDirectoryView2__H_
#define __WsModDirectoryView2__H_

#include <Wt/WContainerWidget>
#include <Wt/WTableView>

#include <Main/WsApplication.h>

class WsDirectoryItem : public Wt::WStandardItem {
public :
  WsDirectoryItem();
  WsDirectoryItem(const Wt::WString& text);
  void            setNode(NodePtr pNode);
  NodePtr node();

private :
  NodePtr m_node;
};

class WsDirectoryView2 : public Wt::WTableView, public WsOptions {
public :
  WsDirectoryView2(Wt::WContainerWidget* parent = 0);
  ~WsDirectoryView2();
  virtual void      load();

public slots:
  void              onViewSelectionChanged();
};




class WsModDirectoryView2 : public WsModule {
public :
  WsModDirectoryView2();
  ~WsModDirectoryView2();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string      description() const;
};

extern "C" {
  void WsModDirectoryView2Init(void) __attribute__((constructor));

  WsModDirectoryView2* buildModule()
  {
    return new WsModDirectoryView2();
  }
}

#endif // __WsModDirectoryView2__H_

