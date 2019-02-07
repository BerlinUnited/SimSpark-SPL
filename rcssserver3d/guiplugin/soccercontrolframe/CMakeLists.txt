########### target files ###############
set(soccercontrolframe_QOBJECT_HEADERS
	soccercontrolframe.h
)

set(soccercontrolframe_HEADERS
	soccercontrolframe_soccercommands.h
	export.h
	
	${PLUGIN_INCLUDE_DIR}/soccer/agentstate/agentstate.h
	${PLUGIN_INCLUDE_DIR}/soccer/ballstateaspect/ballstateaspect.h
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateaspect.h
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateitem.h
	${PLUGIN_INCLUDE_DIR}/soccer/objectstate/objectstate.h
	${PLUGIN_INCLUDE_DIR}/soccer/soccerbase/soccerbase.h
	${PLUGIN_INCLUDE_DIR}/soccer/soccercontrolaspect/soccercontrolaspect.h
	${PLUGIN_INCLUDE_DIR}/soccer/soccernode/soccernode.h
	${PLUGIN_INCLUDE_DIR}/soccer/soccertypes.h
)

set(soccercontrolframe_SOURCES
	soccercontrolframe.cpp
	soccercontrolframe_soccercommands.cpp
	
	${PLUGIN_INCLUDE_DIR}/soccer/agentstate/agentstate.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/agentstate/agentstate_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/ballstateaspect/ballstateaspect.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/ballstateaspect/ballstateaspect_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateaspect.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateaspect_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateitem.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/gamestateaspect/gamestateitem_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/objectstate/objectstate.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/objectstate/objectstate_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/soccerbase/soccerbase.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/soccercontrolaspect/soccercontrolaspect.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/soccercontrolaspect/soccercontrolaspect_c.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/soccernode/soccernode.cpp
	${PLUGIN_INCLUDE_DIR}/soccer/soccernode/soccernode_c.cpp
)

set(soccercontrolframe_FORMS
	soccercontrolframe.ui
)

#set(soccercontrolframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(soccercontrolframe_QOBJECT_HEADERS_MOC ${soccercontrolframe_QOBJECT_HEADERS})
QT4_WRAP_UI(soccercontrolframe_FORMS_HEADERS ${soccercontrolframe_FORMS})
#QT4_ADD_RESOURCES(soccercontrolframe_RESOURCES_RCC ${soccercontrolframe_RESOURCES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CARBON_INCLUDE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PLUGIN_INCLUDE_DIR}/soccer
  ${PLUGIN_INCLUDE_DIR})

add_library(
  soccercontrolframe MODULE 
  ${soccercontrolframe_SOURCES} 
  ${soccercontrolframe_QOBJECT_HEADERS}
  ${soccercontrolframe_QOBJECT_HEADERS_MOC}
  ${soccercontrolframe_HEADERS}
  ${soccercontrolframe_FORMS_HEADERS})
  #${soccercontrolframe_RESOURCES_RCC})

set(soccercontrolframe_LIBS ${OPENGL_LIBRARIES} ${QT_LIBRARIES})
set(soccercontrolframe_DEBUG_LIBS ${spark_gui_debug_libs})
set(soccercontrolframe_RELEASE_LIBS ${spark_gui_release_libs})

target_link_libraries(soccercontrolframe ${soccercontrolframe_LIBS})
FOREACH(LIB ${soccercontrolframe_DEBUG_LIBS})
  target_link_libraries(soccercontrolframe debug ${LIB})
ENDFOREACH(LIB)
FOREACH(LIB ${soccercontrolframe_RELEASE_LIBS})
  target_link_libraries(soccercontrolframe optimized ${LIB})
ENDFOREACH(LIB) 

if (NOT APPLE)
   set_target_properties(soccercontrolframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS soccercontrolframe DESTINATION ${guipluginlibdir})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${GUI_PLUGIN_INCLUDE_DIR}/soccercontrolframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
