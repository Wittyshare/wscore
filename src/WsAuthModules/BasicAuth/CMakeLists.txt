cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModBasicAuth")
PROJECT($APPNAME)

SET ( APPNAME_SRCS 
  src/WsBasicAuth.cpp
  ../../Logger/WsLogger.cpp
  ../../Logger/WsLogWriter.cpp
  ../../Auth/WsAbstractAuth.cpp
  ../../Auth/WsAuthenticator.cpp
  ../../ConfigManager/WsGlobalProperties.cpp
  )

INCLUDE_DIRECTORIES(
../..
)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)
