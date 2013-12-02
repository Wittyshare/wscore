cmake_minimum_required(VERSION 2.8.3)
PROJECT(FS_TEST)
SET(APPNAME node_tests)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

SET (APPNAME_SRCS
  main.cpp
  ../../src/FileSystemTree/WsNode.cpp
  ../../src/FileSystemTree/WsFileNode.cpp
  ../../src/FileSystemTree/WsDirNode.cpp
  ../../src/FileSystemTree/WsFileSystemTree.cpp
  ../../src/FileSystemTree/WsAbstractNode.cpp
  ../../src/DirCrawler/WsDirectoryCrawler.cpp
  ../../src/ConfigManager/WsAbstractProperties.cpp
  ../../src/ConfigManager/WsNodeProperties.cpp
  ../../src/ConfigManager/WsNodeResources.cpp
  ../../src/ConfigManager/WsGlobalProperties.cpp
  ../../src/Logger/WsLogger.cpp
  ../../src/Logger/WsLogWriter.cpp
  )
INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake) 

IF(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs boost libraries... Abord.")
ENDIF(NOT Boost_FOUND)

IF(NOT JSONCPP_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIR}
    ../../src
    )

SET ( APP_LIBRARIES
    ${Boost_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    )

ADD_EXECUTABLE(
    ${APPNAME}
    ${APPNAME_SRCS} 
    )

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )
