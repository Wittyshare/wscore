/*
 *
 *       Filename:  WsCompressor.cpp
 *
 *    Description:
 *
 *        Created:  07/25/2012 09:26:05 AM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsCompressor.h"

#include <Include/WsGlobalConfig.h>
#include <Logger/WsLogger.h>

#include <stdio.h>
#include <zlib.h>
using namespace std;

long WsCompressor::compress(const string& data, char** compressedData)
{
  char* from = new char[data.size()];
  strncpy(from, data.c_str(), data.size());
  unsigned long compBufSize = (uLongf)(data.size() + (data.size() * 0.1) + 12);
  *compressedData = new char[compBufSize];
  if (compress2((Bytef*)*compressedData, (uLongf*)&compBufSize, (const Bytef*)from, (uLongf)data.size(), Z_BEST_COMPRESSION) != Z_OK) {
    LOG(ERROR) << "WsCompressor :: Compression failed";
    delete[] from;
    return ErrorCode::Failure;
  }
  delete[] from;
  return compBufSize;
}
