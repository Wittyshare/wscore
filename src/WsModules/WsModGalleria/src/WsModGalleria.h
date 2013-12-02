/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModGalleria_H__
#define WsModGalleria_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModGalleria.h
   \brief a wittyShare module

 */

class WsGalleria : public Wt::WContainerWidget, public WsOptions {
public :
  WsGalleria(Wt::WContainerWidget* parent = 0);
  ~WsGalleria();
  virtual void       load();
};





class WsModGalleria : public WsModule {
public :
  /*! \brief CTor. */
  WsModGalleria();
  ~WsModGalleria();
  Wt::WWidget*       createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*       createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*    createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*       createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string        description() const;

private :
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModGalleriaInit(void) __attribute__((constructor));

  WsModGalleria* buildModule()
  {
    return new WsModGalleria();
  }
}

#endif // ifndef WsModGalleria_H__

