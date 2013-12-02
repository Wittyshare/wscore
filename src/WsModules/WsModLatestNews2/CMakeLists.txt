cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModLatestNews2")

SET ( APPNAME_SRCS 
     src/WsModLatestNews2.cpp
     src/WsLatestNews2.cpp
     src/WsLatestNews.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

