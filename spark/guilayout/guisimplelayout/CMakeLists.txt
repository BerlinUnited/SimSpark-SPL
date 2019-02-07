########### target files ###############
set(guisimplelayout_QOBJECT_HEADERS
	guisimplelayout.h
)

set(guisimplelayout_HEADERS
	export.h
)

set(guisimplelayout_SOURCES
	guisimplelayout.cpp
)

set(guisimplelayout_FORMS
	guisimplelayout.ui
)

#set(guisimplelayout_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(guisimplelayout_QOBJECT_HEADERS_MOC ${guisimplelayout_QOBJECT_HEADERS})
QT4_WRAP_UI(guisimplelayout_FORMS_HEADERS ${guisimplelayout_FORMS})
#QT4_ADD_RESOURCES(guisimplelayout_RESOURCES_RCC ${guisimplelayout_RESOURCES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

add_library(
  guisimplelayout MODULE 
  ${guisimplelayout_SOURCES}
  ${guisimplelayout_QOBJECT_HEADERS}  
  ${guisimplelayout_QOBJECT_HEADERS_MOC}
  ${guisimplelayout_HEADERS}
  ${guisimplelayout_FORMS_HEADERS})
  #${guisimplelayout_RESOURCES_RCC})

target_link_libraries(
  guisimplelayout
  ${OPENGL_LIBRARIES} 
  ${QT_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(guisimplelayout PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS guisimplelayout DESTINATION ${CARBON_LAYOUT_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_LAYOUT_INCLUDE_DIR}/guisimplelayout
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
