/*
 *
 *       Filename:  WsDecompressor.cpp
 *
 *    Description:
 *
 *        Created:  07/25/2012 09:29:01 AM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#include "WsDecompressor.h"

#include <Include/WsGlobalConfig.h>
#include <Logger/WsLogger.h>

#include <stdio.h>
#include <zlib.h>


using namespace std;

long WsDecompressor::decompress(const char* data, size_t size,  string& uncompressedData)
{
  uLongf unCompSize = 200 * size;
  char* to = new char[unCompSize];
  if (uncompress((Bytef*)to, &unCompSize, (const Bytef*)data, size) != Z_OK) {
    LOG(ERROR) << "WsDecompressor :: Decompression failed";
    return FAILURE;
  }
  uncompressedData = string(to, unCompSize);
  delete[] to;
  return unCompSize;
}

