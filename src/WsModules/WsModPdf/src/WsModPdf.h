
#ifndef WS_MOD_PDF_H__
#define WS_MOD_PDF_H__

#include <Wt/WContainerWidget>

#include <WsModule/src/WsModule.h>

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <Wt/WContainerWidget>


class WsPdf : public Wt::WContainerWidget, public WsOptions {
public :
  WsPdf(Wt::WContainerWidget* parent = 0);
  ~WsPdf();
  void       setDiffPath(const std::string& diffPath);
  virtual void       load();

private :
  std::string        m_sDiffPath;
};


class WsModPdf : public WsModule {
public :
  WsModPdf();
  ~WsModPdf();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool             saveEditor() const;
  std::string      description() const;

private :
};

extern "C" {
  void WsModPdfInit(void) __attribute__((constructor));

  WsModPdf* buildModule()
  {
    return new WsModPdf();
  }
}

#endif

