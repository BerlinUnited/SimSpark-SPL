
########### next target ###############

set(fonttest_SRCS
   main.cpp
)

include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})

add_executable(fonttest ${fonttest_SRCS})

target_link_libraries(fonttest salt zeitgeist oxygen kerosin)

