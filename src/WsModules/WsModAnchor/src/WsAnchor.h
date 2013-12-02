/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsAnchor_H__
#define WsAnchor_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsAnchor.h
   \brief a wittyShare widget

 */

class WsAnchor : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsAnchor(Wt::WContainerWidget* parent = 0);
  ~WsAnchor();
  virtual void        load();
};

#endif // ifndef WsAnchor_H__

