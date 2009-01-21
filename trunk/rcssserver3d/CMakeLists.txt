cmake_minimum_required(VERSION 2.6)

project(rcssserver3d CXX C)
set(PACKAGE_VERSION "0.6")

########## check for headerfiles/libraries ##########
include(CheckIncludeFile) 
check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
find_package(Spark REQUIRED)
find_package(Freetype REQUIRED)

########## add extra flags ########## 
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Release or Debug build type")
add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_BINARY_DIR})
if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif (UNIX)

set(BINDIR "bin" CACHE PATH "The directory to install binaries into.")
set(LIBDIR "lib" CACHE PATH "The directory to install libraries into.")
set(DATADIR "share" CACHE PATH "The directory to install data files into.")
set(INCLUDEDIR "include" CACHE PATH "The directory to install includes into.")
set(RPATH_DIRECTORIES ${SPARK_LIB_DIR}
   "${CMAKE_INSTALL_PREFIX}/${LIBDIR}/${CMAKE_PROJECT_NAME}")
set(CMAKE_SHARED_MODULE_PREFIX "")

mark_as_advanced(BINDIR LIBDIR DATADIR INCLUDEDIR)
 
########## generate configuration file ##########
if (APPLE)
  # TODO: it probably doesn't work on Mac OSX. Please fix it!
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
elseif (UNIX)
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
else (APPLE)
  set(BUNDLE_PATH .)
endif (APPLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rcssserver3d_config.h.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/rcssserver3d_config.h)

########## add subdirectories ############
add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(plugin)
add_subdirectory(rcssagent3d)
add_subdirectory(rcssmonitor3d)
add_subdirectory(simspark)


########### install files ###############

install(FILES  AUTHORS ChangeLog COPYING NEWS README THANKS TODO 
       DESTINATION ${DATADIR}/doc/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_BINARY_DIR}/rcssserver3d_config.h 
       DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME})

########### CPack section #############
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
    /CVS/ /[.]svn/ /.*[.]o /autom4te[.]cache/ /[.].* /bootstrap /RELEASE
    /rpm.am /msi.am)
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})

include(CPack)

#original Makefile.am contents follow:

#DISTCLEANFILES = $(top_builddir)/macosX/SDLMain.o $(top_builddir)/macosX/libSDLmain.a
#EXTRA_DIST = macosX/fwcopy  macosX/mkapp  macosX/plugincopy  \
#            macosX/SDLMain.h  macosX/SDLMain.m \
#            windows/rcssserver3d_config.h windows/simspark.iss windows/spark.ico
