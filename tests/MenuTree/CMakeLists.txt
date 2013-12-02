cmake_minimum_required(VERSION 2.8.3)
PROJECT(DIR_CRAWLER_TEST)
SET(APPNAME MenuTree_tests)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

SET (APPNAME_SRCS
  #main.cpp
#MenuTreeTest.cpp
../../src/Tree/WsAbstractTree.cpp
../../src/Tree/WsMenuTree.cpp
../../src/Tree/WsAccessTree.cpp
../../src/Serializer/WsTreeSerializer.cpp
../../src/Serializer/WsTreeDeserializer.cpp
../../src/Serializer/WsDirContentsSerializer.cpp
../../src/Serializer/WsDirContentsDeserializer.cpp
../../src/TreeTraversal/WsTreeTraversal.cpp
../../src/FileSystemTree/WsAbstractNode.cpp
../../src/FileSystemTree/WsNode.cpp
../../src/FileSystemTree/WsFileNode.cpp
../../src/FileSystemTree/WsDirNode.cpp
../../src/FileSystemTree/WsFileSystemTree.cpp
../../src/DirCrawler/WsDirectoryCrawler.cpp
../../src/ConfigManager/WsNodeProperties.cpp
../../src/ConfigManager/WsAbstractProperties.cpp
../../src/Logger/WsLogger.cpp
../../src/Logger/WsLogWriter.cpp
../../src/ConfigManager/WsGlobalProperties.cpp
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

