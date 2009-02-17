
########### next target ###############

set(fonttest_SRCS
   main.cpp
)

if(WIN32)
	include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${DevIL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/windows/include)
else(WIN32)
	include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
endif(WIN32)

add_executable(fonttest ${fonttest_SRCS})

target_link_libraries(fonttest salt zeitgeist oxygen kerosin)

