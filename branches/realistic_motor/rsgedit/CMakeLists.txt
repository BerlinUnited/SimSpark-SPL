cmake_minimum_required(VERSION 2.6)

project(rsgedit CXX C)
set(PACKAGE_VERSION "0.1")

########## check for headerfiles/libraries ##########
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
find_package(Spark REQUIRED)
find_package(Boost REQUIRED thread COMPONENTS date_time)
find_package(ODE REQUIRED)
find_package(OpenGL REQUIRED)
set(wxWidgets_USE_LIBS  core gl adv)
find_package(wxWidgets REQUIRED)
include( ${wxWidgets_USE_FILE} )

########## add extra flags ########## 
if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-DHAVE_CONFIG_H )
include_directories(${CMAKE_BINARY_DIR})
if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated" CACHE
        STRING "Common C++ compiler flags" FORCE)
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual" CACHE
        STRING "C++ compiler flags for Debug builds" FORCE)
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE
        STRING "C++ compiler flags for Release builds" FORCE)
endif (UNIX)

if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DBOOST_ALL_DYN_LINK)
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
  # TODO: it probably doesn't work on Mac OSX. Please fix it!
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
elseif (UNIX)
  set(BUNDLE_PATH ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})
else (APPLE)
  set(BUNDLE_PATH .)
endif (APPLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rsgedit_config.h.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/rsgedit_config.h)

########## add subdirectories ############
add_subdirectory(wxutil)
add_subdirectory(src)
add_subdirectory(res)


########### install files ###############

install(FILES  doc/rsgedit.txt AUTHORS ChangeLog COPYING NEWS README TODO
        DESTINATION  ${DATADIR}/doc/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_BINARY_DIR}/rsgedit_config.h
        DESTINATION  ${INCLUDEDIR}/${CMAKE_PROJECT_NAME})

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

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_NSIS_HELP_LINK "TODO:simspark wiki link")
set(CPACK_NSIS_URL_INFO_ABOUT "http://simspark.sourceforge.net")
set(CPACK_NSIS_CONTACT "simspark-devel@lists.sourceforge.net")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\install_extra.nsi")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "  !include ${CMAKE_SOURCE_DIR}\\\\windows\\\\uninstall_extra.nsi")
include(CPack)
