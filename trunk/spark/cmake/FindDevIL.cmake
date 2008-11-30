# -Try to find DevIL (developer image) library
# formerly known as OpenIL, see
# http://openil.sourceforge.net
#
# Once run this will define: 
# 
# DevIL_FOUND
# DevIL_INCLUDE_DIR
# DevIL_LIBRARIES
#
# Jan Woetzel 12/2005. 
# 
# www.mip.informatik.uni-kiel.de/~jw
# --------------------------------

# base dirs:
SET(DevIL_POSSIBLE_ROOT_PATHS
  ${DevIL_ROOT_DIR}
  $ENV{DevIL_ROOT_DIR}
  ${DevIL_DIR}
  $ENV{DevIL_DIR}
  ${DEVIL_DIR}
  $ENV{DEVIL_DIR}
  ${DEVIL_HOME}
  $ENV{DEVIL_HOME}
  "$ENV{EXTERN_LIBS_DIR}/DevIL"
  $ENV{EXTRA_DIR}
  $ENV{EXTRA}
  $ENV{ProgramFiles}/DevIL
  /usr/local/
  /usr/
  /opt/net/gcc41/DevIL
  /opt/net/gcc33/DevIL
  )

# appended
SET(DevIL_POSSIBLE_INCDIR_SUFFIXES 
  include 
  DevIL/include )
SET(DevIL_POSSIBLE_LIBDIR_SUFFIXES 
  lib 
  lib64
  DevIL/lib )



FIND_PATH(DevIL_INCLUDE_DIR 
  NAMES IL/il.h
  PATHS         ${DevIL_POSSIBLE_ROOT_PATHS} 
  PATH_SUFFIXES ${DevIL_POSSIBLE_INCDIR_SUFFIXES} )
#MESSAGE("DBG DevIL_INCLUDE_DIR=${DevIL_INCLUDE_DIR}")  


FIND_LIBRARY(DevIL_LIBRARY_IL
  NAMES DevIL devil DevIL IL
  PATHS         ${DevIL_POSSIBLE_ROOT_PATHS}
  PATH_SUFFIXES ${DevIL_POSSIBLE_LIBDIR_SUFFIXES} )
#MESSAGE("DBG DevIL_LIBRARY_IL=${DevIL_LIBRARY_IL}")

FIND_LIBRARY(DevIL_LIBRARY_ILU
  NAMES ILU ilu Ilu 
  PATHS         ${DevIL_POSSIBLE_ROOT_PATHS}
  PATH_SUFFIXES ${DevIL_POSSIBLE_LIBDIR_SUFFIXES} )
#MESSAGE("DBG DevIL_LIBRARY_ILU=${DevIL_LIBRARY_ILU}")

FIND_LIBRARY(DevIL_LIBRARY_ILUT
  NAMES ILUT ilu Ilut
  PATHS         ${DevIL_POSSIBLE_ROOT_PATHS}
  PATH_SUFFIXES ${DevIL_POSSIBLE_LIBDIR_SUFFIXES} )
#MESSAGE("DBG DevIL_LIBRARY_ILUT=${DevIL_LIBRARY_ILUT}")



# --------------------------------

IF  (DevIL_INCLUDE_DIR)
  IF  (DevIL_LIBRARY_IL)
    IF  (DevIL_LIBRARY_ILU)# AND DevIL_LIBRARY_ILUT)

      SET(DevIL_FOUND TRUE)
      SET(DevIL_LIBRARIES 
	${DevIL_LIBRARY_IL}
	${DevIL_LIBRARY_ILU})
      IF (DevIL_LIBRARY_ILUT)
        SET(DevIL_LIBRARIES ${DevIL_LIBRARIES} ${DevIL_LIBRARY_ILUT})
      ENDIF (DevIL_LIBRARY_ILUT)

      # get the link directory for rpath to be used with LINK_DIRECTORIES: 	 
      GET_FILENAME_COMPONENT(DevIL_LINK_DIRECTORIES ${DevIL_LIBRARY_IL} PATH)  

    ENDIF(DevIL_LIBRARY_ILU)# AND DevIL_LIBRARY_ILUT)
  ENDIF(DevIL_LIBRARY_IL)
ENDIF  (DevIL_INCLUDE_DIR)


MARK_AS_ADVANCED(
  DevIL_INCLUDE_DIR
  DevIL_LIBRARY_IL
  DevIL_LIBRARY_ILU
  DevIL_LIBRARY_ILUT
  DevIL_LIBRARIES
  )


# ==========================================
IF(NOT DevIL_FOUND)
  # make FIND_PACKAGE friendly
  IF(NOT DevIL_FIND_QUIETLY)
    IF(DevIL_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "DevIL required, please specify it's location.")
    ELSE(DevIL_FIND_REQUIRED)
      MESSAGE(STATUS       "ERROR: DevIL was not found.")
    ENDIF(DevIL_FIND_REQUIRED)
  ENDIF(NOT DevIL_FIND_QUIETLY)
ENDIF(NOT DevIL_FOUND)

# backward compatibility
SET(DEVIL_FOUND       ${DevIL_FOUND})
SET(DEVIL_LIBRARIES   ${DevIL_LIBRARIES})
SET(DEVIL_INCLUDE_DIR ${DevIL_INCLUDE_DIR})
