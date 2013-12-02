/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModFollowUs_H__
#define WsModFollowUs_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModFollowUs.h
   \brief a wittyShare module

 */

class WsFollowUs : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsFollowUs(Wt::WContainerWidget* parent = 0);
  ~WsFollowUs();
  void                load();

};

class WsModFollowUs : public WsModule {
public :
  /*! \brief CTor. */
  WsModFollowUs();
  ~WsModFollowUs();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModFollowUsInit(void) __attribute__((constructor));

  WsModFollowUs* buildModule()
  {
    return new WsModFollowUs();
  }
}

#endif // ifndef WsModFollowUs_H__

