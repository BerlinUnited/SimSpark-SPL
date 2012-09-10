
########### next target ###############

set(zeitgeisttest_SRCS
   main.cpp
)

add_executable(zeitgeisttest ${zeitgeisttest_SRCS})

target_link_libraries(zeitgeisttest salt zeitgeist)
