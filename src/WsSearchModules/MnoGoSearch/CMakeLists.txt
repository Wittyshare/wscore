cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModMnoGoSearch")
PROJECT($APPNAME)

SET ( APPNAME_SRCS 
     src/WsMnoGoSearch.cpp
     ../../Search/WsResultItem.cpp
     ../../Search/WsAbstractSearch.cpp
     ../../Logger/WsLogger.cpp
     ../../Logger/WsLogWriter.cpp
     ../../User/WsUser.cpp
     ../../User/WsUser.h
     ../../User/WsUser.cpp
     ../../Auth/WsAbstractAuth.cpp
     ../../Auth/WsAuthenticator.cpp
     ../../DirCrawler/WsDirectoryCrawler.cpp
     ../../ConfigManager/WsGlobalProperties.cpp
     ../../FileSystemTree/WsFileSystemTree.cpp
     ../../FileSystemTree/WsNode.cpp
     ../../FileSystemTree/WsDirNode.cpp
     ../../FileSystemTree/WsFileNode.cpp
     ../../FileSystemTree/WsAbstractNode.cpp
     ../../FsClient/WsFsTreeClient.cpp
     ../../FsClient/WsFsDaemonClient.cpp
     ../../FsClient/WsAbstractFsClient.cpp
     ../../Updater/WsFsTreeUpdater.cpp
     ../../Updater/WsAbstractMonitor.cpp
     ../../Updater/WsMonitor.cpp
     ../../ConfigManager/WsAbstractProperties.cpp
     ../../ConfigManager/WsNodeProperties.cpp
     ../../Tree/WsMenuTree.cpp
     ../../Tree/WsAbstractTree.cpp
     ../../Serializer/WsTreeDeserializer.cpp
     ../../TreeTraversal/WsTreeTraversal.cpp
     ../../Logger/WsLogger.cpp
     ../../Logger/WsLogWriter.cpp
    )

INCLUDE(../../../cmake/FindMnoGoSearch.cmake)
INCLUDE(../../../cmake/FindJsoncpp.cmake)
INCLUDE(../../../cmake/FindFam.cmake)

IF(NOT GDCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdcore.cmake)
ENDIF(NOT GDCORE_FOUND)


IF(NOT Boost_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs boost librariries... Abord.")
ENDIF(NOT Boost_FOUND)

IF(NOT JSONCPP_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

IF(NOT FAM_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs fam libraries... Abord.")
ENDIF(NOT FAM_FOUND)

IF(NOT MnoGoSearch_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs mnogosearch libraries... Abord.")
ENDIF(NOT MnoGoSearch_FOUND)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${MnoGoSearch_INCLUDE_DIRS}
	${JSONCPP_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${FAM_INCLUDE_DIR}
    ../..
	)


SET ( APP_LIBRARIES
    ${MnoGoSearch_LIBRARIES}
    WsModule
    )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)
