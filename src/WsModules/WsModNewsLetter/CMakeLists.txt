cmake_minimum_required(VERSION 2.8.3)
SET(APPNAME "WsModNewsLetter")

# Cette librairie contient uniquement des fonctions sans interfaces.

SET(DEPLOYROOT /var/www/wt/resources CACHE PATH "These library is deployed under the wt resources")

SET ( APPNAME_SRCS 
     src/EmailToken.C
     src/WsNewsLetter.cpp
     src/WsModNewsLetter.cpp
    )

INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/WsModuleInclude.cmake)

