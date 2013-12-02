/*
 * =====================================================================================
 *
 *       Filename:  WsCrashLogger.cpp
 *
 *    Description:  Logs WS crashes
 *
 *        Version:  1.0
 *        Created:  27/11/13 09:57:10
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author: Benoit Daccache, ben.daccache@gmail.com
 *
 * =====================================================================================
 */

#include "WsCrashLogger.h"

#include <Include/WsGlobalConfig.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <cxxabi.h>

void WsCrashLogger::printStackTrace(FILE* out, unsigned int maxFrames)
{
  fprintf(out, "Stack trace:\n");
  // storage array for stack trace address data
  void* addrlist[maxFrames + 1];
  // retrieve current stack addresses
  unsigned int addrlen = backtrace( addrlist, sizeof( addrlist ) / sizeof( void* ));
  if ( addrlen == 0 ) {
    fprintf( out, "  \n" );
    return;
  }
  // resolve addresses into strings containing "filename(function+address)",
  // Actually it will be ## program address function + offset
  // this array must be free()-ed
  char** symbollist = backtrace_symbols( addrlist, addrlen );
  size_t funcnamesize = 1024;
  char funcname[1024];
  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for ( unsigned int i = 4; i < addrlen; i++ ) {
    char* begin_name   = NULL;
    char* begin_offset = NULL;
    char* end_offset   = NULL;
    // find parentheses and +address offset surrounding the mangled name
    for ( char* p = symbollist[i]; *p; ++p ) {
      if ( *p == '(' )
        begin_name = p;
      else if ( *p == '+' )
        begin_offset = p;
      else if ( *p == ')' && ( begin_offset || begin_name ))
        end_offset = p;
    }
    if ( begin_name && end_offset && ( begin_name < end_offset )) {
      *begin_name++   = '\0';
      *end_offset++   = '\0';
      if ( begin_offset )
        *begin_offset++ = '\0';
      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():
      int status = 0;
      char* ret = abi::__cxa_demangle( begin_name, funcname,
                                       &funcnamesize, &status );
      char* fname = begin_name;
      if ( status == 0 )
        fname = ret;
      if ( begin_offset ) {
        fprintf( out, "  %-30s ( %-40s  + %-6s) %s\n",
                 symbollist[i], fname, begin_offset, end_offset );
      } else {
        fprintf( out, "  %-30s ( %-40s    %-6s) %s\n",
                 symbollist[i], fname, "", end_offset );
      }
    } else {
      // couldn't parse the line? print the whole line.
      fprintf(out, "  %-40s\n", symbollist[i]);
    }
  }
  free(symbollist);
}
void WsCrashLogger::handleSignal(int signum)
{
  const char* name = NULL;
  switch ( signum ) {
  case SIGABRT:
    name = "SIGABRT";
    break;
  case SIGSEGV:
    name = "SIGSEGV";
    break;
  case SIGILL:
    name = "SIGILL";
    break;
  case SIGFPE:
    name = "SIGFPE";
    break;
  }
  if ( name )
    fprintf( stderr, "Caught signal %d (%s)\n", signum, name );
  else
    fprintf( stderr, "Caught signal %d\n", signum );
  // Dump a stack trace.
  FILE* f = fopen(GlobalConfig::CrashLogPath.c_str(), "w");
  printStackTrace(f, 62);
  fclose(f);
  exit( signum );
}

