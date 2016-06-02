include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/utility
    ${OPENGL_INCLUDE_DIR})

set(spark_gui_libs salt oxygen zeitgeist kerosin spark carbon rcssnet3D)

add_subdirectory(guigridlayout)
add_subdirectory(guisimplelayout)

