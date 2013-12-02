/*
 *
 *       Filename:  WsDecompressor.h
 *
 *    Description:
 *
 *        Created:  07/25/2012 09:29:01 AM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_DECOMPRESSOR_H__
#define WS_DECOMPRESSOR_H__

#include <iostream>

/**
 * @brief Used to decompress data
 *
 * Used between WsFsDaemon and WsFsDaemonClient classes
 */

class WsDecompressor {
public:

  /**
   * @brief decompress the input data and store result in uncompressedData
   * @return the size of the result string
   * @param data the input (compressed) data
   * @param uncompressedData the result (uncompressed) data
   **/
  long decompress(const char* data, size_t size, std::string& uncompressedData);

};

#endif

