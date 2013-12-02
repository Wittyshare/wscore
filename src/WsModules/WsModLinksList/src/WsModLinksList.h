/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModLinksList_H__
#define WsModLinksList_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModLinksList.h
   \brief a wittyShare module

 */

class WsLinksList : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsLinksList(Wt::WContainerWidget* parent = 0);
  ~WsLinksList();
  virtual void        load();
};







class WsModLinksList : public WsModule {
public :
  /*! \brief CTor. */
  WsModLinksList();
  ~WsModLinksList();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool                saveEditor() const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModLinksListInit(void) __attribute__((constructor));

  WsModLinksList* buildModule()
  {
    return new WsModLinksList();
  }
}

#endif // ifndef WsModLinksList_H__

