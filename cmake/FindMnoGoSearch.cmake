 #
 # This module defines
 #  MnoGoSearch_INCLUDE_DIR
 #  MnoGoSearch_LIBRARIES, the libraries to link against to use MnoGoSearch.
 #  MnoGoSearch_LIB_DIR, the location of the libraries
 #  MnoGoSearch_FOUND, If false, do not try to use MnoGoSearch
 #
 # Redistribution and use is allowed according to the terms of the BSD license.
 # For details see the accompanying COPYING-CMAKE-SCRIPTS file.
 
 
 FIND_PATH(MnoGoSearch_INCLUDE_DIR udmsearch.h
     /usr/local/mnogosearch/include
 )
 
 FIND_LIBRARY(MnoGoSearch_LIBRARY NAMES mnogosearch PATHS
     /usr/local/mnogosearch/lib
 )
 
 # Copy the results to the output variables.
 IF (MnoGoSearch_INCLUDE_DIR AND MnoGoSearch_LIBRARY)
         SET(MnoGoSearch_FOUND 1)
         SET(MnoGoSearch_LIBRARIES ${MnoGoSearch_LIBRARY})
         SET(MnoGoSearch_INCLUDE_DIRS ${MnoGoSearch_INCLUDE_DIR})
         MESSAGE(STATUS "Found these MnoGoSearch libs: ${MnoGoSearch_LIBRARIES}")
   ELSE (MnoGoSearch_INCLUDE_DIR AND MnoGoSearch_LIBRARY)
         MESSAGE(STATUS "MnoGoSearch libraries not found")
         SET(MnoGoSearch_FOUND 0)
         SET(MnoGoSearch_LIBRARIES)
         SET(MnoGoSearch_INCLUDE_DIRS)
 ENDIF (MnoGoSearch_INCLUDE_DIR AND MnoGoSearch_LIBRARY)
 
