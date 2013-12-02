/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#ifndef WsModFileUpload_H__
#define WsModFileUpload_H__    1

#include <Wt/WContainerWidget>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>

#include <WsModule/WsModule.h>

using namespace Wt;

/*!
   \file WsModFileUpload.h
   \brief a wittyShare module

 */

class WsFileUpload : public Wt::WContainerWidget, public WsOptions {
public :
  /*! \brief CTor. */
  WsFileUpload(Wt::WContainerWidget* parent = 0);
  ~WsFileUpload();
  void                load();

public slots :
  void                doUpload();
  void                doFileTooLarge(int64_t nSize);
  void                doUploaded();

private :
  Wt::WFileUpload*    m_pFU;
};

class WsModFileUpload : public WsModule {
public :
  /*! \brief CTor. */
  WsModFileUpload();
  ~WsModFileUpload();
  Wt::WWidget*        createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*     createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*        createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string         description() const;
};

extern "C" {
  // http://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
  void WsModFileUploadInit(void) __attribute__((constructor));

  WsModFileUpload* buildModule()
  {
    return new WsModFileUpload();
  }
}

#endif // ifndef WsModFileUpload_H__

