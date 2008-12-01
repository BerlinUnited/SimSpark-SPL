cmake_minimum_required(VERSION 2.6)

project(simspark CXX C)

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

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
find_package(Ruby REQUIRED)
find_package(Freetype REQUIRED)
find_package(DevIL REQUIRED)
find_package(Boost REQUIRED thread COMPONENTS regex)
find_package(wxWidgets COMPONENTS gl base core)
find_package(ODE REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(FMOD)
set(HAVE_IL_IL_H 1)
set(HAVE_KEROSIN_KEROSIN_H 1)

########## check types/functions ##########
include(CheckFunctionExists)
check_function_exists(strupr HAVE_STRUPR)
include(CheckTypeSize)
set(CMAKE_REQUIRED_FLAGS "-include sys/socket.h")
check_type_size(socklen_t SOCKLEN_T)

########## add extra flags ########## 
add_definitions(-DHAVE_CONFIG_H)
include_directories(${CMAKE_BINARY_DIR})
set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH
    "The directory to install binaries into.")
set(LIBDIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH
    "The directory to install libraries into.")
set(DATADIR "${CMAKE_INSTALL_PREFIX}/share" CACHE PATH
    "The directory to install data files into.")
set(INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH
    "The directory to install includes into.")
set(CMAKE_SHARED_MODULE_PREFIX "")

mark_as_advanced(BINDIR LIBDIR DATADIR INCLUDEDIR)

########## library version numbers  ##########
set(SALT_SO_VERSION 3)
set(SALT_VERSION ${SALT_SO_VERSION}.1.3)
set(ZEITGEIST_SO_VERSION 3)
set(ZEITGEIST_VERSION ${ZEITGEIST_SO_VERSION}.1.0)
set(OXYGEN_SO_VERSION 5)
set(OXYGEN_VERSION ${OXYGEN_SO_VERSION}.3.2)
set(KEROSIN_SO_VERSION 1)
set(KEROSIN_VERSION ${KEROSIN_SO_VERSION}.1.0)
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
set(includedir ${INCLUDEDIR})
set(libdir ${LIBDIR})
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
#add_subdirectory(doc)
add_subdirectory(spark)
add_subdirectory(data)


########### install files ###############
install(FILES  AUTHORS ChangeLog COPYING NEWS README THANKS DESTINATION
    ${DATADIR}/doc/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/sparkconfig.h DESTINATION
    ${INCLUDEDIR}/${CMAKE_PROJECT_NAME})

########### CPack section #############
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES
    /CVS/ /[.]svn/ /.*[.]o /autom4te[.]cache/ /[.].*)
set(CPACK_PACKAGE_VERSION "0.1")
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})

include(CPack)

#original Makefile.am contents follow:

#### utility subdir has to be made first
#SUBDIRS = utility lib plugin test doc spark data
#
#pkginclude_HEADERS = \
#sparkconfig.h
#
#doc:	doc-recursive
#doc-recursive:	
#	cd doc && $(MAKE) $(AM_MAKEFLAGS) doc
#
#CLEANFILES = 
#
#DISTCLEANFILES = $(top_builddir)/macosX/SDLMain.o $(top_builddir)/macosX/libSDLmain.a
#EXTRA_DIST = macosX  \
#            windows/sparkconfig.h windows/simspark.iss windows/spark.ico
#dist_doc_DATA = AUTHORS ChangeLog COPYING NEWS README THANKS
#
#EXTRA_SRC_DISTS =
#EXTRA_BIN_DISTS =
#include $(top_srcdir)/config.aux/dist.am
#
#UPLOAD_BIN =
#UPLOAD_SRC = upload-gzip upload-bzip2 upload-zip
#UPLOAD_TARGETS = \
#{gzip=>@PACKAGE@-@VERSION@.tar.gz} \
#{bzip2=>@PACKAGE@-@VERSION@.tar.bz2} \
#{zip=>@PACKAGE@-@VERSION@.zip}
#include $(top_srcdir)/config.aux/sf.am
#
#
#include $(top_srcdir)/config.aux/cvs.am
#
##MSI_SETUP_FILE=Setup/Release/@PACKAGE_NAME@Setup.msi
##include $(top_srcdir)/config.aux/msi.am
#
##include $(top_srcdir)/config.aux/rpm.am
#
