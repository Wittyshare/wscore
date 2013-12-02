
#ifndef WsModVideo__
#define WsModVideo__

#include <dlfcn.h>

#include <Wt/WContainerWidget>

#include <Logger/WsLogger.h>

#include <WsModule/src/WsModule.h>


class WsModVideo : public WsModule {
public :
  WsModVideo();
  ~WsModVideo();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string      description() const;

private :
};

extern "C" {
  void WsModVideoInit(void) __attribute__((constructor));

  WsModVideo* buildModule()
  {
    return new WsModVideo();
  }
}

#endif // WsModVideo__

