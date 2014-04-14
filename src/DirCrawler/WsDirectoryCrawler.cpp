/**
 *
 *       @file  WsdirectoryCrawler.cpp
 *
 *      @brief Fetch a directory and all subdirs
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#include "WsDirectoryCrawler.h"
#include <Include/WsGlobalConfig.h>

using namespace boost::filesystem;
using namespace std;

WsDirectoryCrawler::WsDirectoryCrawler(const string& p): m_isLoaded(false),
  m_p(p)
{
}

WsDirectoryCrawler::~WsDirectoryCrawler()
{
}

int WsDirectoryCrawler::start()
{
  path  thePath(m_p);
  m_isLoaded = false;
  /* Avoid browsing current and hidden directories (avoid endless loops) */
  if (thePath.filename().string().find_first_of(".") == 0 && thePath.filename().string() != "." ) {
    LOG(ERROR) << "DirCrawler::start() :  Root cannot be hidden (" << thePath << ")" << endl;
    return ErrorCode::Failure;
  }
  /* Browse current path */
  int r = this->browse(thePath);
  if (r ==  ErrorCode::Success)
    m_isLoaded = true;
  return r;
}

int WsDirectoryCrawler::browse(const path& p)
{
  try {
    /* If path exists */
    if (exists(p)) {
      if (is_regular_file(p)) {
        /* it's a regular file, we call the virtual function applyFile */
        return applyFile(p);
      } else if (is_directory(p)) {
        /* it's a directory, we call the virtual functuin applyDirectory */
        int r = applyDirectory(p);
        if (r == ErrorCode::Failure)
          return ErrorCode::Failure;
        directory_iterator endItr;
        /* We iterate on all the entries of the directory */
        for (directory_iterator dItr(p); dItr != endItr; ++dItr) {
          /* We call this function to notify that we are accessing a child */
          if (dItr->path().filename().string().find_first_of(".") == 0)
            continue;
          beginChild(dItr->path());
          /* recursive call on the new level */
          int r = browse(dItr->path());
          /* Notify that crawling over the childs is done */
          endChild(dItr->path());
          if (r == ErrorCode::Failure)
            return ErrorCode::Failure;
        }
      } else {
        LOG(ERROR) << "DirCrawler::browse() : This is not a regular File nor a directory : " << p.string() << endl;
      }
    } else {
      LOG(ERROR) << "DirCrawler::browse() : The file does not exist : " << p << endl;
      if (p == m_p)
        return ErrorCode::Failure;
    }
  } catch (const filesystem_error& ex) {
    LOG(ERROR) << "DirCrawler :: " << ex.what() << endl;
  }
  return ErrorCode::Success;
}

