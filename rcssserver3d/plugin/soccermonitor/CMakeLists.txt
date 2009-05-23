########### next target ###############
set(soccermonitor_LIB_HDRS
	soccermonitor.h
	soccerrender.h
	soccerinput.h
    soccerinputlogplayer.h
)

set(soccermonitor_LIB_SRCS
   export.cpp
   soccermonitor.cpp
   soccermonitor_c.cpp
   soccerrender.cpp
   soccerrender_c.cpp
   soccerinput.cpp
   soccerinput_c.cpp
   soccerinputlogplayer.cpp
   soccerinputlogplayer_c.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/plugin ${SPARK_INCLUDE_DIR} 
  ${SALT_INCLUDE_DIR} ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR}
  ${KEROSIN_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${DevIL_INCLUDE_DIR})

add_library(soccermonitor MODULE ${soccermonitor_LIB_SRCS}
   ${soccermonitor_LIB_HDRS})

target_link_libraries(soccermonitor ${SALT_LIBRARIES} ${OXYGEN_LIBRARIES}
   	${ZEITGEIST_LIBRARIES} ${KEROSIN_LIBRARIES})

if (NOT APPLE)
   set_target_properties(soccermonitor PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS soccermonitor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})


########### install files ###############

install(FILES  soccermonitor.h soccerrender.h soccerinput.h
         soccerinputlogplayer.h
          DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/soccermonitor)
