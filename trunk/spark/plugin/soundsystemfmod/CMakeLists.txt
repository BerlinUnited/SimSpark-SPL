
########### next target ###############

set(soundsystemfmod_LIB_SRCS
   export.cpp
   soundeffectfmod.cpp
   soundeffectfmod.h
   soundmodulefmod.cpp
   soundmodulefmod.h
   soundstreamfmod.cpp
   soundstreamfmod.h
   soundsystemfmod.cpp
   soundsystemfmod.h
   soundsystemfmod_c.cpp
)

if (FMOD_FOUND)
  include_directories(${FMOD_INCLUDE_DIRS})
  link_directories(${FMOD_LIBRARY_DIRS})

  add_library(soundsystemfmod MODULE ${soundsystemfmod_LIB_SRCS})

  target_link_libraries(soundsystemfmod ${FMOD_LIBRARIES} ${spark_libs})

  if (NOT APPLE)
     set_target_properties(soundsystemfmod PROPERTIES VERSION 0.0.0 SOVERSION 0)
  endif (NOT APPLE)

  install(TARGETS soundsystemfmod DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
endif (FMOD_FOUND)
