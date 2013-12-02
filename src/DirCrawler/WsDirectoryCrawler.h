/**
 *
 *       @file  WsDirectoryCrawler.h
 *
 *      @brief Fetch a directory and all subdirs
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_DIRECTORY_CRAWLER_H__
#define WS_DIRECTORY_CRAWLER_H__

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem.hpp>

#include <iostream>

#include <Logger/WsLogger.h>

/**
 * @brief Abstract class used to crawl a directory.
 *
 *The start method must be called to start crawling.
 * Every time a folder is found, WsDirectoryCrawler::applyFolder is called.
 * Every time a file is found, WsDirectoryCrawler::applyFile is called
 * The pure virtual methods must be redefined in subclass
 */

class WsDirectoryCrawler {
public:
  /**
   * @brief Class constructor, this class is Abstract and must therefore be inherited
   */
  WsDirectoryCrawler(const std::string& path);

  /**
   * @brief Virtual destructor
   **/
  virtual ~WsDirectoryCrawler() = 0;

  /**
   * @brief Start crawling over all the files and directories/subdirectories
   */
  int start();

private:
  /**
   * @brief applies the required action if file or folder, and browse its children
   */
  int browse(const boost::filesystem::path& p);


protected:
  /**
   * the boost::filesystem::path of the directory to crawl
   */
  std::string m_p;

  /**
   * @brief this method is called by browse when the boost::filesystem::path is a file
   */
  virtual int applyFile(const boost::filesystem::path& p) = 0;

  /**
   * @brief this method is called by browse when the boost::filesystem::path is a folder
   */
  virtual int applyDirectory(const boost::filesystem::path& p) = 0;

  /**
   * @brief this method is called before browsing a subdirectory
   */
  virtual void beginChild(const boost::filesystem::path& p) = 0;

  /**
   * @brief this method is called when finished browsing the subdir
   */
  virtual void endChild(const boost::filesystem::path& p) = 0;

  /**
   * Boolean set to true if the WsDirectoryCrawler::start method has been called and completed successfully
   */
  bool m_isLoaded;
};

#endif
