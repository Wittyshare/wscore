cmake_minimum_required(VERSION 2.8.3)
PROJECT(FSDAEMON_TEST)

ADD_DEFINITIONS(-DHAS_DAEMON)
SET(APPNAME fsdaemon_tests)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

ADD_DEFINITIONS(-DHAS_DAEMON)
SET (APPNAME_SRCS
  main.cpp
  ../../src/User/WsUser.cpp
  ../../src/Auth/WsAuthenticator.cpp
  ../../src/Auth/WsAbstractAuth.cpp
  ../../src/DirCrawler/WsDirectoryCrawler.cpp
  ../../src/ConfigManager/WsGlobalProperties.cpp
  ../../src/ConfigManager/WsAbstractProperties.cpp
  ../../src/ConfigManager/WsNodeProperties.cpp
    ../../src/ConfigManager/WsNodeResources.cpp
    ../../src/FileSystemTree/WsFileSystemTree.cpp
    ../../src/FileSystemTree/WsNode.cpp
    ../../src/FileSystemTree/WsDirNode.cpp
    ../../src/FileSystemTree/WsFileNode.cpp
    ../../src/FileSystemTree/WsAbstractNode.cpp
    ../../src/Tree/WsAbstractTree.cpp
    ../../src/Tree/WsAccessTree.cpp
    ../../src/Serializer/WsTreeDeserializer.cpp
    ../../src/Serializer/WsArrayDeserializer.cpp
    ../../src/TreeTraversal/WsTreeTraversal.cpp
    ../../src/FsClient/WsFsTreeClient.cpp
    ../../src/FsClient/WsFsDaemonClient.cpp
    ../../src/FsClient/WsAbstractFsClient.cpp
    ../../src/Updater/WsFsTreeUpdater.cpp
    ../../src/FsTreeOp/WsFsTreeConsultation.cpp
    ../../src/FsTreeOp/WsFsTreeOperations.cpp
    ../../src/FsTreeOp/WsFsTreeModification.cpp
    ../../src/Updater/WsAbstractMonitor.cpp
    ../../src/Updater/WsMonitor.cpp
    ../../src/Logger/WsLogger.cpp
    ../../src/Logger/WsLogWriter.cpp
    ../../src/Search/WsResultItem.cpp
    ../../src/Search/WsSearch.cpp
    ../../src/Search/WsAbstractSearch.cpp
    ../../src/Compression/WsDecompressor.cpp
    ../../src/Compression/WsCompressor.cpp
    )

#INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdcore.cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
INCLUDE(../../cmake/FindZmq.cmake)
INCLUDE(../../cmake/FindFam.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindZlib.cmake)

#INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdcore.cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
INCLUDE(../../cmake/FindZmq.cmake)
INCLUDE(../../cmake/FindFam.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindZlib.cmake)

#IF(NOT GDCORE_FOUND)
#    MESSAGE(FATAL_ERROR "** these program needs gdcore librariries... Abord.")
#ENDIF(NOT GDCORE_FOUND)

IF(NOT JSONCPP_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

IF(NOT ZMQ_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs zmq libraries... Abord.")
ENDIF(NOT ZMQ_FOUND)

IF(NOT FAM_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs fam libraries... Abord.")
ENDIF(NOT FAM_FOUND)

INCLUDE_DIRECTORIES(
	${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${JSONCPP_INCLUDE_DIR}
    ${ZMQ_INCLUDE_DIR}
    ${FAM_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ../../src
    )

SET ( APP_LIBRARIES
    ${JSONCPP_LIBRARIES}
    ${ZMQ_LIBRARY}
    ${FAM_LIBRARY}
   ${Boost_LIBRARIES}
    ${ZLIB_LIBRARY}
    )   

ADD_EXECUTABLE(
    ${APPNAME}
    ${APPNAME_SRCS} 
    )

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )

