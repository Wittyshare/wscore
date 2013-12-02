
#ifndef __WsModEditor__H_
#define __WsModEditor__H_

#include <dlfcn.h>

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>


class WsModEditor : public WsModule, public WsOptions {
public :
  WsModEditor();
  ~WsModEditor();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string      description() const;

private :
};

extern "C" {
  void WsModEditorInit(void) __attribute__((constructor));

  WsModEditor* buildModule()
  {
    return new WsModEditor();
  }
}

#endif

