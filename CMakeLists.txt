cmake_minimum_required(VERSION 2.8.3)
PROJECT($APPNAME)

# pour support de std::tr1
# http://www.johndcook.com/cpp_regex.html
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=gnu++11")
#set(CMAKE_CSS_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=gnu++11")

SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Deploy the app under this directory")

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

   src/Main/main.cpp
   src/Main/WsFormConfig.cpp
   src/Main/WsLogo.cpp
   src/Main/WsSiteMap.cpp
   src/Main/WsSearchObject.cpp
   src/Main/WsTopBanner.cpp
   src/Main/WsBottomBanner.cpp
   src/Main/WsContent.cpp
   src/Main/WsContentButtonsBar.cpp
   src/Main/WsFileResource.cpp
   src/Main/WsModulesLoader.cpp 
   src/Main/WsFunctionnalities.cpp
   src/Main/WsSiteMapView.cpp
   src/Main/WsSearchView.cpp
   src/Main/WsTemplate.cpp
   src/Main/WsApplication.cpp

   src/Search/WsSearch.cpp
   src/Search/WsResultItem.cpp
   src/Search/WsAbstractSearch.cpp

   src/Compression/WsCompressor.cpp
   src/Compression/WsDecompressor.cpp

    )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdwtcore.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindJsoncpp.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindZmq.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindFam.cmake)
INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindZlib.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

IF(NOT GDWTCORE_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs gdwtcore libraries... Abord.")
ENDIF(NOT GDWTCORE_FOUND)

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
    ${GDWTCORE_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIR}
    ${ZMQ_INCLUDE_DIR}
    ${FAM_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    src
    )

SET ( APP_LIBRARIES
    dl
    ${GDWTCORE_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${ZMQ_LIBRARY}
    ${FAM_LIBRARY}
    ${ZLIB_LIBRARY}
    WsModule
    )

   SET ( APP_LIBRARIES
      ${APP_LIBRARIES}
      WsModule
    )

SUBDIRS (
    src/WsModule
    src/WsModules
    src/WsSearchModules
    src/WsAuthModules
    src/FhtmlEditor
    src/FsDaemon/
    )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_webapp.cmake)

