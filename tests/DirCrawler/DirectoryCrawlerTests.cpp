
#include "DirectoryCrawlerTests.h"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

DirectoryCrawlerTests::DirectoryCrawlerTests(string path): WsDirectoryCrawler(path)
{
  this->fileCount = 0;
  this->directoryCount = 0;
}
void DirectoryCrawlerTests::runTests()
{
  start();
}

int DirectoryCrawlerTests::applyFile(const path& p)
{
  //	cout << "Counting file " << p.string() << endl;
  this->fileCount++;
}

int DirectoryCrawlerTests::applyDirectory(const path& p)
{
  //	cout << "Counting directory " << p.string() << endl;
  this->directoryCount++;
}

void DirectoryCrawlerTests::beginChild(const path& p)
{
}
void DirectoryCrawlerTests::endChild(const path& p)
{
}
