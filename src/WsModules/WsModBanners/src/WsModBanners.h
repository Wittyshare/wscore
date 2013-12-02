/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModBanners_H__
#define WsModBanners_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

class WsModBanners : public WsModule {
public :
  /*! \brief CTor. */
  WsModBanners();
  ~WsModBanners();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModBannersInit(void) __attribute__((constructor));

  WsModBanners* buildModule()
  {
    return new WsModBanners();
  }
}

#endif // ifndef WsModBanners_H__

