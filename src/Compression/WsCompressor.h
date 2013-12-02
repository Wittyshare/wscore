/*
 *
 *       Filename:  WsCompressor.h
 *
 *    Description:
 *
 *        Created:  07/25/2012 09:26:05 AM
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 */

#ifndef WS_COMPRESSOR_H__
#define WS_COMPRESSOR_H__

#include <iostream>

/**
 * @brief Used to compress data.
 *
 * It is used for compressing data between WsFsDaemon and WsFsDaemonClient classes
 */

class WsCompressor {
public:
  /**
   * @brief compress the input string (data) and puts it in compressedData.
   * @return FAILURE if error occured, otherwise the size of the compressed data buffer
   * @param data the uncompressed string
   * @param compressedData will contain the compressed data
   **/
  long compress(const std::string& data, char** compressedData);


};

#endif
