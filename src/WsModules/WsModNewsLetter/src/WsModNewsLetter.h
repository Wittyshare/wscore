/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModNewsLetter_H__
#define WsModNewsLetter_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModNewsLetter.h
   \brief a wittyShare module

 */

class WsModNewsLetter : public WsModule {
public :
  /*! \brief CTor. */
  WsModNewsLetter();
  ~WsModNewsLetter();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool                saveEditor() const;
  std::string         description() const;
  virtual std::string checkPath(const std::string& currentPath);
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModNewsLetterInit(void) __attribute__((constructor));

  WsModNewsLetter* buildModule()
  {
    return new WsModNewsLetter();
  }
}

#endif // ifndef WsModNewsLetter_H__

