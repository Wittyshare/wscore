cmake_minimum_required(VERSION 2.8.3)
PROJECT(Search_TESTS)
SET(APPNAME Search_tests)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

SET (APPNAME_SRCS
	main.cpp
	../../src/Search/WsSearch.cpp
    ../../src/Search/WsAbstractSearch.cpp
	../../src/Search/WsResultItem.cpp

	../../src/User/WsUser.cpp
	../../src/User/WsUser.h
	../../src/User/WsUser.cpp
    ../../src/Auth/WsAbstractAuth.cpp
    ../../src/Auth/WsAuthenticator.cpp
	../../src/DirCrawler/WsDirectoryCrawler.cpp
	../../src/ConfigManager/WsGlobalProperties.cpp
	../../src/FileSystemTree/WsFileSystemTree.cpp
	../../src/FileSystemTree/WsNode.cpp
	../../src/FileSystemTree/WsDirNode.cpp
	../../src/FileSystemTree/WsFileNode.cpp
    ../../src/FileSystemTree/WsAbstractNode.cpp
	../../src/FsClient/WsFsTreeClient.cpp
	../../src/FsClient/WsFsDaemonClient.cpp
    ../../src/FsClient/WsAbstractFsClient.cpp
	../../src/Updater/WsFsTreeUpdater.cpp
    ../../src/Updater/WsAbstractMonitor.cpp
	../../src/Updater/WsMonitor.cpp
	../../src/ConfigManager/WsAbstractProperties.cpp
	../../src/ConfigManager/WsFileProperties.cpp
	../../src/MenuTree/WsMenuTree.cpp
    ../../src/MenuTree/WsAccessTree.cpp
    ../../src/MenuTree/WsDirContentsDeserializer.cpp
    ../../src/MenuTree/WsDirContentsSerializer.cpp
    ../../src/MenuTree/WsTreeDeserializer.cpp
    ../../src/TreeTraversal/WsTreeTraversal.cpp
    ../../src/Logger/WsLogger.cpp
    ../../src/Logger/WsLogWriter.cpp
	)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdcore.cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
INCLUDE(../../cmake/FindZmq.cmake)
INCLUDE(../../cmake/FindFam.cmake)

IF(NOT GDCORE_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs gdcore libraries... Abord.")
ENDIF(NOT GDCORE_FOUND)

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
        ${GDCORE_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}
	${JSONCPP_INCLUDE_DIR}
	${ZMQ_INCLUDE_DIR}
	${FAM_INCLUDE_DIR}
    ../../src
	)

SET ( APP_LIBRARIES
      ${GDCORE_LIBRARIES}
      ${JSONCPP_LIBRARIES}
      ${ZMQ_LIBRARY}
      ${FAM_LIBRARY}
)

ADD_EXECUTABLE(
	${APPNAME}
	${APPNAME_SRCS} 
	)

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} ssl crypto pq crypt rt m z)

