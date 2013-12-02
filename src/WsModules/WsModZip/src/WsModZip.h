/*
 *
 *       Filename:  WsModZip.h
 *
 *    Description:  Zip Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_MOD_ZIP_H__
#define WS_MOD_ZIP_H__

#include <Wt/WContainerWidget>

#include <WsModule/WsModule.h>

#include <FileSystemTree/WsAbstractNode.h>
#include <User/WsUser.h>
#include <Main/WsApplication.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include <Wt/WApplication>
#include <Wt/WLogger>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WRadioButton>
#include <Wt/WButtonGroup>
#include <Wt/WDialog>
#include <Wt/WGroupBox>
#include <Wt/WLabel>
#include <Wt/WImage>
#include <Wt/WMessageBox>
#include <Wt/WCheckBox>

#include <vector>
using namespace std;

using namespace Wt;

#define ZIP 1
#define TAR 2

#define WSMODZIP "wsmodzip"
#define ALLOW_ZIP "allow_zip"
#define ALLOW_ZIP_ALL "allow_zip_all"

class WsZipEditorWidget : public WsEditorWidget {
public :

  /**
   * @brief Constructor
   *
   * @param parent the parent widget
   */
  WsZipEditorWidget(Wt::WContainerWidget* parent = 0);

  /**
   * @brief Destructor
   */
  ~WsZipEditorWidget();

  /**
   * @brief Load the widget
   */
  virtual void       load();

  virtual bool       saveEditor() const;

private :
  Wt::WCheckBox*     m_cb;
  bool               m_bState;
};


class WsModZip : public WsModule {

public :
  WsModZip();
  ~WsModZip();
  Wt::WWidget*     createContentsMenuBar(Wt::WContainerWidget* parent = 0) const;
  virtual void     destroyContentsMenuBar();
  Wt::WWidget*     createContents(Wt::WContainerWidget* parent = 0) const;
  WsEditorWidget*  createEditor(Wt::WContainerWidget* parent = 0) const;
  Wt::WWidget*     createAdmin(Wt::WContainerWidget* parent = 0) const;
  std::string      description() const;

private :
  Wt::WWidget* buildEditor(Wt::WContainerWidget* parent);
  Wt::WWidget* buildMenuBar();

  /**
   * @brief Append the file to the zip
   *
   * @param path the  path to the node
   */
  void appendToZip(const string& path);

  /**
   * @brief A
   *
   * @param path
   */
  void appendSubDir(const string& path);
  int writeAndCloseZip(int format);

  int handleAll(int t);
  int handleFiles(int t);

  vector<string> m_paths;

  Wt::WDialog* m_dialog;
  Wt::WRadioButton* m_all;
  Wt::WRadioButton* m_fonly;
  Wt::WRadioButton* m_tar;
  Wt::WRadioButton* m_zip;
  Wt::WGroupBox* m_container;
  Wt::WGroupBox* m_container2;
  Wt::WButtonGroup* m_archiveType;
  Wt::WButtonGroup* m_fileType;
  Wt::WPushButton* m_ok;
  Wt::WPushButton* m_cancel;
  Wt::WLabel* m_loading;
  Wt::WLabel* m_label;

  string m_errmsg;

public slots:
  void handleZipClick();
  void handleAccept();
  void handleReject();


};

extern "C" {
  void WsModZipInit(void) __attribute__((constructor));

  WsModZip* buildModule()
  {
    return new WsModZip();
  }
}

#endif

