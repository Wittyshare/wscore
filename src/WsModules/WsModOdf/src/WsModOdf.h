/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModOdf_H__
#define WsModOdf_H__    1

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModOdf.h
   \brief a wittyShare module

 */

class WsOdf : public Wt::WContainerWidget, public WsOptions {
public :
  WsOdf(Wt::WContainerWidget* parent = 0);
  ~WsOdf();
  void       setDiffPath(const std::string& diffPath);
  virtual void       load();

private :
  std::string        m_sDiffPath;
};





class WsModOdf : public WsModule {
public :
  /*! \brief CTor. */
  WsModOdf();
  ~WsModOdf();
  Wt::WWidget*       createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*       createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*    createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*       createAdmin(Wt::WContainerWidget* parent = 0) const;
  bool               saveEditor() const;
  std::string        description() const;

private :
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModOdfInit(void) __attribute__((constructor));

  WsModOdf* buildModule()
  {
    return new WsModOdf();
  }
}

#endif // ifndef WsModOdf_H__

