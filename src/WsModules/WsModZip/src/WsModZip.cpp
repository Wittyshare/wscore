/*
 *
 *       Filename:  WsModZip.cpp
 *
 *    Description:  Zip Wittyshare plugin
 *
 *        Created:  03/07/12 11:07:15
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include <Wt/WTheme>
#include <Wt/WFileResource>

#include <gdwtcore/gdToolbar.h>

#include "WsModZip.h"
#include <Main/WsFileResource.h>
#include <Logger/WsLogger.h>

extern "C" {
  void WsModZipInit(void)
  {
    fprintf(stderr, "preparing required libraries : libarchive.so!\n");
    void* hndl = dlopen("libarchive.so", RTLD_NOW | RTLD_GLOBAL);
    if ( hndl == NULL ) {
      fprintf(stderr, "cannot load libarchive.so shared library! %s\n", dlerror());
      return;
    }
  }
}
WsZipEditorWidget::WsZipEditorWidget(WContainerWidget* parent)
  : WsEditorWidget(parent)
{ }

WsZipEditorWidget::~WsZipEditorWidget()
{ }

void WsZipEditorWidget::load()
{
  WsEditorWidget::load();
  WsUser* user = WsApplication::wsInstance()->wsUser();
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsZipEditorWidget::load() " << path;
  std::string prop(user->getProperty(path, WSMODZIP, ALLOW_ZIP, "true"));
  m_cb = new WCheckBox("Allow in zip ? ");
  if ( prop == "true" ) {
    m_bState = true;
    m_cb->setCheckState(Wt::Checked);
  } else
    m_bState = false;
  m_cb->setCheckState(Wt::Unchecked);
  addWidget(m_cb);
}

bool WsZipEditorWidget::saveEditor() const
{
  WsUser* user = WsApplication::wsInstance()->wsUser();
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsModZip ::saveEditor " << path;
  if ( m_cb->checkState() != m_bState )
    if ( user->saveProperty(path, WSMODZIP, ALLOW_ZIP, m_cb->checkState() ? "true" : "false") == FAILURE ) return false;
  return true;
}

WsModZip::WsModZip()
  : WsModule()
{
  LOG(DEBUG) << "WsModZip :: end ctor of WsModZip !";
  m_errmsg = "Could not create archive";
}

WsModZip::~WsModZip()
{
  LOG(DEBUG) << "WsModZip :: end dtor of WsModZip !";
}

WWidget* WsModZip::createContentsMenuBar(WContainerWidget* parent) const
{
  LOG(DEBUG) << "WsModZip::createContentsMenuBar ";
  WsModZip* self =  const_cast<WsModZip*>(this);
  self->buildMenuBar();
  return 0;
}

void WsModZip::destroyContentsMenuBar()
{
  WsModule::destroyContentsMenuBar();
}

WWidget* WsModZip::createContents(WContainerWidget* parent) const
{
  return 0;
}

WsEditorWidget* WsModZip::createEditor(WContainerWidget* parent) const
{
  WsZipEditorWidget* editor = new WsZipEditorWidget();
  return editor;
}

WWidget* WsModZip::createAdmin(WContainerWidget* parent) const
{
  return 0;
}

WWidget* WsModZip::buildMenuBar()
{
  if ( !contentButtonsBar() ) return 0;
  gdToolbarItem*    pTbItem = contentButtonsBar()->addToolbarItem(wApp->theme()->resourcesUrl() + "wittyshare/Icons/compress.png", "", "Compress this directory");
  pTbItem->clicked().connect(SLOT(this, WsModZip::handleZipClick));
  return 0;
}

void WsModZip::handleZipClick()
{
  LOG(DEBUG) << "WsModZip :: Creating widget";
  m_dialog = new WDialog();
  m_dialog->setTitleBarEnabled(false);
  m_dialog->rejectWhenEscapePressed();
  m_archiveType = new WButtonGroup(m_dialog->contents());
  m_fileType = new WButtonGroup(m_dialog->contents());
  m_container = new WGroupBox("Archive Type", m_dialog->contents());
  m_zip = new WRadioButton("Zip", m_container);
  m_tar = new WRadioButton("Tar", m_container);
  m_archiveType->addButton(m_zip);
  m_archiveType->addButton(m_tar);
  m_archiveType->setCheckedButton(m_zip);
  m_container2 = new WGroupBox("What ?", m_dialog->contents());
  m_all = new WRadioButton("All contents", m_container2);
  m_fonly = new WRadioButton("Files only", m_container2);
  m_fileType->addButton(m_all);
  m_fileType->addButton(m_fonly);
  m_fileType->setCheckedButton(m_fonly);
  new Wt::WBreak(m_dialog->contents());
  m_ok = new WPushButton("Ok ", m_dialog->contents());
  m_cancel = new WPushButton("Cancel ", m_dialog->contents());
  m_ok->clicked().connect(SLOT(this, WsModZip::handleAccept));
  m_cancel->clicked().connect(SLOT(this, WsModZip::handleReject));
  m_dialog->show();
}

void WsModZip::appendToZip(const string& path)
{
  LOG(DEBUG) << "WsModZip :: Adding path to archive : " << path;
  m_paths.push_back(path);
}

void WsModZip::handleAccept()
{
  m_paths.clear();
  m_ok->hide();
  m_cancel->hide();
  m_container->hide();
  m_container2->hide();
  int t = -1;
  /* Archive type is TAR */
  if (m_archiveType->id(m_tar) == m_archiveType->checkedId())
    t = TAR;
  else
    /* Archive type is ZIP */
    t = ZIP;
  m_label = new WLabel("Creating archive...\n Please Wait", m_dialog->contents());
  m_loading = new WLabel(new WImage(WApplication::instance()->resourcesUrl() + "loading.gif"), m_dialog->contents());
  int r;
  /* Compress files and folder */
  if (m_fileType->id(m_all) == m_fileType->checkedId())
    r = handleAll(t);
  else
    /* Compress only files (1 level)*/
    r = handleFiles(t);
  if (r < 0) {
    m_dialog->hide();
    StandardButton result = WMessageBox::show("Error", m_errmsg,  Ok );
  }
}

void WsModZip::handleReject()
{
  m_dialog->hide();
  delete m_dialog;
}

int WsModZip::handleFiles(int t)
{
  LOG(DEBUG) << "WsModZip :: Handling only files";
  /* Get the WsUser class */
  WsUser* user = WsApplication::wsInstance()->wsUser();
  NodePtr accessTree = user->getAccessRoot();
  if (!accessTree.get()) {
    LOG(ERROR) << "WsModZip::handleFiles() AccessRoot is NULL";
    return FAILURE;
  }
  /* Get the internalPath */
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsModZip :: Archiving " << path;
  /* Get the directory contents */
  NodePtr tmp = accessTree.get()->eatPath(path);
  if (!tmp)
    return FAILURE;
  vector<NodePtr> contents =   tmp.get()->getAll();
  LOG(DEBUG) << "WsModZip :: Dir contents size :" << contents.size();
  vector<NodePtr>::iterator it;
  int size = contents.size();
  int i = 0;
  /* Iterate over all files and folders in directory */
  for (it = contents.begin(); it != contents.end(); ++it) {
    /* If it's a file */
    if ((*it).get()->isRegularFile()) {
      LOG(DEBUG) << "WsModZip :: Typeid is file for " << (*it)->getFullPath().string();
      /* Check if we can include it in zip (read config) */
      if ((*it).get()->getProperties().get()->get(WSMODZIP, ALLOW_ZIP, "true") == "true")
        if ((*it).get()->getSize() > 0)
          /* If yes and we file size is not 0 add it to zip */
          appendToZip((*it).get()->getPath().string());
    }
  }
  return writeAndCloseZip(t);
}

int WsModZip::handleAll(int t)
{
  LOG(DEBUG) << "WsModZip :: Handling everything";
  /* Get WsUser class */
  WsUser* user = WsApplication::wsInstance()->wsUser();
  NodePtr accessTree = user->getAccessRoot();
  if (!accessTree.get()) {
    LOG(ERROR) << "WsModZip::handleFiles() AccessRoot is NULL";
    return FAILURE;
  }
  /* Get the internalPath */
  string path = WApplication::instance()->internalPath();
  path = WsApplication::wsInstance()->WsModules().pathWithoutPrefix(path);
  LOG(DEBUG) << "WsModZip :: Archiving " << path;
  /* Get the directory contents */
  NodePtr tmp = accessTree.get()->eatPath(path);
  if (!tmp.get())
    return FAILURE;
  vector<NodePtr> contents =   tmp.get()->getAll();
  LOG(DEBUG) << "WsModZip :: Dir contents size :" << contents.size();
  vector<NodePtr>::iterator it;
  int size = contents.size();
  int i = 0;
  /* Iterate over all files and folder */
  for (it = contents.begin(); it != contents.end(); ++it) {
    /* If it's a file */
    if ((*it)->isRegularFile()) {
      LOG(DEBUG) << "WsModZip :: Typeid is file for " << (*it).get()->getFullPath().string();
      /* Check if we can add it to zip */
      if ((*it).get()->getProperties().get()->get(WSMODZIP, ALLOW_ZIP, "true") == "true")
        /* If we can add it to zip and file size > 0, add it */
        if ((*it).get()->getSize() > 0)
          appendToZip((*it)->getPath().string());
    } else {
      /* It's a directory */
      LOG(DEBUG) << "WsModZip :: Typeid is dir for " << (*it).get()->getFullPath().string();
      /* Check if we can add it to zip */
      if ((*it).get()->getProperties().get()->get(WSMODZIP, ALLOW_ZIP_ALL, "true") == "true" && (*it).get()->getProperties().get()->get(WSMODZIP, ALLOW_ZIP, "true") == "true")
        /* Append the directory */
        appendSubDir((*it).get()->getPath().string());
    }
  }
  return writeAndCloseZip(t);
}

void WsModZip::appendSubDir(const string& path)
{
  LOG(DEBUG) << "WsModZip :: Appending sub dir of " << path;
  /* Get the WsUser class */
  WsUser* user = WsApplication::wsInstance()->wsUser();
  NodePtr accessTree = user->getAccessRoot();
  if (!accessTree.get()) {
    LOG(ERROR) << "WsModZip::handleFiles() AccessRoot is NULL";
    return;
  }
  /* Get the internalPath */
  LOG(DEBUG) << "WsModZip :: Archiving " << path;
  /* Get the directory contents */
  NodePtr tmp = accessTree.get()->eatPath(path);
  if (!tmp.get())
    return;
  vector<NodePtr> contents =   tmp.get()->getAll();
  /* Get contents of directory */
  LOG(DEBUG) << "WsModZip :: Got dir contents for " << path;
  vector<NodePtr>::iterator it;
  /* Iterate over files and folder */
  for (it = contents.begin(); it != contents.end(); ++it) {
    /* If it's a file */
    if (typeid(*(*it)) == typeid(WsFileNode)) {
      appendToZip((*it)->getPath().string());
    } else {
      /* it's a folder, recursively add sub files and folders */
      appendSubDir((*it)->getPath().string());
    }
  }
}

std::string WsModZip::description() const
{
  return "WsModZip wittyShare module";
}


int WsModZip::writeAndCloseZip(int format)
{
  LOG(DEBUG) << "WsModZip :: Generating archive file with " << m_paths.size() << " files";
  char buff[16384];
  struct archive* a;
  struct archive* disk;
  struct archive_entry* entry;
  ssize_t len;
  int fd;
  WsApplication* wsApp = WsApplication::wsInstance();
  WsUser* user = wsApp->wsUser();
  char* currentDir = get_current_dir_name();
  string rootPath = user->getRootPath();
  const char* temp = rootPath.c_str();
  char* newDir = new char[rootPath.size() + 1]();
  LOG(DEBUG) << "WsModZip :: Root path is " << rootPath;
  strncpy(newDir, temp, rootPath.size());
  newDir[rootPath.size()] = '\0';
  /* Do a chdir to zip files correctly */
  LOG(DEBUG) << "WsModZip :: Changing current dir name from " << currentDir << " to " << newDir;
  if (chdir(newDir) != 0) {
    LOG(ERROR) << "WsModZip :: Could not chdir to " << newDir;
    return -1;
  }
  LOG(DEBUG) << "WsModZip ::  Current dir name : " << get_current_dir_name();
  string ext;
  /* Create a new archive */
  a = archive_write_new();
  /* Check format wanted */
  if (format == TAR) {
    LOG(DEBUG) << "WsModZip :: Archive format will be tar.bz2";
    /* Set flags TAR */
    archive_write_set_compression_compress(a);
    archive_write_set_compression_bzip2(a);
    archive_write_set_format_ustar(a);
    ext = ".tar.bz2";
  } else if (format == ZIP) {
    /* Set flags zIP */
    archive_write_set_compression_compress(a);
    LOG(DEBUG) << "WsModZip :: Archive format will be zip";
    archive_write_set_format_zip(a);
    ext = ".zip";
  } else {
    LOG(ERROR) << "WsModZip :: Unknown file format ";
    return -1;
  }
  /* Get TMP directory of user */
  string filenameTmp = wsApp->getUserTmpPath() + string(basename(tmpnam(NULL))) + ext;
  LOG(DEBUG) << "WsModZip :: Archive file name will be " << filenameTmp.c_str();
  /* Open archive */
  archive_write_open_file(a, filenameTmp.c_str());
  disk = archive_read_disk_new();
  archive_read_disk_set_standard_lookup(disk);
  /* Nothing to archive */
  if (m_paths.size() == 0) {
    m_errmsg = "Nothing to archive";
    return -1;
  }
  /* Iterate over paths we want to add to the archive */
  for (int i = 0; i < m_paths.size(); ++i) {
    struct archive* disk = archive_read_disk_new();
    int r;
    LOG(DEBUG) << "WsModZip :: Adding file " << m_paths[i];
    string str = m_paths[i];
    /* remove the starting / */
    if (m_paths[i][0] == '/') {
      str = m_paths[i].substr(1);
      LOG(DEBUG) << "WsModZip :: m_paths starts with /";
    }
    LOG(DEBUG) << "Reading file " << str.c_str();
    r = archive_read_disk_open(disk, str.c_str());
    if (r != ARCHIVE_OK) {
      LOG(ERROR) << "WsModZip :: Cannot read file";
      return -1;
    }
    /* Write */
    for (;;) {
      int needcr = 0;
      entry = archive_entry_new();
      r = archive_read_next_header2(disk, entry);
      if (r == ARCHIVE_EOF)
        break;
      if (r != ARCHIVE_OK) {
        LOG(ERROR) << "WsModZip :: Archive error";
        return -1;
      }
      archive_read_disk_descend(disk);
      LOG(DEBUG) << "WsModZip :: Writing archive header";
      r = archive_write_header(a, entry);
      if (r < ARCHIVE_OK) {
        needcr = 1;
      }
      if (r == ARCHIVE_FATAL) {
        LOG(ERROR) << "WsModZip :: Archive fatal";
        return -1;
      }
      if (r > ARCHIVE_FAILED) {
        fd = open(archive_entry_sourcepath(entry), O_RDONLY);
        len = read(fd, buff, sizeof(buff));
        while (len > 0) {
          LOG(DEBUG) << "WsModZip :: writing data of length " << sizeof(buff);
          archive_write_data(a, buff, len);
          len = read(fd, buff, sizeof(buff));
        }
        close(fd);
      } else
        LOG(ERROR) << "WsModZip :: Archive failed";
      archive_entry_free(entry);
    }
    archive_read_close(disk);
    archive_read_free(disk);
  }
  archive_write_close(a);
  archive_write_free(a);
  if (chdir(currentDir) != 0)
    LOG(DEBUG) << "WsModZip :: Could not chdir to " << currentDir;
  LOG(DEBUG) << "WsModZip :: Archive ready for Download";
  boost::replace_all(filenameTmp, wsApp->docRoot(), "");
  WApplication::instance()->redirect(filenameTmp);
  m_dialog->hide();
  delete m_dialog;
  return 0;
}
