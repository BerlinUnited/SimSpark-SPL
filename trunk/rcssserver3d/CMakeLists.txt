cmake_minimum_required(VERSION 2.6)

project(rcssserver3d CXX C)
set(PACKAGE_VERSION "0.6.6")

########## check for headerfiles/libraries ##########
include(CheckIncludeFile) 
check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)

if (WIN32)
  set(Boost_USE_STATIC_LIBS        ON)
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME    OFF)

  # this must be added by cmake itself, but it is not
  if ("${CMAKE_CXX_COMPILER}" MATCHES "mingw")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared")
  endif ("${CMAKE_CXX_COMPILER}" MATCHES "mingw")
endif (WIN32)

include(AdditionalSearchPaths)

find_package(Spark REQUIRED)
find_package(Freetype REQUIRED)
find_package(Boost REQUIRED system)
find_package(SDL REQUIRED)
find_package(DevIL REQUIRED)
find_package(ODE REQUIRED)
find_package(OpenGL REQUIRED)

########## add extra flags ########## 
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-DHAVE_CONFIG_H ${ODE_CFLAGS})
include_directories(${CMAKE_BINARY_DIR} ${Boost_INCLUDE_DIRS} ${ODE_INCLUDE_DIR}
  ${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR} ${ZEITGEIST_INCLUDE_DIR}
  ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

if (UNIX)
  set(CMAKE_CXX_FLAGS $ENV{CXXFLAGS} CACHE
        STRING "Common C++ compiler flags" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual" CACHE
        STRING "C++ compiler flags for Debug builds" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE
        STRING "C++ compiler flags for Release builds" FORCE)
endif (UNIX)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  include_directories(${CMAKE_SOURCE_DIR}/windows/include)
    if(MSVC)
      add_definitions(/Gm /Zi /GL)
    endif(MSVC)
endif(WIN32)

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
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
elseif (UNIX)
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
else (APPLE)
  set(BUNDLE_PATH ../share/${CMAKE_PROJECT_NAME})
endif (APPLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rcssserver3d_config.h.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/rcssserver3d_config.h)

########## add subdirectories ############
add_subdirectory(data)
add_subdirectory(doc)
add_subdirectory(plugin)
add_subdirectory(rcssagent3d)
add_subdirectory(rcssmonitor3d)
add_subdirectory(rcssserver3d)


########### install files ###############

install(FILES  AUTHORS ChangeLog COPYING NEWS README THANKS TODO 
       DESTINATION ${DATADIR}/doc/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_BINARY_DIR}/rcssserver3d_config.h 
       DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME})

########### uninstall support ############
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

########### CPack section #############
include(InstallRequiredSystemLibraries)
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
    /CVS/ /[.]svn/ /.*[.]o$ /.*[.]o/ /autom4te[.]cache/ /[.].* /bootstrap
    /RELEASE /rpm.am /msi.am)
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})

set(CPACK_STRIP_FILES TRUE)	
set(CPACK_PACKAGE_VENDOR "RoboCup Soccer Server 3D Maintenance Group")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RoboCup Soccer Simulation Server 3D")
#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/data/logos/simsparklogo.png")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_NSIS_HELP_LINK "TODO:simspark wiki link")
set(CPACK_NSIS_URL_INFO_ABOUT "http://simspark.sourceforge.net")
set(CPACK_NSIS_CONTACT "simspark-devel@lists.sourceforge.net")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\install_extra.nsi")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\uninstall_extra.nsi")

include(CPack)

#original Makefile.am contents follow:

#DISTCLEANFILES = $(top_builddir)/macosX/SDLMain.o $(top_builddir)/macosX/libSDLmain.a
#EXTRA_DIST = macosX/fwcopy  macosX/mkapp  macosX/plugincopy  \
#            macosX/SDLMain.h  macosX/SDLMain.m \
#            windows/rcssserver3d_config.h windows/simspark.iss windows/spark.ico
