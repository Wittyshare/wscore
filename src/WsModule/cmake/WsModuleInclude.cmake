#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${HARU_PREFIX} ${GDCORE_PREFIX} ${GDWTCORE_PREFIX} ${GDDBCORE_PREFIX} ${GDDBWTCORE_PREFIX} ${WS_PREFIX})

MESSAGE(STATUS "Starting search of WsModule for : ${APPNAME} ")

IF(NOT GDWTCORE_FOUND)
   INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_gdwtcore.cmake)
ENDIF(NOT GDWTCORE_FOUND)

IF (NOT ${APPNAME} EQUAL "WsModule")
     IF(EXISTS ${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
        INCLUDE(${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
     ELSE(EXISTS ${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
        INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/Find_wsmodule.cmake)
     ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/src/WsModule/cmake/Find_wsmodule.cmake)
ENDIF(NOT ${APPNAME} EQUAL "WsModule")



INCLUDE_DIRECTORIES(
    ${GDWTCORE_INCLUDE_DIRS}
    ${WSMODULE_INCLUDE_DIRS}
    ${WS_PREFIX}/include
)

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/install_libs.cmake)

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES})
