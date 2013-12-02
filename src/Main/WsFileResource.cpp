/*
 * Copyright (C) 2006-Today G. Deleeuw
 *
 * See the LICENSE file for terms of use.
 */
#include <stdio.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/filesystem/convenience.hpp>

#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WApplication>
#include <Wt/WLogger>

#include "WsFileResource.h"

WsFileResource::WsFileResource(const std::string& strFile, WObject* parent)
  : WFileResource(parent), m_bDelete(false), m_strFile(strFile)
{
  std::string m_strExtension = boost::filesystem::extension(m_strFile);
  //  wApp->log("notice") << "fileResource::fileResource : extension = " << m_strExtension;
  if ( m_strExtension == ".pdf" )
    setMimeType("application/pdf");
  if ( m_strExtension == ".rss" )
    setMimeType("application/rss+xml");
  if ( m_strExtension == ".ods" )
    setMimeType("application/x-vnd.oasis.opendocument.spreadsheet");
  if ( m_strExtension == ".xls" )
    setMimeType("application/vnd.ms-excel");
  if ( m_strExtension == ".png" )
    setMimeType("image/png");
  if ( m_strExtension == ".zip" )
    setMimeType("application/x-gzip");
  if ( m_strExtension == ".bz2" )
    setMimeType("application/x-bz2");
  suggestFileName("data" + m_strExtension);
}

void WsFileResource::deleteAfterProcessed(bool bDelete)
{
  m_bDelete = bDelete;
}

void WsFileResource::handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response)
{
  // is not a continuation for more data of a previous request
  if ( !request.continuation() ) {
    if ( !boost::filesystem::exists(m_strFile) ) return;
    setFileName(m_strFile);
  }
  WFileResource::handleRequest(request, response);
  // this was the last data for the request
  if ( !response.continuation() )
    if ( m_bDelete )
      unlink(fileName().c_str());
}

