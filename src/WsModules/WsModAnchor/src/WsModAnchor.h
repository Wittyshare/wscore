/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModAnchor_H__
#define WsModAnchor_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModAnchor.h
   \brief a wittyShare module

 */

class WsModAnchor : public WsModule {
public :
  /*! \brief CTor. */
  WsModAnchor();
  ~WsModAnchor();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModAnchorInit(void) __attribute__((constructor));

  WsModAnchor* buildModule()
  {
    return new WsModAnchor();
  }
}

#endif // ifndef WsModAnchor_H__

