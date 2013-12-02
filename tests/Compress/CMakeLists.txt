cmake_minimum_required(VERSION 2.8.3)
PROJECT(DIR_CRAWLER_TEST)
SET(APPNAME compression_tests)
SET(APPBIN "${APPNAME}.wt ${APPNAME}.wthttpd")
SET(DEPLOYROOT /var/www/wt CACHE PATH "Path to deploy examples into")

SET (APPNAME_SRCS
    main.cpp
    ../../src/Compression/WsCompressor.cpp
    ../../src/Compression/WsDecompressor.cpp
    ../../src/Logger/WsLogger.cpp
    ../../src/Logger/WsLogWriter.cpp
    ../../src/ConfigManager/WsGlobalProperties.cpp
    ../../src/ConfigManager/WsAbstractProperties.cpp
    )


INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)
INCLUDE(../../cmake/FindJsoncpp.cmake)
INCLUDE(FindZlib.cmake)

IF(NOT Boost_FOUND)
    MESSAGE(FATAL_ERROR "** these program needs boost libraries... Abord.")
ENDIF(NOT Boost_FOUND)

IF(NOT JSONCPP_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
ENDIF(NOT JSONCPP_FOUND)

IF(NOT ZLIB_FOUND)
	MESSAGE(FATAL_ERROR "** these program needs zlib libraries... Abord.")
ENDIF(NOT ZLIB_FOUND)


INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ../../src
    )

SET ( APP_LIBRARIES
    ${Boost_LIBRARIES}
    ${JSONCPP_LIBRARIES}
    ${ZLIB_LIBRARY}
    )

ADD_EXECUTABLE(
    ${APPNAME}
    ${APPNAME_SRCS} 
    )

TARGET_LINK_LIBRARIES(${APPNAME} ${APP_LIBRARIES} )

