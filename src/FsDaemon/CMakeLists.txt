cmake_minimum_required(VERSION 2.8.3)

PROJECT(DIR_FSDAEMON)
SET(APPNAME fsdaemon)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

SET (APPNAME_SRCS
    ../FsDaemon/WsFsDaemonLoader.cpp
    ../FsDaemon/WsFsDaemon.cpp
    ../FsDaemon/WsUserSession.cpp

    ../FsTreeOp/WsFsTreeConsultation.cpp
    ../FsTreeOp/WsFsTreeModification.cpp
    ../FsTreeOp/WsFsTreeOperations.cpp

    ../Updater/WsFsTreeUpdater.cpp
    ../Updater/WsAbstractMonitor.cpp
    ../Updater/WsMonitor.cpp

    ../FileSystemTree/WsNode.cpp
    ../FileSystemTree/WsFileNode.cpp
    ../FileSystemTree/WsDirNode.cpp
    ../FileSystemTree/WsFileSystemTree.cpp
    ../FileSystemTree/WsAbstractNode.cpp

    ../DirCrawler/WsDirectoryCrawler.cpp

    ../ConfigManager/WsNodeProperties.cpp
    ../ConfigManager/WsNodeResources.cpp
    ../ConfigManager/WsAbstractProperties.cpp
    ../ConfigManager/WsGlobalProperties.cpp

    ../Auth/WsAuthenticator.cpp
    ../Auth/WsAbstractAuth.cpp

    ../Tree/WsMenuTree.cpp
    ../Tree/WsAccessTree.cpp
    ../Tree/WsAbstractTree.cpp

    ../Serializer/WsTreeSerializer.cpp
    ../Serializer/WsArraySerializer.cpp

    ../TreeTraversal/WsTreeTraversal.cpp

    ../Logger/WsLogger.cpp
    ../Logger/WsLogWriter.cpp
    ../Logger/WsCrashLogger.cpp

    ../Search/WsSearch.cpp
    ../Search/WsResultItem.cpp
    ../Search/WsAbstractSearch.cpp
    ../Search/WsAbstractSearch.cpp

    ../Compression/WsCompressor.cpp
    ../Compression/WsDecompressor.cpp

)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
INCLUDE(../../cmake/FindZmq.cmake)
INCLUDE(../../cmake/FindFam.cmake)

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
    ../
)

SET ( APP_LIBRARIES
      dl
      ${Boost_LIBRARIES}
      ${JSONCPP_LIBRARIES}
      ${ZMQ_LIBRARY}
      ${FAM_LIBRARY}
    ${ZLIB_LIBRARY}
    )

ADD_EXECUTABLE(
   ${APPNAME}
   ${APPNAME_SRCS} 
)

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${APPNAME}
      DESTINATION bin)

 
