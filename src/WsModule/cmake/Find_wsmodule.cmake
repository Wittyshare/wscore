#- Find wittyshare module includes and library
 #
 # This module defines
 #  WSMODULE_INCLUDE_DIR
 #  WSMODULE_LIBRARIES, the libraries to link against to use WSMODULE.
 #  WSMODULE_LIB_DIR, the location of the libraries
 #  WSMODULE_FOUND, If false, do not try to use WSMODULE
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
 
 FIND_PATH(WSMODULE_INCLUDE_DIR NAMES WsModule/WsModule.h src/WsModule/src/WsModule.h PATHS
     /usr/include
     /usr/local/include
     ${CMAKE_SOURCE_DIR}
     ${WS_PREFIX}/include
 )
 
 IF (WSMODULE_INCLUDE_DIR)
         SET(WSMODULE_FOUND TRUE)
         SET(WSMODULE_INCLUDE_DIRS ${WSMODULE_INCLUDE_DIR})
         MESSAGE(STATUS "Found these WsModule include : ${WSMODULE_INCLUDE_DIRS}")
 ELSE (WSMODULE_INCLUDE_DIR)
         MESSAGE(FATAL_ERROR "WsModule was not found. Make sure WSMODULE_LIBRARY and WSMODULE_INCLUDE_DIR are set.")
         SET(WSMODULE_FOUND FALSE)
 ENDIF (WSMODULE_INCLUDE_DIR)


IF(EXISTS ${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
 FIND_LIBRARY(WSMODULE_LIBRARY NAMES WsModule PATHS
     /usr/lib
     /usr/local/lib
     /usr/local/lib/WsModule
     ${WS_PREFIX}/lib
 )
ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
 
 # Copy the results to the output variables.
 IF (WSMODULE_LIBRARY)
         SET(WSMODULE_LIBRARIES ${WSMODULE_LIBRARY})
         MESSAGE(STATUS "Found these WsModule libs    : ${WSMODULE_LIBRARIES}")
 ELSE (WSMODULE_LIBRARY)
         MESSAGE(STATUS "Cannot Found WsModule libs   : ${WSMODULE_LIBRARIES}")
 ENDIF (WSMODULE_LIBRARY)
 
