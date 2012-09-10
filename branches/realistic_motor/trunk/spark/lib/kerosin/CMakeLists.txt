########### target ###############
set(kerosin_LIB_HDRS
    fontserver/font.h
    fontserver/fontserver.h
    fontserver/glyph.h
    imageserver/image.h
    imageserver/imageserver.h
    inputserver/inputdevice.h
    inputserver/inputserver.h
    inputserver/inputsystem.h
    inputserver/scancodemap.h
    inputserver/inputcontrol.h
    inputserver/inputitem.h
    inputserver/input.h
    kerosin.h
    kerosin_defines.h
    materialserver/material.h
    materialserver/materialserver.h
    sceneserver/axis.h
    materialserver/material2dtexture.h
    materialserver/materialexternal.h
    materialserver/materialsolid.h
    materialserver/materialexporter.h
    sceneserver/singlematnode.h
    sceneserver/light.h
    sceneserver/sphere.h
    sceneserver/box.h
    sceneserver/capsule.h
    sceneserver/cylinder.h
    sceneserver/staticmesh.h
    soundserver/soundeffect.h
    soundserver/soundmodule.h
    soundserver/soundobject.h
    soundserver/soundserver.h
    soundserver/soundstream.h
    soundserver/soundsystem.h
    textureserver/texture.h
    textureserver/texture2d.h
    textureserver/textureserver.h
    openglserver/openglwrapper.h
    openglserver/openglsystem.h
    openglserver/openglserver.h
    openglserver/glbase.h
    renderserver/baserenderserver.h
    renderserver/renderserver.h
    renderserver/rendernode.h
    renderserver/rendercontrol.h
    renderserver/customrender.h
)

set(kerosin_LIB_SRCS
    fontserver/font.cpp
    fontserver/fontserver.cpp
    fontserver/fontserver_c.cpp
    fontserver/glyph.cpp
    imageserver/image.cpp
    imageserver/imageserver.cpp
    imageserver/imageserver_c.cpp
    inputserver/input.cpp
    inputserver/inputdevice.cpp
    inputserver/inputdevice_c.cpp
    inputserver/inputserver.cpp
    inputserver/inputserver_c.cpp
    inputserver/inputsystem.cpp
    inputserver/inputsystem_c.cpp
    inputserver/scancodemap.cpp
    inputserver/inputcontrol.cpp
    inputserver/inputcontrol_c.cpp
    inputserver/inputitem.cpp
    inputserver/inputitem_c.cpp
    kerosin.cpp
    materialserver/material.cpp
    materialserver/material_c.cpp
    materialserver/material2dtexture.cpp
    materialserver/material2dtexture_c.cpp
    materialserver/materialexternal.cpp
    materialserver/materialexternal_c.cpp
    materialserver/materialsolid.cpp
    materialserver/materialsolid_c.cpp
    materialserver/materialserver.cpp
    materialserver/materialserver_c.cpp
    materialserver/materialexporter_c.cpp
    sceneserver/axis.cpp
    sceneserver/axis_c.cpp
    sceneserver/light.cpp
    sceneserver/light_c.cpp
    sceneserver/singlematnode.cpp
    sceneserver/singlematnode_c.cpp
    sceneserver/box.cpp
    sceneserver/box_c.cpp
    sceneserver/sphere.cpp
    sceneserver/sphere_c.cpp
    sceneserver/capsule.cpp
    sceneserver/capsule_c.cpp
    sceneserver/cylinder.cpp
    sceneserver/cylinder_c.cpp
    sceneserver/staticmesh.cpp
    sceneserver/staticmesh_c.cpp
    soundserver/soundserver.cpp
    soundserver/soundserver_c.cpp
    textureserver/texture.cpp
    textureserver/texture2d.cpp
    textureserver/textureserver.cpp
    textureserver/textureserver_c.cpp
    openglserver/openglserver.cpp
    openglserver/openglserver_c.cpp
    openglserver/openglsystem_c.cpp
    renderserver/baserenderserver.cpp
    renderserver/baserenderserver_c.cpp
    renderserver/renderserver.cpp
    renderserver/renderserver_c.cpp
    renderserver/rendernode.cpp
    renderserver/rendernode_c.cpp
    renderserver/rendercontrol.cpp
    renderserver/rendercontrol_c.cpp
    renderserver/customrender.cpp
    renderserver/customrender_c.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/lib ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_library(kerosin ${kerosin_LIB_SRCS} ${kerosin_LIB_HDRS})

target_link_libraries(kerosin ${FREETYPE_LIBRARIES} ${IL_LIBRARIES}
    ${OPENGL_LIBRARIES} ${kerosin_require_libs})

set_target_properties(kerosin PROPERTIES VERSION ${KEROSIN_VERSION}
    SOVERSION ${KEROSIN_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS kerosin DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/kerosin-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/kerosin-config)

########### install files ###############

install(FILES  kerosin.rb  	inputserver/german.scan.rb
        openglserver/glbase.h  DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/kerosin-config DESTINATION
        ${BINDIR} PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
        GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/kerosin
        FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
