/**
 *
 *       @file  WsAbstractNode.h
 *
 *      @brief  Abstract Node class
 *
 *    @version  1.0
 *       @date  07/15/11 16:51:16
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_ABSTRACT_NODE_H__
#define WS_ABSTRACT_NODE_H__
#define BOOST_FILESYSTEM_NO_DEPRECATED



#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::filesystem;

#include <ConfigManager/WsNodeProperties.h>
#include <Logger/WsLogger.h>

#include <iostream>
#include <list>
#include <vector>

using namespace std;

/**
 * Forward Reference
 */
class WsAbstractNode;

/* Typedef used to swith easily from boost to C++11 */
typedef boost::shared_ptr<WsAbstractNode> NodePtr;
typedef boost::weak_ptr<WsAbstractNode> WNodePtr;

typedef boost::shared_ptr<WsNodeProperties> NodePropertiesPtr;

/**
 * @brief Contains Abstract metods on nodes and variables.
 *
 * Abstract and is inherited by WsNode
 */

class WsAbstractNode : public boost::enable_shared_from_this<WsAbstractNode> {
public:

  /**
   * Type of ressource
   */
  enum ResourceType { Icones, Images, Templates };

  /**
   * @brief Virtual destructor
   **/
  virtual ~WsAbstractNode();

  /**
   * @brief returns the relative path of the node
   * @return the relative path of the node,
   * ie the Full path without the rootPath
   */
  virtual const path& getPath() = 0;

  /**
   * @brief returns the full path of the node
   */
  virtual const path& getFullPath() = 0;

  /**
   * @brief returns the name of the node
   * @returns the name of the node. ie the text contained between the last '/' and the end
   */
  virtual string getName(const bool noExt = false) = 0;

  /**
   * @brief returns the display name of the node
   * @returns the display name of the node if set in the configuration file of the node, otherwise WsAbstractNode::getName() is returned
   */
  virtual const string getDisplayName(const bool noExt = false) = 0;

  /**
   * @brief adds a node as  a child directory
   * The Directory is added to the WsAbstractNode::m_dirVect
   * @param f a NodePtr of the WsAbstractNode representing the directory
   * @returns SUCCESS or FAILURE
   */
  virtual int addChildDirectory(NodePtr f) = 0;

  /**
   * @brief adds a node as a child file
   * The Directory is added to the WsAbstractNode::m_fileVect
   * @param f a NodePtr of the WsAbstractNode representing the file
   */
  virtual int addChildFile(NodePtr f) = 0;

  /**
   * @brief add a Node to the combined vect WsAbstractNode::m_combinedVect
   * @param f a NodePtr of the WsAbstractNode representing the file or directory
   */
  virtual int addChildNode(NodePtr f) = 0;

  /**
   * @brief sets the parent of the node represented by this
   * @param n the parent node that is a NodePtr representing a WsDirNode
   */
  virtual void setParent(NodePtr n) = 0;

  /**
   * @brief returns the parent node of this
   * @returns the parent Node, a NodePtr representing a WsDirNode if existing otherwise returns NULL
   */
  virtual NodePtr getParent() = 0;

  /**
   * @brief returns the list of all the file contained in the directory represented by this
   * @return a list of Nodes containing these children
   */
  virtual const vector<NodePtr> getFiles() = 0;

  /**
   * @brief returns the list of all the file contained in the file reprensented by this
   * @returns a list of Nodes containing these children
   */
  virtual const vector<NodePtr> getDirectories() = 0;

  /**
   * @brief combine the vector of files and directories into a single one
   * @return the combined vector
   */
  virtual const vector<NodePtr> getAll() = 0;

  /**
   * @brief Searches for the NodePtr reprenting the filename
   * @param name the name of the node with the extension
   * @return the corresponding NodePtr or an empty NodePtr if not found
   */
  virtual NodePtr getNodeByName(const std::string& name) = 0;

  /**
   * @brief returns true if the item should be displayed in the menu. This value is set in the configuration file of the node
   **/
  virtual bool getDisplayInMenu() = 0;

  /**
   * @brief set the properties of the node.
   * The properties are a WsAbstractProperties object.
   * @param properties w WsNodeProperties representing the properties of the node. The object should be instanciated and values set in it
   **/
  virtual void setProperties(NodePropertiesPtr properties) = 0 ;

  /**
   * @brief get the properties of the node
   * If the properties are not set (no conf found, or node retrieved from server) empty properties are returned.
   * To get the properties for a node retrieved from server use WsUser::getProperties
   * @see WsUser::getProperties
   **/
  virtual NodePropertiesPtr getProperties() = 0;

  /**
   * @brief get a node property reprensented by section/id
   * If no value found, the value def is returned
   * @param section the section of the key we are searching
   * @param id the key we are searching
   * @param def the default value to return if not found
   * @param recurse if set to true and no value found , the method will look for parent values until a value is found.
   */
  virtual std::string getProperty(const std::string& section, const std::string& id, const std::string& def, bool recurse = false) = 0;

  /**
   * @brief sort the nodes using the sort number or the name if not provided
   **/
  virtual void sort() = 0;

  /**
   * @brief return true if one of the groups contained in gids has access to the node
   * false otherwise
   * If the node is retrieved from the daemon use WsUser::isAllowed instead
   * @param gids a set of gids (Typically the gids of a user
   * @return true if one or more gid that have access are present in gids, false otherwise
   * @see WsUser::isAllowed
   */
  virtual bool isAllowed(const std::set<string>& gids) = 0;

  /**
   * @brief returns true if Node is a WsDirNode, false otherwise
   **/
  virtual bool isDirectory() = 0;

  /**
   *  @brief returns true if Node is a WsFileNode, false otherwise
   **/
  virtual bool isRegularFile() = 0;

  /**
   * @brief returns the modification date of the file reprenseted by the node
   **/
  virtual const time_t& getModifyDate() = 0;

  /**
   * @brief returns the creation date of the file represented by the node
   **/
  virtual const time_t& getCreateDate() = 0;

  /**
   * @brief sets the creation date of the file.
   * This method does not change the actual creation date of the file represented by the node
   **/
  virtual void setCreateDate(const time_t& t) = 0;

  /**
   * @brief sets the modification date of the file.
   * This method does not change the actual modification date of the file represented by the node
   **/
  virtual void setModifyDate(const time_t& t) = 0;

  /**
   * @brief sets the file size in the node. Typically called by  WsFileSystemTree when building the FileSystemTree
   **/
  virtual void setSize(const uintmax_t& size) = 0;

  /**
   * @brief returns the file size. If the Node is a WsDirNode, 0 is returned. if
   * the global property dir_size is set to true, the number of items in the node is returned (one level)
   **/
  virtual const uintmax_t getSize() = 0;

  /**
   * @brief resolves the path and returns the node reprenseted by this path
   * The path should be starting from the rootPath otherwise the method won't be able to resolve it.
   * @param path the path of the node we are searching starting from the rootPath
   * @return The corresponding NodePtr reprensenting the WsAbstractNode if found or an empty NodePtr
   **/
  virtual NodePtr eatPath(const std::string& path) = 0;

  /**
   * The parent Node
   */
  WNodePtr m_parent;
  /**
   * The std::vector containing the children files of the Node represented by this
   */
  vector<NodePtr> m_fileVect;
  /**
   * The std::vector containing the children directories of the Node represented by this
   */
  vector<NodePtr> m_dirVect;
  /**
   * A combined vector of WsAbstractNode::m_fileVect and WsAbstractNode::m_dirVect
   * It is built in the WsAbstractNode::sort() method
   */
  vector<NodePtr> m_combinedVect;
  /**
   * The full path of the Node (ie the rootPath + m_relPath)
   */
  path m_fullPath;
  /**
   * The relative path of the Node (ie the path starting from the rootPath
   */
  path m_relPath;
  /**
   * The size of the file represented by the node on disk
   */
  uintmax_t m_size;

};

#endif
