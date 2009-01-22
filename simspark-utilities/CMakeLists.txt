cmake_minimum_required(VERSION 2.6)

project(simspark-utilities CXX C)
set(PACKAGE_VERSION "0.1")

########## check for headerfiles/libraries ##########
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
find_package(Spark REQUIRED)
find_package(Boost REQUIRED)

########## add extra flags ########## 
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Release or Debug build type")
add_definitions(-DHAVE_CONFIG_H)
if (UNIX)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -W -Wall -Woverloaded-virtual")
  set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
endif (UNIX)

set(BINDIR "bin" CACHE PATH "The directory to install binaries into.")
set(LIBDIR "lib" CACHE PATH "The directory to install libraries into.")
set(DATADIR "share" CACHE PATH "The directory to install data files into.")
set(INCLUDEDIR "include" CACHE PATH "The directory to install includes into.")
set(RPATH_DIRECTORIES ${SPARK_LIB_DIR})
set(CMAKE_SHARED_MODULE_PREFIX "")

mark_as_advanced(BINDIR LIBDIR DATADIR INCLUDEDIR)
 
########## add subdirectories ############
add_subdirectory(data)
add_subdirectory(gendot)
add_subdirectory(monitorspark)
add_subdirectory(samplesim)
add_subdirectory(sampleagent)


########### install files ###############
install(FILES  AUTHORS ChangeLog COPYING README 
       DESTINATION ${DATADIR}/doc/${CMAKE_PROJECT_NAME})

########### CPack section #############
set(CPACK_SOURCE_GENERATOR TGZ)
set(CPACK_SOURCE_IGNORE_FILES /CVS/ /[.]svn/ /.*[.]o /[.].*)
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_SOURCE_PACKAGE_FILE_NAME
    ${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION})

include(CPack)
