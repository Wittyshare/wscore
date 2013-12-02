/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsMenu_H__
#define WsMenu_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WPopupMenu>
#include <Wt/WPushButton>
#include <Wt/WLink>

#include <WsModule/WsModule.h>

/*!
   \file WsMenu.h
   \brief A class that render a menu.

 */

class WsMenu : public Wt::WContainerWidget, public WsOptions {
public :
  WsMenu(Wt::WContainerWidget* parent = 0);
  ~WsMenu();
  WsMenu(const std::string& path, Wt::WContainerWidget* parent = 0);
  void                          load();
  void                          doLoadCurPath();
  void                          doSelectedMenu(std::string newPath);
  void                          loadMenu(NodePtr pNodeParent, Wt::WMenu* menuParent);
  void                          createMenu(NodePtr pNode, Wt::WMenu* menuParent);
  void                          loadPopupMenu(NodePtr pNodeParent, Wt::WPopupMenu* menuParent);
  Wt::WLink                     makeLink(const std::string& path, bool bUseIcon = false);
  void                          loadImage(NodePtr pNodeParent);

public slots :
  void                          doPathChanged(std::string newPath);
  void                          onMouseWentOver(Wt::WPushButton* pButton);

private :
  std::string                   m_sCurPath;
  bool                          m_bDebug;
  std::vector<Wt::WPushButton*> m_vPushButton;
};

#endif // ifndef WsMenu_H__

