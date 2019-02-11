# - Find Carbon
# Find the native Carbon includes and libraries
#
#  CARBON_INCLUDE_DIR        - where to find carbon include files
#  CARBON_LIB_DIR            - where to find carbon libraries
#  CARBON_LIBRARIES          - list of libraries when using carbon (contains first found carbon lib, debug or release)
#  CARBON_LIBRARY_DEBUG      - carbon debug library
#  CARBON_LIBRARY_RELEASE    - carbon release library
#  CARBON_FOUND              - true if a carbon library was found
  
# Carbon section
IF (CARBON_INCLUDE_DIR)
  # Already in cache, be silent
  SET(CARBON_FIND_QUIETLY TRUE)
ENDIF (CARBON_INCLUDE_DIR)

SET(CARBON_DIR "C:/Program Files/simspark" "C:/Program Files (x86)/simspark" "C:/library/simspark")

FIND_PATH(CARBON_INCLUDE_DIR carbon/carbon.h
  HINTS ENV CARBON_DIR
  PATHS ${CARBON_DIR}
  PATH_SUFFIXES simspark include/simspark)

FIND_PATH(CARBON_ROOT_DIR share/carbon/setups/DefaultSetupGrid.xml
  HINTS ENV CARBON_DIR
  PATHS ${CARBON_DIR})  
  
FIND_PATH(CARBON_DATA_DIR setups/DefaultSetupGrid.xml
  HINTS ENV CARBON_DIR
  PATHS ${CARBON_DIR}
  PATH_SUFFIXES share/carbon)
  
SET(CARBON_NAMES carbon carbon_debug)
FIND_LIBRARY(CARBON_LIBRARY NAMES ${CARBON_NAMES}
  HINTS ENV CARBON_DIR
  PATHS ${CARBON_DIR}
  PATH_SUFFIXES simspark lib/simspark)
    
GET_FILENAME_COMPONENT(CARBON_LIB_DIR ${CARBON_LIBRARY} PATH)

# handle the QUIETLY and REQUIRED arguments and set CARBON_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CARBON DEFAULT_MSG CARBON_LIBRARY
         CARBON_INCLUDE_DIR)

IF(CARBON_FOUND)
  SET( CARBON_LIBRARIES ${CARBON_LIBRARY} )
  STRING(REPLACE "carbon." "carbon_debug." CARBON_LIBRARY_DEBUG ${CARBON_LIBRARY})
  STRING(REPLACE "carbon_debug." "carbon." CARBON_LIBRARY_RELEASE ${CARBON_LIBRARY})  
ELSE(CARBON_FOUND)
  SET( CARBON_LIBRARIES )
ENDIF(CARBON_FOUND)

MARK_AS_ADVANCED( CARBON_PLUGIN_LIB_DIR CARBON_LAYOUT_LIB_DIR CARBON_DIR CARBON_LIBRARY CARBON_LIBRARY_RELEASE CARBON_LIBRARY_DEBUG CARBON_INCLUDE_DIR )
