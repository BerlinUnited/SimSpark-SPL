cmake_minimum_required(VERSION 2.6)

project(simspark CXX C)
set(PACKAGE_VERSION "0.1.2")

########## check for headerfiles/libraries ##########
include(CheckIncludeFile) 
check_include_file("netdb.h" HAVE_NETDB_H)
check_include_file("resolv.h" HAVE_RESOLV_H)
check_include_file("sys/socket.h" HAVE_SYS_SOCKET_H)
check_include_file("netinet/in.h" HAVE_NETINET_IN_H)
check_include_file("Winsock2.h" HAVE_WINSOCK2_H)
check_include_file("arpa/nameser.h" HAVE_ARPA_NAMESER_H)
check_include_file("arpa/inet.h" HAVE_ARPA_INET_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("float.h" HAVE_FLOAT_H)
check_include_file("limits.h" HAVE_LIMITS_H)
check_include_file("malloc.h" HAVE_MALLOC_H)
check_include_file("memory.h" HAVE_MEMORY_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("execinfo.h" HAVE_EXECINFO_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("poll.h" HAVE_POLL_H)

check_include_file("CoreFoundation/CoreFoundation.h"
	           HAVE_COREFOUNDATION_COREFOUNDATION_H)

if (NOT USE_COREFOUNDATION STREQUAL "OFF")
   if (HAVE_COREFOUNDATION_COREFOUNDATION_H)
      set(USE_COREFOUNDATION ON)		
      message("USE_COREFOUNDATION ON")		
   else (HAVE_COREFOUNDATION_COREFOUNDATION_H)
      set(USE_COREFOUNDATION OFF)		
      message("USE_COREFOUNDATION OFF")		
   endif (HAVE_COREFOUNDATION_COREFOUNDATION_H)
endif (NOT USE_COREFOUNDATION STREQUAL "OFF")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
set(ENV{SDLDIR} $ENV{SDLDIR} C:/library/SDL/ "C:/Program Files/SDL/" C:/SDL/)

find_package(Ruby REQUIRED)
find_package(Freetype REQUIRED)
find_package(DevIL REQUIRED)
find_package(Boost REQUIRED thread COMPONENTS regex)
find_package(wxWidgets COMPONENTS gl base core)
find_package(ODE REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(FMOD)
find_package(ZLIB)
set(HAVE_IL_IL_H 1)
set(HAVE_KEROSIN_KEROSIN_H 1)

########## check types/functions ##########
include(CheckFunctionExists)
include(CheckTypeSize)
check_function_exists(strupr HAVE_STRUPR)
set(CMAKE_REQUIRED_FLAGS "-include sys/socket.h")
check_type_size(socklen_t SOCKLEN_T)
set(CMAKE_REQUIRED_FLAGS "")

########## check for required libraries ##########
include(CheckLibraryExists)
check_function_exists(socket FUNC_SOCKET_EXISTS)
if (NOT FUNC_SOCKET_EXISTS)
  check_library_exists(socket socket "" SOCKET_LIB_REQ)
  if (SOCKET_LIB_REQ)
    set(NET_LIBS ${NET_LIBS} socket)
  endif (SOCKET_LIB_REQ)
endif (NOT FUNC_SOCKET_EXISTS)

check_function_exists(gethostbyname FUNC_GETHOSTBYNAME_EXISTS)
if (NOT FUNC_GETHOSTBYNAME_EXISTS)
  check_library_exists(nsl gethostbyname "" NSL_LIB_REQ)
  if (NSL_LIB_REQ)
    set(NET_LIBS ${NET_LIBS} nsl)
  endif (NSL_LIB_REQ)
endif (NOT FUNC_GETHOSTBYNAME_EXISTS)

check_function_exists(hstrerror FUNC_HSTRERROR_EXISTS)
if (NOT FUNC_HSTRERROR_EXISTS)
  check_library_exists(resolv hstrerror "" RESOLV_LIB_REQ)
  if (RESOLV_LIB_REQ)
    set(NET_LIBS ${NET_LIBS} resolv)
  endif (RESOLV_LIB_REQ)
endif (NOT FUNC_HSTRERROR_EXISTS)

if (WIN32)
 set(NET_LIBS ${NET_LIBS} WS2_32.lib)
endif (WIN32)
 
########## add extra flags ########## 
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_BINARY_DIR})
link_directories(${Boost_LIBRARY_DIRS})

if (UNIX)
  set(CMAKE_CXX_FLAGS $ENV{CXXFLAGS} CACHE
        STRING "Common C++ compiler flags" FORCE)
  set(CMAKE_C_FLAGS $ENV{CFLAGS} CACHE
        STRING "Common C compiler flags" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual" CACHE
        STRING "C++ compiler flags for Debug builds" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE
        STRING "C++ compiler flags for Release builds" FORCE)
endif (UNIX)

set(BINDIR "bin" CACHE PATH "The directory to install binaries into.")
set(LIBDIR "lib" CACHE PATH "The directory to install libraries into.")
set(DATADIR "share" CACHE PATH "The directory to install data files into.")
set(INCLUDEDIR "include" CACHE PATH "The directory to install includes into.")
set(CMAKE_SHARED_MODULE_PREFIX "")

mark_as_advanced(BINDIR LIBDIR DATADIR INCLUDEDIR)
set(BUILD_SHARED_LIBS TRUE)
if (BUILD_SHARED_LIBS)
  set(SHARED_LIB_COMPILE 1)
endif(BUILD_SHARED_LIBS)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_DYN_LINK)
    include_directories(${Boost_INCLUDE_DIRS} ${ODE_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/windows/include)
    if(MSVC)
      add_definitions(/Gm /Zi /GL)
    endif(MSVC)
endif(WIN32)

########## library version numbers  ##########
set(SALT_SO_VERSION 0)
set(SALT_VERSION ${SALT_SO_VERSION}.3.2)
set(ZEITGEIST_SO_VERSION 3)
set(ZEITGEIST_VERSION ${ZEITGEIST_SO_VERSION}.0.2)
set(OXYGEN_SO_VERSION 3)
set(OXYGEN_VERSION ${OXYGEN_SO_VERSION}.2.4)
set(KEROSIN_SO_VERSION 1)
set(KEROSIN_VERSION ${KEROSIN_SO_VERSION}.0.2)
set(SPARK_SO_VERSION 0)
set(SPARK_VERSION ${SPARK_SO_VERSION}.0.0)
set(RCSSNET_SO_VERSION 0)
set(RCSSNET_VERSION ${RCSSNET_SO_VERSION}.0.0)

########## additional variables for -config scripts  ##########
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
set(spark_version ${SPARK_VERSION})
set(salt_version ${SALT_VERSION})
set(zeitgeist_version ${ZEITGEIST_VERSION})
set(oxygen_version ${OXYGEN_VERSION})
set(kerosin_version ${KEROSIN_VERSION})
set(includedir ${CMAKE_INSTALL_PREFIX}/${INCLUDEDIR})
set(libdir ${CMAKE_INSTALL_PREFIX}/${LIBDIR})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(debug "true")
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(debug "false")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")


########## generate configuration file ##########
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sparkconfig.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/sparkconfig.h)

########## add subdirectories ############
add_subdirectory(utility)
add_subdirectory(lib)
add_subdirectory(plugin)
add_subdirectory(test)
add_subdirectory(doc)
add_subdirectory(spark)
add_subdirectory(data)


########### install files ###############
install(FILES  AUTHORS ChangeLog COPYING NEWS README THANKS DESTINATION
    ${DATADIR}/doc/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/sparkconfig.h DESTINATION
    ${INCLUDEDIR}/${CMAKE_PROJECT_NAME})

########### uninstall support ############
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

########### CPack section #############
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
    /CVS/ /[.]svn/ /.*[.]o$ /.*[.]o/ /autom4te[.]cache/ /[.].* /bootstrap 
    /RELEASE /styles/ /papers/ /rpm.am /msi.am /codingstyle.txt)
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})
	
set(CPACK_PACKAGE_VENDOR "RoboCup Soccer Server 3D Maintenance Group")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A generic physical simulation system")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "simspark")
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
#
#UPLOAD_BIN =
#UPLOAD_SRC = upload-gzip upload-bzip2 upload-zip
#UPLOAD_TARGETS = \
#{gzip=>@PACKAGE@-@VERSION@.tar.gz} \
#{bzip2=>@PACKAGE@-@VERSION@.tar.bz2} \
#{zip=>@PACKAGE@-@VERSION@.zip}
#include $(top_srcdir)/config.aux/sf.am
#
#include $(top_srcdir)/config.aux/cvs.am
