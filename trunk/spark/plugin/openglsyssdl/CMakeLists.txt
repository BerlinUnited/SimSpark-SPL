
########### next target ###############

set(openglsyssdl_LIB_SRCS
   export.cpp
   openglsystemsdl.cpp
   openglsystemsdl.h
   openglsystemsdl_c.cpp
)

include_directories(${SDL_INCLUDE_DIR})

add_library(openglsyssdl MODULE ${openglsyssdl_LIB_SRCS})

target_link_libraries(openglsyssdl ${spark_libs})

set_target_properties(openglsyssdl PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS openglsyssdl DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
