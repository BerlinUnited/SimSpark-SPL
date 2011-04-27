# find ODE (Open Dynamics Engine) includes and library
#
# ODE_INCLUDE_DIR - where the directory containing the ODE headers can be
#                   found
# ODE_LIBRARY     - full path to the ODE library
# ODE_CFLAGS      - additional compiler flags for ODE
# ODE_FOUND       - TRUE if ODE was found

IF (NOT ODE_FOUND)

  FIND_PROGRAM(ODE_CONFIG NAMES ${ODE_CONFIG_EXEC} ode-config)
  IF(ODE_CONFIG)
      # Use the newer EXECUTE_PROCESS command if it is available.
      IF(COMMAND EXECUTE_PROCESS)
        EXECUTE_PROCESS(
          COMMAND ${ODE_CONFIG} --cflags
          OUTPUT_VARIABLE ODE_CONFIG_CFLAGS
          OUTPUT_STRIP_TRAILING_WHITESPACE
          RESULT_VARIABLE ODE_CONFIG_RESULT
          )
        EXECUTE_PROCESS(
          COMMAND ${ODE_CONFIG} --prefix
          OUTPUT_VARIABLE ODE_CONFIG_PREFIX
          OUTPUT_STRIP_TRAILING_WHITESPACE
          RESULT_VARIABLE ODE_CONFIG_RESULT
          )
        EXECUTE_PROCESS(
          COMMAND ${ODE_CONFIG} --libs
          OUTPUT_VARIABLE ODE_CONFIG_LIBS
          OUTPUT_STRIP_TRAILING_WHITESPACE
          RESULT_VARIABLE ODE_CONFIG_RESULT
          )
      ELSE(COMMAND EXECUTE_PROCESS)
        EXEC_PROGRAM(${ODE_CONFIG} ARGS "--cflags"
          OUTPUT_VARIABLE ODE_CONFIG_CFLAGS
          RETURN_VALUE ODE_CONFIG_RESULT
          )
        EXEC_PROGRAM(${ODE_CONFIG} ARGS "--prefix"
          OUTPUT_VARIABLE ODE_CONFIG_PREFIX
          RETURN_VALUE ODE_CONFIG_RESULT
          )
        EXEC_PROGRAM(${ODE_CONFIG} ARGS "--libs"
          OUTPUT_VARIABLE ODE_CONFIG_LIBS
          RETURN_VALUE ODE_CONFIG_RESULT
          )
      ENDIF(COMMAND EXECUTE_PROCESS)

      # Parse the include flags.
      IF("${ODE_CONFIG_RESULT}" MATCHES "^0$")
        # Convert the compile flags to a CMake list.
        STRING(REGEX REPLACE " +" ";"
          ODE_CONFIG_CFLAGS "${ODE_CONFIG_CFLAGS}")
    
        # Look for -D options.
        SET(ODE_EXTRA_CFLAGS)
        FOREACH(flag ${ODE_CONFIG_CFLAGS})
          IF("${flag}" MATCHES "^-D")
            SET(ODE_EXTRA_CFLAGS ${ODE_EXTRA_CFLAGS} "${flag}")
          ENDIF("${flag}" MATCHES "^-D")
        ENDFOREACH(flag)
        
        SET(ODE_EXTRA_LDFLAGS ${ODE_CONFIG_LIBS})
      ELSE("${ODE_CONFIG_RESULT}" MATCHES "^0$")
        MESSAGE("Error running ${ODE_CONFIG}: [${ODE_CONFIG_RESULT}]")
      ENDIF("${ODE_CONFIG_RESULT}" MATCHES "^0$")

  ENDIF(ODE_CONFIG)
  
  FIND_PATH(ODE_INCLUDE_DIR ode/ode.h
    ${ODE_CONFIG_PREFIX}/include
    /usr/include
    /usr/local/include
    $ENV{ODE_HOME}/include
    $ENV{INCLUDE}
    C:/library/ode/include
    "C:/Program Files/ode/include"
    "C:/Program Files (x86)/ode/include"
    C:/ode/include
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(ODE_LIBRARY
    NAMES ode ode_double ode_single
    PATHS
    ${ODE_CONFIG_PREFIX}/lib
    /usr/lib
    /usr/lib64
    /usr/local/lib
    $ENV{ODE_HOME}/lib
    C:/library/ode/lib/
    "C:/Program Files/ode/lib/"
    "C:/Program Files (x86)/ode/lib/"
    C:/ode/lib/
    PATH_SUFFIXES
      releaselib
      ReleaseDoubleDLL ReleaseDoubleLib 
      ReleaseSingleDLL ReleaseSingleLib
    NO_DEFAULT_PATH
  )

  IF (WIN32)
    IF("${ODE_LIBRARY}" MATCHES ".*double.*")
      SET(ODE_EXTRA_CFLAGS "-DdDOUBLE")
    ELSE("${ODE_LIBRARY}" MATCHES ".*double.*")
      SET(ODE_EXTRA_CFLAGS "-DdSINGLE")
    ENDIF("${ODE_LIBRARY}" MATCHES ".*double.*")
  ENDIF (WIN32)

  IF (ODE_EXTRA_CFLAGS)
    SET(ODE_CFLAGS ${ODE_EXTRA_CFLAGS} CACHE STRING "Additional ODE flags")
    MESSAGE(STATUS "Found additional flags for ODE: ${ODE_CFLAGS}")
  ELSE (ODE_EXTRA_CFLAGS)
    SET(ODE_CFLAGS CACHE STRING "Additional ODE flags")
  ENDIF (ODE_EXTRA_CFLAGS)

  IF (ODE_EXTRA_LDFLAGS)
    SET(ODE_LDFLAGS ${ODE_EXTRA_LDFLAGS} CACHE STRING "Additional ODE linker flags")
    MESSAGE(STATUS "Found additional linker flags for ODE: ${ODE_LDFLAGS}")
  ELSE (ODE_EXTRA_LDFLAGS)
    SET(ODE_CFLAGS CACHE STRING "Additional ODE linker flags")
  ENDIF (ODE_EXTRA_LDFLAGS)

  IF(ODE_INCLUDE_DIR)
    MESSAGE(STATUS "Found ODE include dir: ${ODE_INCLUDE_DIR}")
  ELSE(ODE_INCLUDE_DIR)
    MESSAGE(STATUS "Could NOT find ODE headers.")
  ENDIF(ODE_INCLUDE_DIR)

  IF(ODE_LIBRARY)
    MESSAGE(STATUS "Found ODE library: ${ODE_LIBRARY}")
  ELSE(ODE_LIBRARY)
    MESSAGE(STATUS "Could NOT find ODE library.")
  ENDIF(ODE_LIBRARY)

  IF(ODE_INCLUDE_DIR AND ODE_LIBRARY)
     SET(ODE_FOUND TRUE CACHE STRING "Whether ODE was found or not")
   ELSE(ODE_INCLUDE_DIR AND ODE_LIBRARY)
     SET(ODE_FOUND FALSE)
     IF(ODE_FIND_REQUIRED)
       MESSAGE(FATAL_ERROR "Could not find ODE. Please install ODE (http://www.ode.org)")
     ENDIF(ODE_FIND_REQUIRED)
   ENDIF(ODE_INCLUDE_DIR AND ODE_LIBRARY)
ENDIF (NOT ODE_FOUND)

