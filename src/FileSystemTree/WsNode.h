/**
 *
 *       @file  WsNode.h
 *
 *      @brief  Node structure, must be inherited
 *
 *    @version  1.0
 *       @date  01.07.2011 15:28:22
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_NODE_H__
#define WS_NODE_H__

#include <boost/lexical_cast.hpp>
#include <algorithm>

#include <ConfigManager/WsNodeProperties.h>
#include <Include/WsGlobalConfig.h>
#include "WsAbstractNode.h"


/**
 * @brief Represents a node on disk
 */
class WsNode: public WsAbstractNode {

public:

  /**
   * @brief Compares two nodes and returns true or false depending on :
   * If Sort order is asc in the node configuration file, return true if the lhs sort number > rhs sort number, false otherwise
   * If there are no sort number return true if lhs.getName < rhs.getName.
   * If the sort order is desc, the return values are inversed
   * @return true or false
   */
  struct compareNodes {
    bool operator ()(NodePtr lhs, NodePtr rhs) {
      int lsortNumber, rsortNumber;
      /* Get the sort order (asc or desc) */
      std::string sortOrder = lhs.get()->getParent()->getProperties().get()->get("global", "sort_order", "asc");
      try {
        /* Get the sort number of lhs and rhs from the configuration file of the node */
        lsortNumber = boost::lexical_cast<int>(lhs->getProperty("global", "sort_number", "-1"));
        rsortNumber = boost::lexical_cast<int>(rhs->getProperty("global", "sort_number", "-1"));
        if (lsortNumber  == rsortNumber) {
          if (sortOrder == "asc")
            return lhs.get()->getName() < rhs.get()->getName() ;
          return lhs.get()->getName() > rhs.get()->getName() ;
        }
        if (sortOrder == "asc")
          return  lsortNumber > rsortNumber;
        return  lsortNumber < rsortNumber;
      } catch (boost::bad_lexical_cast&) {
        LOG(ERROR) << "WsNode :: Cannot cast sort number, check config file values";
      }
    }
  };

  /**
   * @brief Constructor for the node
   * @param fullPath the fullPath of the node
   * @param rootPath the rootPath (ie the directory of the site)
   */
  WsNode(const path& fullPath, const path& rootPath);

  /**
   * Empty Constructor
   */
  WsNode();

  /**
   * Defaut Destructor
   */
  virtual ~WsNode();

  /**
   * @copydoc WsAbstractNode::getPath()
   */
  const path& getPath();

  /**
   * @copydoc WsAbstractNode::getFullPath()
   */
  const path& getFullPath();

  /**
   * @copydoc WsAbstractNode::getName()
   */
  string getName(const bool noExt = false);

  /**
   * @copydoc WsAbstractNode::getDisplayName()
   */
  const string getDisplayName(const bool noExt = false );

  /**
   * @copydoc WsAbstractNode::addChildDirectory()
   */
  int addChildDirectory(NodePtr f);

  /**
   * @copydoc WsAbstractNode::addChildFile()
   */
  int addChildFile(NodePtr f);

  /**
   * @copydoc WsAbstractNode::addChildNode()
   */
  int addChildNode(NodePtr f);

  /**
   * @copydoc WsAbstractNode::setParent()
   */
  void setParent(NodePtr n);

  /**
   * @copydoc WsAbstractNode::getParent()
   */
  NodePtr getParent();

  /**
   * @copydoc WsAbstractNode::getFiles()
   */
  const vector<NodePtr> getFiles();

  /**
   * @copydoc WsAbstractNode::getDirectories()
   */
  const vector<NodePtr> getDirectories();

  /**
   * @copydoc WsAbstractNode::getAll()
   */
  const vector<NodePtr> getAll();

  /**
   * @copydoc WsAbstractNode::getNodeByName()
   */
  NodePtr getNodeByName(const std::string& name);

  /**
   * @copydoc WsAbstractNode::getDisplayInMenu()
   */
  bool getDisplayInMenu();

  /**
   * @copydoc WsAbstractNode::setProperties()
   */
  void setProperties(NodePropertiesPtr properties);

  /**
   * @copydoc WsAbstractNode::getProperties()
   */
  NodePropertiesPtr getProperties();

  /**
   * @copydoc WsAbstractNode::getProperty()
   */
  std::string getProperty(const std::string& section, const std::string& id, const std::string& def, bool recurse = false);


  /**
   * @copydoc WsAbstractNode::sort()
   */
  void sort();

  /**
   * @copydoc WsAbstractNode::isDirectory()
   **/
  virtual bool isDirectory() = 0;

  /**
   * @copydoc WsAbstractNode::isRegularFile()
   **/
  virtual bool isRegularFile() = 0;

  /**
   * @copydoc WsAbstractNode::getModifyDate()
   **/
  const time_t& getModifyDate();

  /**
   * @brief WsAbstractNode::getCreateDate()
   **/
  const time_t& getCreateDate();

  /**
   * @copydoc WsAbstractNode::setCreateDate()
   **/
  void setCreateDate(const time_t& t);

  /**
   * @copydoc WsAbstractNode::setModifyDate()
   **/
  void setModifyDate(const time_t& t);

  /**
   * @copydoc WsAbstractNode::setSize()
   **/
  void setSize(const uintmax_t& size);

  /**
   * @copydoc WsAbstractNode::getSize()
   **/
  virtual const uintmax_t getSize() = 0;

  /**
   * @copydoc WsAbstractNode::eatPath()
   **/
  NodePtr eatPath(const std::string& path);

protected:
  /**
   * The properties of the node
   */
  NodePropertiesPtr m_properties;
  /**
   * The creation date of the node
   */
  time_t m_createTime;
  /**
   * The modification date of the node
   */
  time_t m_modifyTime;
  
  boost::mutex *m_mutexSort;

};

#endif
