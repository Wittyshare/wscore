cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModVideo")

SET ( APPNAME_SRCS 
     src/WsVideo.cpp
     src/WsModVideo.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

