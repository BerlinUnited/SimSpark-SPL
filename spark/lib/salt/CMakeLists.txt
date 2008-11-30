set(salt_LIB_SRCS
  bounds.cpp
  fileclasses.cpp
  frustum.cpp
  matrix.cpp
  path.cpp
  plane.cpp
  sharedlibrary.cpp
)

########### target ###############

add_library(salt SHARED ${salt_LIB_SRCS})

target_link_libraries(salt libobject)

set_target_properties(salt PROPERTIES VERSION ${SALT_VERSION}
    SOVERSION ${SALT_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS salt DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/salt-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/salt-config)

########### install files ###############

install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/salt
          FILES_MATCHING PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/salt-config DESTINATION ${BINDIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

#original Makefile.am contents follow:

#if DEBUG
#libsalt_debug_la_CXXFLAGS = -O -g -W -Wall
#libsalt_la_CXXFLAGS = -O2
#
#libpkginclude_HEADERS = \
#bounds.h   \
#fileclasses.h  \
#gmath.h   \
#path.h   \
#random.h \
#salt.h \
#tvector.h \
#defines.h \
#frustum.h \
#matrix.h \
#plane.h \
#rect.h \
#sharedlibrary.h \
#vector.h
