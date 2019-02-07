# - Find Spark
# Find the native Spark includes and libraries
#
#  SPARK_INCLUDE_DIR         - where to find spark include files
#  SPARK_LIB_DIR             - where to find spark libraries
#  SPARK_LIBRARIES           - list of libraries when using spark (contains first found spark lib, debug or release)
#  SPARK_LIBRARY_DEBUG       - spark debug library
#  SPARK_LIBRARY_RELEASE     - spark release library
#  SPARK_FOUND               - true if a spark library was found
#  SALT_INCLUDE_DIR          - where to find salt include files
#  SALT_LIB_DIR              - where to find salt libraries
#  SALT_LIBRARIES            - list of libraries when using salt (contains first found salt lib, debug or release)
#  SALT_LIBRARY_DEBUG        - salt debug library
#  SALT_LIBRARY_RELEASE      - salt release library
#  SALT_FOUND                - true if a salt library was found
#  KEROSIN_INCLUDE_DIR       - where to find spark include files
#  KEROSIN_LIB_DIR           - where to find spark libraries
#  KEROSIN_LIBRARIES         - list of libraries when using spark (contains first found spark lib, debug or release)
#  KEROSIN_LIBRARY_DEBUG     - spark debug library
#  KEROSIN_LIBRARY_RELEASE   - spark release library
#  KEROSIN_FOUND             - true if a spark library was found
#  ZEITGEIST_INCLUDE_DIR     - where to find zeitgeist include files
#  ZEITGEIST_LIB_DIR         - where to find zeitgeist libraries
#  ZEITGEIST_LIBRARIES       - list of libraries when using zeitgeist (contains first found zeitgeist lib, debug or release)
#  ZEITGEIST_LIBRARY_DEBUG   - zeitgeist debug library
#  ZEITGEIST_LIBRARY_RELEASE - zeitgeist release library
#  ZEITGEIST_FOUND           - true if a zeitgeist library was found
#  OXYGEN_INCLUDE_DIR        - where to find oxygen include files
#  OXYGEN_LIB_DIR            - where to find oxygen libraries
#  OXYGEN_LIBRARIES          - list of libraries when using oxygen (contains first found oxygen lib, debug or release)
#  OXYGEN_LIBRARY_DEBUG      - oxygen debug library
#  OXYGEN_LIBRARY_RELEASE    - oxygen release library
#  OXYGEN_FOUND              - true if a oxygen library was found
#  RCSSNET3D_INCLUDE_DIR     - where to find rcssnet include files
#  RCSSNET3D_LIBRARIES       - List of libraries when using rcssnet.
#  RCSSNET3D_FOUND           - True if rcssnet found.

# Spark section
IF (SPARK_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SPARK_FIND_QUIETLY TRUE)
ENDIF (SPARK_INCLUDE_DIR)

SET(SPARK_DIR $ENV{SPARK_DIR} "C:/Program Files/simspark" "C:/Program Files (x86)/simspark" "C:/library/simspark")

FIND_PATH(SPARK_INCLUDE_DIR spark/spark.h
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(SPARK_NAMES spark spark_debug)
FIND_LIBRARY(SPARK_LIBRARY NAMES ${SPARK_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)
GET_FILENAME_COMPONENT(SPARK_LIB_DIR ${SPARK_LIBRARY} PATH)

# handle the QUIETLY and REQUIRED arguments and set SPARK_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SPARK DEFAULT_MSG SPARK_LIBRARY
         SPARK_INCLUDE_DIR)

IF(SPARK_FOUND)
  SET( SPARK_LIBRARIES ${SPARK_LIBRARY} )
  STRING(REPLACE "spark." "spark_debug." SPARK_LIBRARY_DEBUG ${SPARK_LIBRARY})
  STRING(REPLACE "spark_debug." "spark." SPARK_LIBRARY_RELEASE ${SPARK_LIBRARY})
ELSE(SPARK_FOUND)
  SET( SPARK_LIBRARIES )
ENDIF(SPARK_FOUND)

MARK_AS_ADVANCED( SPARK_LIBRARY SPARK_LIBRARY_RELEASE SPARK_LIBRARY_DEBUG SPARK_INCLUDE_DIR )

# Salt section
IF (SALT_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SALT_FIND_QUIETLY TRUE)
ENDIF (SALT_INCLUDE_DIR)

FIND_PATH(SALT_INCLUDE_DIR salt/salt.h
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(SALT_NAMES salt salt_debug)
FIND_LIBRARY(SALT_LIBRARY NAMES ${SALT_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set SALT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SALT DEFAULT_MSG SALT_LIBRARY
         SALT_INCLUDE_DIR)

IF(SALT_FOUND)
  SET( SALT_LIBRARIES ${SALT_LIBRARY} )
  STRING(REPLACE "salt." "salt_debug." SALT_LIBRARY_DEBUG ${SALT_LIBRARY})
  STRING(REPLACE "salt_debug." "salt." SALT_LIBRARY_RELEASE ${SALT_LIBRARY})
ELSE(SALT_FOUND)
  SET( SALT_LIBRARIES )
ENDIF(SALT_FOUND)

MARK_AS_ADVANCED( SALT_LIBRARY SALT_LIBRARY_RELEASE SALT_LIBRARY_DEBUG SALT_INCLUDE_DIR )

# Zeitgeist section
IF (ZEITGEIST_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ZEITGEIST_FIND_QUIETLY TRUE)
ENDIF (ZEITGEIST_INCLUDE_DIR)

FIND_PATH(ZEITGEIST_INCLUDE_DIR zeitgeist/zeitgeist.h
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(ZEITGEIST_NAMES zeitgeist zeitgeist_debug)
FIND_LIBRARY(ZEITGEIST_LIBRARY NAMES ${ZEITGEIST_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set ZEITGEIST_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZEITGEIST DEFAULT_MSG ZEITGEIST_LIBRARY
         ZEITGEIST_INCLUDE_DIR)

IF(ZEITGEIST_FOUND)
  SET( ZEITGEIST_LIBRARIES ${ZEITGEIST_LIBRARY} )
  STRING(REPLACE "zeitgeist." "zeitgeist_debug." ZEITGEIST_LIBRARY_DEBUG ${ZEITGEIST_LIBRARY})
  STRING(REPLACE "zeitgeist_debug." "zeitgeist." ZEITGEIST_LIBRARY_RELEASE ${ZEITGEIST_LIBRARY})
ELSE(ZEITGEIST_FOUND)
  SET( ZEITGEIST_LIBRARIES )
ENDIF(ZEITGEIST_FOUND)

MARK_AS_ADVANCED( ZEITGEIST_LIBRARY ZEITGEIST_LIBRARY_RELEASE ZEITGEIST_LIBRARY_DEBUG ZEITGEIST_INCLUDE_DIR )

# Oxygen section
IF (OXYGEN_INCLUDE_DIR)
  # Already in cache, be silent
  SET(OXYGEN_FIND_QUIETLY TRUE)
ENDIF (OXYGEN_INCLUDE_DIR)

FIND_PATH(OXYGEN_INCLUDE_DIR oxygen/oxygen.h
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(OXYGEN_NAMES oxygen oxygen_debug)
FIND_LIBRARY(OXYGEN_LIBRARY NAMES ${OXYGEN_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set OXYGEN_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OXYGEN DEFAULT_MSG OXYGEN_LIBRARY
         OXYGEN_INCLUDE_DIR)

IF(OXYGEN_FOUND)
  SET( OXYGEN_LIBRARIES ${OXYGEN_LIBRARY} )
  STRING(REPLACE "oxygen." "oxygen_debug." OXYGEN_LIBRARY_DEBUG ${OXYGEN_LIBRARY})
  STRING(REPLACE "oxygen_debug." "oxygen." OXYGEN_LIBRARY_RELEASE ${OXYGEN_LIBRARY})
ELSE(OXYGEN_FOUND)
  SET( OXYGEN_LIBRARIES )
ENDIF(OXYGEN_FOUND)

MARK_AS_ADVANCED( OXYGEN_LIBRARY OXYGEN_LIBRARY_RELEASE OXYGEN_LIBRARY_DEBUG OXYGEN_INCLUDE_DIR )

# Kerosin section
IF (KEROSIN_INCLUDE_DIR)
  # Already in cache, be silent
  SET(KEROSIN_FIND_QUIETLY TRUE)
ENDIF (KEROSIN_INCLUDE_DIR)

FIND_PATH(KEROSIN_INCLUDE_DIR kerosin/kerosin.h
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(KEROSIN_NAMES kerosin kerosin_debug)
FIND_LIBRARY(KEROSIN_LIBRARY NAMES ${KEROSIN_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set KEROSIN_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KEROSIN DEFAULT_MSG KEROSIN_LIBRARY
         KEROSIN_INCLUDE_DIR)

IF(KEROSIN_FOUND)
  SET( KEROSIN_LIBRARIES ${KEROSIN_LIBRARY} )
  STRING(REPLACE "kerosin." "kerosin_debug." KEROSIN_LIBRARY_DEBUG ${KEROSIN_LIBRARY})
  STRING(REPLACE "kerosin_debug." "kerosin." KEROSIN_LIBRARY_RELEASE ${KEROSIN_LIBRARY})
ELSE(KEROSIN_FOUND)
  SET( KEROSIN_LIBRARIES )
ENDIF(KEROSIN_FOUND)

MARK_AS_ADVANCED( KEROSIN_LIBRARY KEROSIN_LIBRARY_RELEASE KEROSIN_LIBRARY_DEBUG KEROSIN_INCLUDE_DIR )

# rcssnet3D
IF (RCSSNET3D_INCLUDE_DIR)
  # Already in cache, be silent
  SET(RCSSNET3D_FIND_QUIETLY TRUE)
ENDIF (RCSSNET3D_INCLUDE_DIR)

FIND_PATH(RCSSNET3D_INCLUDE_DIR rcssnet/addr.hpp
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(RCSSNET3D_NAMES rcssnet3D rcssnet3D_debug)
FIND_LIBRARY(RCSSNET3D_LIBRARY NAMES ${RCSSNET3D_NAMES}
  PATHS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set RCSSNET3D_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(RCSSNET3D DEFAULT_MSG RCSSNET3D_LIBRARY
         RCSSNET3D_INCLUDE_DIR)

IF(RCSSNET3D_FOUND)
  SET( RCSSNET3D_LIBRARIES ${RCSSNET3D_LIBRARY} )
ELSE(RCSSNET3D_FOUND)
  SET( RCSSNET3D_LIBRARIES )
ENDIF(RCSSNET3D_FOUND)

MARK_AS_ADVANCED( RCSSNET3D_LIBRARY RCSSNET3D_INCLUDE_DIR )
