/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModMenu_H__
#define WsModMenu_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModMenu.h
   \brief a wittyShare module to display a menu in a side of a page

 */

class WsModMenu : public WsModule {
public :
  /*! \brief CTor. */
  WsModMenu();
  ~WsModMenu();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool                saveEditor() const;
  std::string         description() const;

private :
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModMenuInit(void) __attribute__((constructor));

  WsModMenu* buildModule()
  {
    return new WsModMenu();
  }
}

#endif // ifndef WsModMenu_H__

