cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModAuth")
PROJECT($APPNAME)

SET ( APPNAME_SRCS 
     src/WsLdapAuth.cpp
     ../../Logger/WsLogger.cpp
     ../../Logger/WsLogWriter.cpp
    ../../Auth/WsAbstractAuth.cpp
    ../../Auth/WsAuthenticator.cpp
	../../ConfigManager/WsGlobalProperties.cpp
    )

IF(NOT GDCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdcore.cmake)
ENDIF(NOT GDCORE_FOUND)

INCLUDE_DIRECTORIES(
    ${GDCORE_INCLUDE_DIRS}
    ../..
	)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)
