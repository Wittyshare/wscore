#ifndef DIRECTORY_CRAWLER_TESTSH__
#define DIRECTORY_CRAWLER_TESTSH__

#include <DirCrawler/WsDirectoryCrawler.h>

class DirectoryCrawlerTests: public WsDirectoryCrawler {
public:
  DirectoryCrawlerTests(string path);
  void runTests();
  int fileCount;
  int directoryCount;
protected:
  int applyFile(const boost::filesystem::path& p) ;
  int applyDirectory(const boost::filesystem::path& p);
  void beginChild(const boost::filesystem::path& p) ;
  void endChild(const boost::filesystem::path& p) ;

};
#endif
