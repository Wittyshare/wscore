#
# We set:
# - WSCORE_INCLUDE_DIR  : the ws include dir
# - WSCORE_INCLUDE_DIRS : the ws include dir + all required include
# - WSCORE_LIBRARY      : the ws libraries
# - WSCORE_LIBRARIES    : the ws libraries + all required libraries
# - GDDEBUG             : = 1 enabled else disabled
# - WSCORE_FOUND

FIND_PATH(WSCORE_INCLUDE_DIR FileSystemTree/WsNode.h PATHS
    ${WSCORE_PREFIX}/include/wscore/
)

IF( WSCORE_INCLUDE_DIR )
  IF(CMAKE_BUILD_TYPE MATCHES "Debug")
     FIND_LIBRARY(WSCORE_LIBRARY       NAMES wscored PATHS ${WSCORE_PREFIX}/lib)
     ADD_DEFINITIONS(-DGDDEBUG=1)
   ELSE(CMAKE_BUILD_TYPE MATCHES "Debug")
     FIND_LIBRARY(WSCORE_LIBRARY       NAMES wscore  PATHS ${WSCORE_PREFIX}/lib)
     ADD_DEFINITIONS(-DGDDEBUG=0)
   ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")

   IF( WSCORE_LIBRARY )
     SET(WSCORE_FOUND TRUE)
   ELSE(WSCORE_LIBRARY)
     SET(WSCORE_FOUND FALSE)
   ENDIF(WSCORE_LIBRARY)

   IF(WSCORE_FOUND)
      MESSAGE(STATUS "Found the ws ${CMAKE_BUILD_TYPE} libraries at ${WSCORE_LIBRARY}")
      MESSAGE(STATUS "Found the ws ${CMAKE_BUILD_TYPE} headers   at ${WSCORE_INCLUDE_DIR}")
   ELSE(WSCORE_FOUND)
      MESSAGE(FATAL "Could NOT find ws ${CMAKE_BUILD_TYPE} libraries")
   ENDIF(WSCORE_FOUND)
ENDIF( WSCORE_INCLUDE_DIR )

IF(WSCORE_FOUND)
    INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindJsoncpp.cmake)
    INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindZmq.cmake)
    INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindFam.cmake)
    INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/FindZlib.cmake)
    INCLUDE(${CMAKE_ROOT}/Modules/gdcmake/WtFindBoost-cmake)


    IF(NOT JSONCPP_FOUND)
        MESSAGE(FATAL_ERROR "** these program needs jsoncpp libraries... Abord.")
    ENDIF(NOT JSONCPP_FOUND)

    IF(NOT ZMQ_FOUND)
        MESSAGE(FATAL_ERROR "** these program needs zmq libraries... Abord.")
    ENDIF(NOT ZMQ_FOUND)

    IF(NOT FAM_FOUND)
        MESSAGE(FATAL_ERROR "** these program needs fam libraries... Abord.")
    ENDIF(NOT FAM_FOUND)

    IF(NOT ZLIB_FOUND)
        MESSAGE(FATAL_ERROR "** these program needs zlib libraries... Abord.")
    ENDIF(NOT ZLIB_FOUND)

    SET(WSCORE_INCLUDE_DIRS
        ${JSONCPP_INCLUDE_DIR}
        ${ZMQ_INCLUDE_DIR}
        ${FAM_INCLUDE_DIR}
        ${ZLIB_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${WSCORE_INCLUDE_DIR}
        )

    SET(WSCORE_LIBRARIES
        dl
        ${Boost_LIBRARIES}
        ${JSONCPP_LIBRARIES}
        ${ZMQ_LIBRARY}
        ${FAM_LIBRARY}
        ${ZLIB_LIBRARY}
        ${WSCORE_LIBRARY}
        )

ENDIF(WSCORE_FOUND)

