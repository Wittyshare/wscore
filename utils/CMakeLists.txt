cmake_minimum_required(VERSION 2.8.3)
PROJECT(MENU_PERM)
SET(APPNAME menu_perm)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")
#SET(SERVER 1)
ADD_DEFINITIONS(-DSERVER=${SERVER})


SET (APPNAME_SRCS
    WsAccessTreeDisplayer.cpp
    ../src/Auth/WsAuthenticator.cpp
    ../src/Auth/WsAbstractAuth.cpp
    ../src/DirCrawler/WsDirectoryCrawler.cpp
    ../src/ConfigManager/WsGlobalProperties.cpp
    ../src/ConfigManager/WsAbstractProperties.cpp
    ../src/FileSystemTree/WsFileSystemTree.cpp
    ../src/FileSystemTree/WsNode.cpp
    ../src/FileSystemTree/WsDirNode.cpp
    ../src/FileSystemTree/WsFileNode.cpp
    ../src/FileSystemTree/WsAbstractNode.cpp
    ../src/ConfigManager/WsNodeProperties.cpp
    ../src/Tree/WsAbstractTree.cpp
    ../src/Tree/WsAccessTree.cpp
    ../src/Serializer/WsTreeSerializer.cpp
    ../src/Tree/WsMenuTree.cpp
    ../src/TreeTraversal/WsTreeTraversal.cpp
    ../src/Logger/WsLogger.cpp
    ../src/Logger/WsLogWriter.cpp
    ../src/ConfigManager/WsNodeResources.cpp
    )

INCLUDE(../cmake/FindJsoncpp.cmake)
INCLUDE(../cmake/FindZmq.cmake)
INCLUDE(../cmake/FindFam.cmake)
INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)

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
    ../src
    )

SET ( APP_LIBRARIES
    ${JSONCPP_LIBRARIES}
    ${ZMQ_LIBRARY}
      ${FAM_LIBRARY}
   ${Boost_LIBRARIES}
    )   

ADD_EXECUTABLE(
    ${APPNAME}
    ${APPNAME_SRCS} 
    )

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )

