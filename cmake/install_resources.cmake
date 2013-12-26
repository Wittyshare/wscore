
IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
   FILE(GLOB mycmake_files "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*cmake")
   INSTALL(FILES ${mycmake_files} DESTINATION ${CMAKE_ROOT}/Modules/gdcmake)
ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

UNSET(GDRESOURCES)
IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")
  SET ( GDRESOURCES "resources" )
ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources")

SET ( WTRESDIR "/var/www/wt" )
IF( DEFINED GDRESOURCES )
  INSTALL(DIRECTORY ${GDRESOURCES} DESTINATION ${WTRESDIR}  CONFIGURATIONS Debug)
  INSTALL(DIRECTORY ${GDRESOURCES} DESTINATION ${WTRESDIR}  CONFIGURATIONS Release)
ENDIF( DEFINED GDRESOURCES )

# Actually I build the doc only for the debug mode
IF(CMAKE_BUILD_TYPE MATCHES "Debug")
  IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxygen" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
     find_package (Doxygen)
     if (DOXYGEN_FOUND)
         configure_file (${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile @ONLY)
#         EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log)
         EXEC_PROGRAM(${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  #       add_custom_target (html ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${APPNAME}/doxygen/Doxyfile)
     endif (DOXYGEN_FOUND)
  ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxygen" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/doxygen")
ENDIF(CMAKE_BUILD_TYPE MATCHES "Debug")
