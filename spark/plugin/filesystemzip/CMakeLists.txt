
########### next target ###############

set(filesystemzip_LIB_SRCS
   export.cpp
   filesystemzip.cpp
   filesystemzip.h
   filesystemzip_c.cpp
)

if (Boost_REGEX_FOUND)
  add_library(filesystemzip MODULE ${filesystemzip_LIB_SRCS})

	if(WIN32)
		include_directories(${ZLIB_INCLUDE_DIR})
	endif(WIN32)

  target_link_libraries(filesystemzip ${Boost_REGEX_LIBRARY})

  set_target_properties(filesystemzip PROPERTIES VERSION 0.0.0 SOVERSION 0)
  install(TARGETS filesystemzip DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
endif (Boost_REGEX_FOUND)
