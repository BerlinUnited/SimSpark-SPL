
########### next target ###############

set(inputsdl_LIB_SRCS
   export.cpp
   inputdevicesdl.h
   inputdevicesdl_c.cpp
   inputsystemsdl.cpp
   inputsystemsdl.h
   inputsystemsdl_c.cpp
   keyboardsdl.cpp
   keyboardsdl.h
   keyboardsdl_c.cpp
   mousesdl.cpp
   mousesdl.h
   mousesdl_c.cpp
   timersdl.cpp
   timersdl.h
   timersdl_c.cpp
)

include_directories(${SDL_INCLUDE_DIR})

add_library(inputsdl MODULE ${inputsdl_LIB_SRCS})

target_link_libraries(inputsdl)

set_target_properties(inputsdl PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS inputsdl DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

