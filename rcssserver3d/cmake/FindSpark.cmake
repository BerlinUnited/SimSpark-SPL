# - Find Spark
# Find the native Spark includes and libraries
#
#  SPARK_INCLUDE_DIR - where to find spark include files
#  SPARK_LIB_DIR     - where to find spark libraries
#  SPARK_LIBRARIES   - List of libraries when using spark.
#  SPARK_FOUND       - True if spark found.
#  SALT_INCLUDE_DIR - where to find salt include files
#  SALT_LIBRARIES   - List of libraries when using salt.
#  SALT_FOUND       - True if salt found.
#  ZEITGEIST_INCLUDE_DIR - where to find zeitgeist include files
#  ZEITGEIST_LIBRARIES   - List of libraries when using zeitgeist.
#  ZEITGEIST_FOUND       - True if zeitgeist found.
#  OXYGEN_INCLUDE_DIR - where to find oxygen include files
#  OXYGEN_LIBRARIES   - List of libraries when using oxygen.
#  OXYGEN_FOUND       - True if oxygen found.
#  KEROSIN_INCLUDE_DIR - where to find kerosin include files
#  KEROSIN_LIBRARIES   - List of libraries when using kerosin.
#  KEROSIN_FOUND       - True if kerosin found.
#  RCSSNET3D_INCLUDE_DIR - where to find kerosin include files
#  RCSSNET3D_LIBRARIES   - List of libraries when using kerosin.
#  RCSSNET3D_FOUND       - True if kerosin found.

# Spark section
IF (SPARK_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SPARK_FIND_QUIETLY TRUE)
ENDIF (SPARK_INCLUDE_DIR)

SET(SPARK_DIR $ENV{SPARK_DIR} "C:/Program Files/simspark" "C:/Program Files (x86)/simspark" "C:/library/simspark")

FIND_PATH(SPARK_INCLUDE_DIR spark/spark.h
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(SPARK_NAMES spark spark_debug)
FIND_LIBRARY(SPARK_LIBRARY NAMES ${SPARK_NAMES}
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)
GET_FILENAME_COMPONENT(SPARK_LIB_DIR ${SPARK_LIBRARY} PATH)

# handle the QUIETLY and REQUIRED arguments and set SPARK_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SPARK DEFAULT_MSG SPARK_LIBRARY
         SPARK_INCLUDE_DIR)

IF(SPARK_FOUND)
  SET( SPARK_LIBRARIES ${SPARK_LIBRARY} )
ELSE(SPARK_FOUND)
  SET( SPARK_LIBRARIES )
ENDIF(SPARK_FOUND)

MARK_AS_ADVANCED( SPARK_LIBRARY SPARK_INCLUDE_DIR )

# Salt section
IF (SALT_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SALT_FIND_QUIETLY TRUE)
ENDIF (SALT_INCLUDE_DIR)

FIND_PATH(SALT_INCLUDE_DIR salt/salt.h
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(SALT_NAMES salt salt_debug)
FIND_LIBRARY(SALT_LIBRARY NAMES ${SALT_NAMES}
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set SALT_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SALT DEFAULT_MSG SALT_LIBRARY
         SALT_INCLUDE_DIR)

IF(SALT_FOUND)
  SET( SALT_LIBRARIES ${SALT_LIBRARY} )
ELSE(SALT_FOUND)
  SET( SALT_LIBRARIES )
ENDIF(SALT_FOUND)

MARK_AS_ADVANCED( SALT_LIBRARY SALT_INCLUDE_DIR )

# Zeitgeist section
IF (ZEITGEIST_INCLUDE_DIR)
  # Already in cache, be silent
  SET(ZEITGEIST_FIND_QUIETLY TRUE)
ENDIF (ZEITGEIST_INCLUDE_DIR)

FIND_PATH(ZEITGEIST_INCLUDE_DIR zeitgeist/zeitgeist.h
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(ZEITGEIST_NAMES zeitgeist zeitgeist_debug)
FIND_LIBRARY(ZEITGEIST_LIBRARY NAMES ${ZEITGEIST_NAMES}
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set ZEITGEIST_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZEITGEIST DEFAULT_MSG ZEITGEIST_LIBRARY
         ZEITGEIST_INCLUDE_DIR)

IF(ZEITGEIST_FOUND)
  SET( ZEITGEIST_LIBRARIES ${ZEITGEIST_LIBRARY} )
ELSE(ZEITGEIST_FOUND)
  SET( ZEITGEIST_LIBRARIES )
ENDIF(ZEITGEIST_FOUND)

MARK_AS_ADVANCED( ZEITGEIST_LIBRARY ZEITGEIST_INCLUDE_DIR )

# Oxygen section
IF (OXYGEN_INCLUDE_DIR)
  # Already in cache, be silent
  SET(OXYGEN_FIND_QUIETLY TRUE)
ENDIF (OXYGEN_INCLUDE_DIR)

FIND_PATH(OXYGEN_INCLUDE_DIR oxygen/oxygen.h
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(OXYGEN_NAMES oxygen oxygen_debug)
FIND_LIBRARY(OXYGEN_LIBRARY NAMES ${OXYGEN_NAMES}
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set OXYGEN_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OXYGEN DEFAULT_MSG OXYGEN_LIBRARY
         OXYGEN_INCLUDE_DIR)

IF(OXYGEN_FOUND)
  SET( OXYGEN_LIBRARIES ${OXYGEN_LIBRARY} )
ELSE(OXYGEN_FOUND)
  SET( OXYGEN_LIBRARIES )
ENDIF(OXYGEN_FOUND)

MARK_AS_ADVANCED( OXYGEN_LIBRARY OXYGEN_INCLUDE_DIR )

# Kerosin section
IF (KEROSIN_INCLUDE_DIR)
  # Already in cache, be silent
  SET(KEROSIN_FIND_QUIETLY TRUE)
ENDIF (KEROSIN_INCLUDE_DIR)

FIND_PATH(KEROSIN_INCLUDE_DIR kerosin/kerosin.h
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(KEROSIN_NAMES kerosin kerosin_debug)
FIND_LIBRARY(KEROSIN_LIBRARY NAMES ${KEROSIN_NAMES}
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark lib/simspark)

# handle the QUIETLY and REQUIRED arguments and set KEROSIN_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(KEROSIN DEFAULT_MSG KEROSIN_LIBRARY
         KEROSIN_INCLUDE_DIR)

IF(KEROSIN_FOUND)
  SET( KEROSIN_LIBRARIES ${KEROSIN_LIBRARY} )
ELSE(KEROSIN_FOUND)
  SET( KEROSIN_LIBRARIES )
ENDIF(KEROSIN_FOUND)

MARK_AS_ADVANCED( KEROSIN_LIBRARY KEROSIN_INCLUDE_DIR )

# rcssnet3D
IF (RCSSNET3D_INCLUDE_DIR)
  # Already in cache, be silent
  SET(RCSSNET3D_FIND_QUIETLY TRUE)
ENDIF (RCSSNET3D_INCLUDE_DIR)

FIND_PATH(RCSSNET3D_INCLUDE_DIR rcssnet/addr.hpp
  HINTS ${SPARK_DIR}
  PATH_SUFFIXES simspark include/simspark)

SET(RCSSNET3D_NAMES rcssnet3D rcssnet3D_debug)
FIND_LIBRARY(RCSSNET3D_LIBRARY NAMES ${RCSSNET3D_NAMES}
  HINTS ${SPARK_DIR}
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
