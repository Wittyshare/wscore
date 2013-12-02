#- Find ZeroMQ includes and library
 #
 # This module defines
 #  FAM_INCLUDE_DIR
 #  FAM_LIBRARIES, the libraries to link against to use FAM.
 #  FAM_LIB_DIR, the location of the libraries
 #  FAM_FOUND, If false, do not try to use FAM
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
 IF (FAM_LIBRARIES AND FAM_INCLUDE_DIR)
    SET(FAM_FIND_QUIETLY TRUE) # Already in cache, be silent
 ENDIF (FAM_LIBRARIES AND FAM_INCLUDE_DIR)
 
 
 FIND_PATH(FAM_INCLUDE_DIR fam.h
     /usr/include
     /usr/local/include
     /usr/local/include/fam
 )
 
 FIND_LIBRARY(FAM_LIBRARY NAMES fam PATHS
     /usr/lib
     /usr/local/lib
     /usr/local/lib/fam
 )
 
 # Copy the results to the output variables.
 IF (FAM_INCLUDE_DIR AND FAM_LIBRARY)
         SET(FAM_FOUND 1)
         SET(FAM_LIBRARIES ${FAM_LIBRARY})
         SET(FAM_INCLUDE_DIRS ${FAM_INCLUDE_DIR})
         
         MESSAGE(STATUS "Found these fam libs: ${FAM_LIBRARIES}")
         
 ELSE (FAM_INCLUDE_DIR AND FAM_LIBRARY)
         SET(FAM_FOUND 0)
         SET(FAM_LIBRARIES)
         SET(FAM_INCLUDE_DIRS)
 ENDIF (FAM_INCLUDE_DIR AND FAM_LIBRARY)
 
 # Report the results.
 IF (NOT FAM_FOUND)
         SET(FAM_DIR_MESSAGE "FAM was not found. Make sure FAM_LIBRARY and FAM_INCLUDE_DIR are set.")
         IF (NOT FAM_FIND_QUIETLY)
                 MESSAGE(STATUS "${FAM_DIR_MESSAGE}")
         ELSE (NOT FAM_FIND_QUIETLY)
                 IF (FAM_FIND_REQUIRED)
                         MESSAGE(FATAL_ERROR "${FAM_DIR_MESSAGE}")
                 ENDIF (FAM_FIND_REQUIRED)
         ENDIF (NOT FAM_FIND_QUIETLY)
 ENDIF (NOT FAM_FOUND)
 
 
 MARK_AS_ADVANCED(
     FAM_INCLUDE_DIRS
     FAM_LIBRARIES
 )
