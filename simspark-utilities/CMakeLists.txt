cmake_minimum_required(VERSION 2.6)

project(simspark-utilities CXX C)
set(PACKAGE_VERSION "0.1")

########## check for headerfiles/libraries ##########
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_SOURCE_DIR}/../spark/cmake/)
find_package(Spark REQUIRED)
find_package(Boost REQUIRED)

find_package(ODE REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL REQUIRED)
find_package(DevIL REQUIRED)

if (WIN32)
 set(NET_LIBS WS2_32.lib)
endif (WIN32)
########## add extra flags ########## 
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-DHAVE_CONFIG_H)
include_directories(${Boost_INCLUDE_DIRS} ${ODE_INCLUDE_DIR} ${SPARK_INCLUDE_DIR}
  ${SALT_INCLUDE_DIR} ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR}
  ${KEROSIN_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated" CACHE
        STRING "Common C++ compiler flags" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual" CACHE
        STRING "C++ compiler flags for Debug builds" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE
        STRING "C++ compiler flags for Release builds" FORCE)
endif (UNIX)

set(BINDIR "bin" CACHE PATH "The directory to install binaries into.")
set(LIBDIR "lib" CACHE PATH "The directory to install libraries into.")
set(DATADIR "share" CACHE PATH "The directory to install data files into.")
set(INCLUDEDIR "include" CACHE PATH "The directory to install includes into.")
set(RPATH_DIRECTORIES ${SPARK_LIB_DIR})
set(CMAKE_SHARED_MODULE_PREFIX "")

mark_as_advanced(BINDIR LIBDIR DATADIR INCLUDEDIR)
 
if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_DYN_LINK)
    if(MSVC)
      add_definitions(/Gm /Zi /GL)
    endif(MSVC)
endif(WIN32)
########## add subdirectories ############
add_subdirectory(data)
add_subdirectory(gendot)
add_subdirectory(monitorspark)
add_subdirectory(samplesim)
add_subdirectory(sampleagent)


########### install files ###############
install(FILES  AUTHORS ChangeLog COPYING README 
       DESTINATION ${DATADIR}/doc/${CMAKE_PROJECT_NAME})

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
    /RELEASE /rpm.am /msi.am)
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_NSIS_HELP_LINK "TODO:simspark wiki link")
set(CPACK_NSIS_URL_INFO_ABOUT "http://simspark.sourceforge.net")
set(CPACK_NSIS_CONTACT "simspark-devel@lists.sourceforge.net")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\install_extra.nsi")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\uninstall_extra.nsi")
include(CPack)
