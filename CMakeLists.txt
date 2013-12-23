cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME wscore)
SET(CMAKE_INSTALL_PREFIX  /usr/local/wscore)
PROJECT(wscore)

# pour support de std::tr1
# http://www.johndcook.com/cpp_regex.html
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=gnu++11")
#set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=gnu++11")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HARU_PREFIX} ${GDCORE_PREFIX} ${GDWTCORE_PREFIX})

ADD_DEFINITIONS(-DHAS_DAEMON)

SET (APPNAME_SRCS
   src/FileSystemTree/WsAbstractNode.cpp
   src/FileSystemTree/WsDirNode.cpp
   src/FileSystemTree/WsDirNode.h
   src/FileSystemTree/WsFileNode.cpp
   src/FileSystemTree/WsFileNode.h
   src/FileSystemTree/WsNode.cpp
   src/FileSystemTree/WsNode.h
   src/FileSystemTree/WsFileSystemTree.cpp
   src/FileSystemTree/WsFileSystemTree.h

   src/DirCrawler/WsDirectoryCrawler.cpp
   src/DirCrawler/WsDirectoryCrawler.h

   src/ConfigManager/WsNodeProperties.cpp  
   src/ConfigManager/WsNodeResources.cpp
   src/ConfigManager/WsAbstractProperties.cpp
   src/ConfigManager/WsGlobalProperties.cpp
   src/ConfigManager/WsLayoutProperties.cpp

   src/FsClient/WsFsTreeClient.cpp
   src/FsClient/WsFsDaemonClient.cpp
   src/FsClient/WsAbstractFsClient.cpp

   src/FsTreeOp/WsFsTreeOperations.cpp
   src/FsTreeOp/WsFsTreeConsultation.cpp
   src/FsTreeOp/WsFsTreeModification.cpp

   src/Tree/WsMenuTree.cpp
   src/Tree/WsAccessTree.cpp
   src/Tree/WsAbstractTree.cpp

   src/Serializer/WsTreeSerializer.cpp
   src/Serializer/WsArraySerializer.cpp
   src/Serializer/WsTreeDeserializer.cpp
   src/Serializer/WsArrayDeserializer.cpp

   
   src/Updater/WsFsTreeUpdater.cpp
   src/Updater/WsAbstractMonitor.cpp
   src/Updater/WsMonitor.cpp

   src/TreeTraversal/WsTreeTraversal.cpp

   src/User/WsUser.cpp

   src/Auth/WsAbstractAuth.cpp
   src/Auth/WsAuthenticator.cpp

   src/Logger/WsLogger.cpp          
   src/Logger/WsLogWriter.cpp
   src/Logger/WsCrashLogger.cpp

   src/Search/WsSearch.cpp
   src/Search/WsResultItem.cpp
   src/Search/WsAbstractSearch.cpp

   src/Compression/WsCompressor.cpp
   src/Compression/WsDecompressor.cpp

    )

INCLUDE(./cmake/FindJsoncpp.cmake)
INCLUDE(./cmake/FindZmq.cmake)
INCLUDE(./cmake/FindFam.cmake)
INCLUDE(./cmake/FindZlib.cmake)

IF(NOT JSONCPP_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

IF(NOT ZMQ_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs zmq libraries... Abord.")
ENDIF(NOT ZMQ_FOUND)

IF(NOT FAM_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs fam libraries... Abord.")
ENDIF(NOT FAM_FOUND)

IF(NOT ZLIB_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs zlib libraries... Abord.")
ENDIF(NOT ZLIB_FOUND)

INCLUDE_DIRECTORIES(
    ${JSONCPP_INCLUDE_DIR}
    ${ZMQ_INCLUDE_DIR}
    ${FAM_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    src
    )

SET ( APP_LIBRARIES
    dl
    ${JSONCPP_LIBRARIES}
    ${ZMQ_LIBRARY}
    ${FAM_LIBRARY}
    ${ZLIB_LIBRARY}
    )

   SET ( APP_LIBRARIES
      ${APP_LIBRARIES}
      WsModule
    )

SUBDIRS (
    src/WsSearchModules
    src/WsAuthModules
    )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)

