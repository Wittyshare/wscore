cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModRSS")

SET ( APPNAME_SRCS 
     src/WsModRSS.cpp
     src/WsRSS.cpp
     src/WsSearchRSS.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

