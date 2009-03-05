
########### next target ###############

set(soundsystembass_LIB_SRCS
   export.cpp
   soundeffectbass.cpp
   soundeffectbass.h
   soundmodulebass.cpp
   soundmodulebass.h
   soundstreambass.cpp
   soundstreambass.h
   soundsystembass.cpp
   soundsystembass.h
   soundsystembass_c.cpp
)

add_library(soundsystembass MODULE ${soundsystembass_LIB_SRCS})

target_link_libraries(soundsystembass ${spark_libs})

set_target_properties(soundsystembass PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS soundsystembass DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
