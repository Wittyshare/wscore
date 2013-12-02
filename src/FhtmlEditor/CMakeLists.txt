cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "fhtmlEditor")
PROJECT($APPNAME)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt/wittyshare)

SET (APPNAME_SRCS
 src/main.cpp
 ../ConfigManager/WsLayoutProperties.cpp
 ../ConfigManager/WsAbstractProperties.cpp
 ../ConfigManager/WsGlobalProperties.cpp
 ../Logger/WsLogger.cpp
 ../Logger/WsLogWriter.cpp
)

IF(NOT GDWTCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdwtcore.cmake)
ENDIF(NOT GDWTCORE_FOUND)

IF(NOT GDWTCORE_FOUND)
  MESSAGE(FATAL_ERROR "** these program needs gdwtcore libraries... Abord.")
ENDIF(NOT GDWTCORE_FOUND)

IF(NOT JSONCPP_FOUND)
   INCLUDE(${CMAKE_HOME_DIRECTORY}/cmake/FindJsoncpp.cmake)
ENDIF(NOT JSONCPP_FOUND)

IF(NOT JSONCPP_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

INCLUDE_DIRECTORIES(
  ${GDWT_INCLUDE_DIRS}
  ${JSONCPP_INCLUDE_DIR}
)

SET ( APP_LIBRARIES
  ${GDWTCORE_LIBRARIES}
  ${JSONCPP_LIBRARIES}
  )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_webapp.cmake)
