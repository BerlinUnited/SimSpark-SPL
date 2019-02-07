########### target files ###############
set(setupframe_QOBJECT_HEADERS
	setupframe.h
)

set(setupframe_HEADERS
	export.h
)

set(setupframe_SOURCES
	setupframe.cpp
)

set(setupframe_FORMS
	setupframe.ui
)

#set(setupframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(setupframe_QOBJECT_HEADERS_MOC ${setupframe_QOBJECT_HEADERS})
QT4_WRAP_UI(setupframe_FORMS_HEADERS ${setupframe_FORMS})
#QT4_ADD_RESOURCES(setupframe_RESOURCES_RCC ${setupframe_RESOURCES})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/carbon)

add_library(
  setupframe MODULE 
  ${setupframe_SOURCES} 
  ${setupframe_QOBJECT_HEADERS}
  ${setupframe_QOBJECT_HEADERS_MOC}
  ${setupframe_HEADERS}
  ${setupframe_FORMS_HEADERS})
  #${setupframe_RESOURCES_RCC})

target_link_libraries(
  setupframe
  ${OPENGL_LIBRARIES} 
  ${QT_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(setupframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS setupframe DESTINATION ${CARBON_PLUGIN_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_PLUGIN_INCLUDE_DIR}/setupframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
