/**
 *
 *       @file  WsRequestType.h
 *
 *      @brief  RequestType enum variables
 *
 *    @version  1.0
 *       @date  01.07.2011 14:22:26
 *
 *     Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */
#ifndef WS_REQUEST_TYPE_H__
#define WS_REQUEST_TYPE_H__

#include <iostream>

namespace RequestField {
const std::string Type        = "type";
const std::string Uid         = "uid";
const std::string Ip          = "ip";
const std::string Pass        = "pass";
const std::string Name        = "name";
const std::string Surname     = "surname";
const std::string Email       = "email";
const std::string ExclNames   = "excl_names";
const std::string ExclExt     = "excl_ext";
const std::string Path        = "path";
const std::string Section     = "section";
const std::string Property    = "property";
const std::string NewPath     = "new_path";
const std::string Success     = "success";
const std::string Failure     = "failure";
const std::string Terms       = "terms";
const std::string Body        = "body";
const std::string Size        = "size";
const std::string ModifyDate  = "modify_date";
const std::string Key         = "key";
const std::string Value       = "value";
const std::string NodeType    = "node_type";
};



enum RequestType {
  Auth,
  Perm,
  AllGroups,
  Properties,
  Property,
  Search,
  ClearCache,
  MenuItems,
  MenuItemsEx,
  AccessItems,
  IsEditor,
  IsAdmin,
  RootPath,
  TreeVersion,
  SaveProps,
  SaveProp,
  CreateNode,
  DeleteNode,
  RenameNode
};

#endif
