/**
 *
 *       @file WsfileSystemTree.cpp
 *
 *      @brief  Structure representing all the fs tree from the root
 *
 *    @version  1.0
 *       @date  01.07.2011 15:27:15
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#include "WsFileSystemTree.h"
#include <Include/WsGlobalConfig.h>
#include <ConfigManager/WsNodeProperties.h>

#include <sys/time.h>

#include<boost/tokenizer.hpp>
#include <boost/thread/thread.hpp>
using namespace boost;
using namespace std;

WsFileSystemTree::WsFileSystemTree(const string& p, bool m):
  WsDirectoryCrawler(p),
  m_rootPath(p),
  m_monitor(m),
  m_useCount(0)
{
}

WsFileSystemTree::~WsFileSystemTree()
{
  m_root.reset();
  m_current.reset();
  LOG(DEBUG) << "WsFileSystemTree::~WsFileSystemTree() : deleting ";
}

int WsFileSystemTree::start()
{
  /* Create unique stamp */
  createStamp();
  LOG(INFO) << "WsFileSystemTree::start() : Building FsTree for path " << m_p;
  /* Start Crawling */
  int r = WsDirectoryCrawler::start();
  if (r == ErrorCode::Failure)
    LOG(ERROR) << "WsFileSystemTree::start() : Error while building FsTree for path " << m_p;
  else {
    LOG(INFO) << "WsFileSystemTree::start() :  FsTree built with success for path " << m_p;
  }
  return r;
}


int WsFileSystemTree::applyFile(const path& filePath)
{
  /* Create node for current path */
  NodePtr n(new WsFileNode(filePath, m_rootPath));
  /* Create properties for current path */
  NodePropertiesPtr props(new WsNodeProperties(n.get()->getFullPath(), WsNodeProperties::File));
  n.get()->setProperties(props);
  /* Get the last write time of file on disk and set it on the node */
  std::time_t t = boost::filesystem::last_write_time( filePath ) ;
  n.get()->setModifyDate(t);
  n.get()->setSize(boost::filesystem::file_size(filePath));
  //TODO ADD creation date support on ext4 only
  //
  /* if current is NULL, it means we are at the root */
  if (m_current == 0)
    m_current = n;
  else {
    /* It's not the root so we add the Node to the current node's children file */
    m_current.get()->addChildFile(n);
  }
  if (m_root == 0)
    m_root = m_current;
  return ErrorCode::Success;
}

const path& WsFileSystemTree::getRootPath() const
{
  return m_rootPath;
}

int WsFileSystemTree::applyDirectory(const path& dirPath)
{
  /* If monitored by gamin is on, add it to the list of monitored files/folders */
  if (m_monitor) {
    m_monitorPaths.push_back(dirPath);
  }
  WsNodeResources* resources = new WsNodeResources(dirPath);
  /* Create  .config/{nodes, images, templates, ..} if not already here */
  if (resources->createResourcesDirectories() == ErrorCode::Failure )
    return ErrorCode::Failure;
  /* Create a dir node */
  NodePtr n(new WsDirNode(dirPath, m_rootPath));
  std::time_t t = boost::filesystem::last_write_time( dirPath ) ;
  n.get()->setModifyDate(t);
  dynamic_cast<WsDirNode*>(n.get())->setResources(resources);
  /* Parse Config of the node */
  NodePropertiesPtr props(new WsNodeProperties(n.get()->getFullPath(), WsNodeProperties::Dir));
  /* Create properties directories */
  props.get()->createPropertiesDirectories();
  n.get()->setProperties(props);
  /* if current is NULL, it means we are at the root */
  if (m_current == 0)
    m_current = n;
  else {
    /* Add the node as a child of the current node */
    m_current.get()->addChildDirectory(n);
    m_current = n;
  }
  if (m_root == 0)
    m_root = m_current;
  return ErrorCode::Success;
}

void WsFileSystemTree::beginChild(const path& p)
{
  //We need to define it but we don't need to use it
}

void WsFileSystemTree::endChild(const path& p)
{
  /* after we handle a directory (and all its children) when traversing the tree,
     we set current to its parent i.e. we go back up in the tree */
  NodePtr parent = m_current.get()->getParent();
  if (m_current.get() != 0 && parent.get() != 0 && is_directory(p)) {
    m_current = parent;
  }
}

NodePtr WsFileSystemTree::getRoot()
{
  if (WsDirectoryCrawler::m_isLoaded)
    return m_root;
  else {
    LOG(ERROR) << "WsFileSystemTree::getRoot() : FileSystemTree not loaded" << endl;
    return NodePtr();
  }
}

NodePtr WsFileSystemTree::eatPath(const string& p)
{
  //This methods should be deleted and WsAbstractNode::eatPath() should be used instead
  if (!WsDirectoryCrawler::m_isLoaded) {
    LOG(ERROR) << "WsFileSystemTree::eatPath() : FileSystemTree not loaded" << endl;
    return NodePtr();
  }
  typedef tokenizer<char_separator<char> >
  tokenizer;
  char_separator<char> sep("/");
  tokenizer tok(p, sep);
  NodePtr curNode = m_root;
  for (tokenizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
    NodePtr res = curNode.get()->getNodeByName(*beg);
    if (res.get() == 0)
      return res;
    curNode = res;
  }
  return curNode;
}

int WsFileSystemTree::getUseCount()
{
  return m_useCount;
}

int WsFileSystemTree::insertNode(NodePtr newNode)
{
  path p = newNode.get()->getPath();
  NodePtr parent = eatPath(p.parent_path().string());
  if (parent.get() == 0)
    return ErrorCode::Failure;
  /* Parent is here, check if it's a DIR */
  if ( !parent.get()->isDirectory() )
    return ErrorCode::Failure;
  if ( newNode.get()->isRegularFile()) {
    if ( parent.get()->addChildFile(newNode) == ErrorCode::Failure)
      return ErrorCode::Failure;
  } else {
    if ( parent.get()->addChildDirectory(newNode) == ErrorCode::Failure)
      return ErrorCode::Failure;
  }
  if ( parent.get()->addChildNode(newNode) == ErrorCode::Failure)
    return ErrorCode::Failure;
  LOG(DEBUG) << "WsFileSystemTree::insertNode() : Node " << p.string() << " inserted in FileSystemTree";
  /* Create new stamp */
  createStamp();
  return ErrorCode::Success;
}

void WsFileSystemTree::incrementUseCount()
{
  //Not used now because fsdaemon is not multihreaded
  /*
  {
    boost::mutex::scoped_lock lock(m_lMutex);
    LOG(DEBUG) << "WsFileSystemTree::incrementUseCount() : Incrementing use count of FsTree " << m_stamp << " to " << (m_useCount + 1);
    if (m_useCount < 0)
      LOG(ERROR) << "WsFileSystemTree :: FileSystemTree usage is negative!!" << endl;
    else
      m_useCount++;
  }
  */
}

void WsFileSystemTree::decrementUseCount()
{
  //Not used now because fsdaemon is not multihreaded
  /*
  {
    boost::mutex::scoped_lock lock(m_lMutex);
    LOG(DEBUG) << "WsFileSystemTree :: Decrementing use count of FsTree " << m_stamp << " to " << (m_useCount - 1);
    if (m_useCount < 1)
      LOG(ERROR) << "WsFileSystemTree :: FileSystemTree usage is negative!!" << endl;
    else
      m_useCount--;
  }
  */
}


vector<path>& WsFileSystemTree::getMonitorPaths()
{
  return m_monitorPaths;
}

const string& WsFileSystemTree::getStamp()
{
  return m_stamp;
}

void WsFileSystemTree::createStamp()
{
  //Create a random stamp
  timeval time;
  gettimeofday(&time, 0);
  long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
  m_stamp = boost::lexical_cast<string>(millis);
}

