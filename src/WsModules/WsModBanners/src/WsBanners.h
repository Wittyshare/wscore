/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsBanners_H__
#define WsBanners_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

/*!
   \file WsBanners.h
   \brief A class that render a menu.

 */

class WsBanners : public Wt::WContainerWidget, public WsOptions {
public :
  WsBanners(Wt::WContainerWidget* parent = 0);
  void                    load();

public slots :
  void                    doPathChanged(std::string newPath);
};

#endif // ifndef WsBanners_H__

