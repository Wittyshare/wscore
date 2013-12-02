cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModBreadCrumb")

# Cette librairie contient uniquement des fonctions sans interfaces.

SET(DEPLOYROOT /var/www/wt/resources CACHE PATH "These library is deployed under the wt resources")

SET ( APPNAME_SRCS 
     src/WsBreadCrumb.cpp
     src/WsModBreadCrumb.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

