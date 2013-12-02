/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModBreadCrumb_H__
#define WsModBreadCrumb_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModBreadCrumb.h
   \brief a wittyShare module

 */


class WsModBreadCrumb : public WsModule {
public :
  /*! \brief CTor. */
  WsModBreadCrumb();
  ~WsModBreadCrumb();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModBreadCrumbInit(void) __attribute__((constructor));

  WsModBreadCrumb* buildModule()
  {
    return new WsModBreadCrumb();
  }
}

#endif // ifndef WsModBreadCrumb_H__

