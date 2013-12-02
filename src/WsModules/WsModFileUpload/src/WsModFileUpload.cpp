/*
* Copyright (C) 2006-Today Guy Deleeuw
*
* See the LICENSE file for terms of use.
*/

#include <dlfcn.h>
#include <stdio.h>

#include <iostream>

#include <boost/filesystem.hpp>

#include <Wt/WLogger>
#include <Wt/WText>

#include <gdcore/gdCore.h>

#include <Logger/WsLogger.h>

#include <Main/WsApplication.h>

#include "WsModFileUpload.h"

extern "C" {
  void WsModFileUploadInit(void)
  {
    fprintf(stderr, "preparing required libraries : libwt.so! %s\n", dlerror());
    void* hndl = dlopen("libwt.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libwt.so shared library! %s\n", dlerror());
      return;
    }
  }
}

// Class WsFileUpload
// ==================
WsFileUpload::WsFileUpload(WContainerWidget* parent)
  : WContainerWidget(parent), m_pFU(0)
{
  addStyleClass("WsFileUpload");
}

void WsFileUpload::load()
{
  WContainerWidget::load();
  addWidget(new WText("Upload a file with a maximum size of 8Mb"));
  m_pFU = new WFileUpload();
  m_pFU->setProgressBar(new WProgressBar());
  //  m_pFU->changed().connect(this, &WFileUpload::upload);
  m_pFU->changed().connect(this, &WsFileUpload::doUpload);
  m_pFU->uploaded().connect(this, &WsFileUpload::doUploaded);
  m_pFU->fileTooLarge().connect(this, &WsFileUpload::doFileTooLarge);
  addWidget(m_pFU);
  WApplication::instance()->log("notice") << "WsFileUpload::load : end !";
}

WsFileUpload::~WsFileUpload()
{
  LOG(DEBUG) << "WsFileUpload::~WsFileUpload() !";
}

void WsFileUpload::doUpload()
{
  wApp->log("notice") << " WsFileUpload::doUpload() : start upload";
  m_pFU->upload();
}

void WsFileUpload::doUploaded()
{
  // TODO : Verif des paths, quid si multiples file ?, optimiser les strings, progressbar marche pas, , ajout d'un champ  : nom desire a l'arrivee ?, test si file existe : replace ?
  wApp->log("notice") << " WsFileUpload::doUploaded() " << m_pFU->spoolFileName();
  std::string sCurUrl          = wApp->internalPath();
  std::string sWithoutPrefix   = WsApp->WsModules().pathWithoutPrefix(sCurUrl);
  WsUser*     pUser            = WsApp->wsUser();
  std::string m_sDocumentRoot  = pUser->getRootPath(); // /var/www/demo_site
  std::string sCurPath         = m_sDocumentRoot + sWithoutPrefix;
  wApp->log("notice") << " WsFileUpload::doUploaded() current path = " << sCurPath;
  std::string sNewFile;
  if ( gdcore_isPathFile(sCurPath) )
    sNewFile = boost::filesystem::path(sCurPath).parent_path().string() + "/" + m_pFU->clientFileName().toUTF8();
  else
    sNewFile = sCurPath + "/" + m_pFU->clientFileName().toUTF8();
  wApp->log("notice") << " WsFileUpload::doUploaded() current dir = " << sNewFile;
  try {
    boost::filesystem::copy_file(m_pFU->spoolFileName(), sNewFile, copy_option::overwrite_if_exists);
    //  The remove is make by the Wt::WFileUpload class
    //    boost::filesystem::remove(m_pFU->spoolFileName());
    boost::algorithm::replace_first(sNewFile, m_sDocumentRoot, "/Edit");
    wApp->log("notice") << " WsFileUpload::doUploaded() set internalPath to : " << sNewFile;
    sleep(1);
    wApp->setInternalPath(sNewFile, true);
  } catch  (boost::filesystem::filesystem_error& e) {
    wApp->log("ERROR") << " WsFileUpload::doUploaded() cannot move " << m_pFU->spoolFileName() << " to " << sNewFile << " error = " << e.what();
  }
}

void WsFileUpload::doFileTooLarge(int64_t nSize)
{
  wApp->log("notice") << " WsFileUpload::doFileTooLarge() : size " << nSize;
}


// Class WsModFileUpload
// =====================
WsModFileUpload::WsModFileUpload()
  : WsModule()
{
  WApplication::instance()->log("notice") << "end ctor of WsModFileUpload !";
}

WsModFileUpload::~WsModFileUpload()
{
  //  WApplication::instance()->log("notice") << "end dtor of WsModFileUpload !";
}

WWidget* WsModFileUpload::createContentsMenuBar(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModFileUpload::createContents(WContainerWidget* parent) const
{
  WsFileUpload* pFU = new WsFileUpload(parent);
  wApp->log("notice") <<  "WsModFileUpload::createContents() : prepare options ";
  pFU->setOptions(options());
  pFU->outOptions("WsModFileUpload::createContents()");
  return pFU;
}

WsEditorWidget* WsModFileUpload::createEditor(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModFileUpload::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

std::string WsModFileUpload::description() const
{
  return "WsModFileUpload wittyShare module";
}

