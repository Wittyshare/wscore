/*
 * =====================================================================================
 *
 *       Filename:  WsUtils.cpp
 *
 *    Description:  Utilities
 *
 *        Version:  1.0
 *        Created:  02/05/14 15:55:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Benoit Daccache (ben.daccache@gmail.com)
 *   Organization:  
 *
 * =====================================================================================
 */

#include "WsUtils.h"
#include <ConfigManager/WsGlobalProperties.h>

using namespace std;
using namespace boost;

bool WsUtils::ipValid(string ip)
{
  string ipMask = WsGlobalProperties::instance()->get("ldap", "ip_mask", "*");
  if (ipMask == "*")
    return true;

  char_separator<char> sep1("|");
  tokenizer<char_separator<char> > tokens(ipMask, sep1);
  tokenizer<char_separator<char> >::iterator iter = tokens.begin();
  for (; iter != tokens.end(); ++iter) {
      if(ipMatch(ip, *iter))
          return true;
  }
  return false;
}

bool WsUtils::ipMatch(string ip, string mask)
{
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> sep(".");
    tokenizer maskTok(mask, sep);
    tokenizer uipTok(ip, sep);
    tokenizer::iterator mask_iter = maskTok.begin();
    tokenizer::iterator ip_iter = uipTok.begin();
    for (; ip_iter != uipTok.end(); ++mask_iter, ++ip_iter) {
        if (mask_iter == maskTok.end())
            return false;
        if (*mask_iter == "*")
            continue;
        if (*ip_iter != *mask_iter)
            return false;
    }
    return true;
}
