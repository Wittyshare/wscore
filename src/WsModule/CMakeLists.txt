cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModule")

# Cette librairie contient uniquement des fonctions sans interfaces.

SET(DEPLOYROOT /var/www/wt/resources CACHE PATH "These library is deployed under the wt resources")

SET ( APPNAME_SRCS 
     src/WsModule.cpp
    )

IF(NOT GDWTCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdwtcore.cmake)
ENDIF(NOT GDWTCORE_FOUND)

INCLUDE_DIRECTORIES(
    ${GDWTCORE_INCLUDE_DIRS}
)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)

