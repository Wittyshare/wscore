cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModAnchor")

# Cette librairie contient uniquement des fonctions sans interfaces.

SET(DEPLOYROOT /var/www/wt/resources CACHE PATH "These library is deployed under the wt resources")

SET ( APPNAME_SRCS 
     src/WsModAnchor.cpp
     src/WsAnchor.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

