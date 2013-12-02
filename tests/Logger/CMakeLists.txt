cmake_minimum_required(VERSION 2.8.3)
PROJECT(LOG_TEST)
SET(APPNAME log_test)

SET (APPNAME_SRCS
    ../../src/Logger/WsLogger.cpp
    ../../src/Logger/WsLogWriter.cpp
    TestLogger.cpp
    )

INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
FIND_PACKAGE (Threads)

IF(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs boost libraries... Abord.")
ENDIF(NOT Boost_FOUND)

IF(NOT JSONCPP_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
	${JSONCPP_INCLUDE_DIR}
    ../../src
	)

SET ( APP_LIBRARIES
    ${Boost_LIBRARIES}
	${JSONCPP_LIBRARIES}
    )

ADD_EXECUTABLE(
	${APPNAME}
	${APPNAME_SRCS} 
	)
target_link_libraries (${APPNAME} ${CMAKE_THREAD_LIBS_INIT} ${APP_LIBRARIES})
