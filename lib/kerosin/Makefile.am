if DEBUG
pkglib_LTLIBRARIES = libkerosin_debug.la
libkerosin_debug_la_SOURCES = $(sources)
libkerosin_debug_la_CXXFLAGS = -O -g -W -Wall
libkerosin_debug_la_LIBADD = @FREETYPE_LIBADD@
else
pkglib_LTLIBRARIES = libkerosin.la
libkerosin_la_SOURCES = $(sources)
libkerosin_la_CXXFLAGS = -O2
libkerosin_la_LIBADD = @FREETYPE_LIBADD@
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/kerosin

BUILT_SOURCES = openglserver/glextensionreg.cpp openglserver/glextensionreg.h
CLEANFILES = openglserver/glextensionreg.cpp openglserver/glextensionreg.h
nodist_libkerosin_la_SOURCES = openglserver/glextensionreg.cpp
nobase_nodist_libpkginclude_HEADERS = openglserver/glextensionreg.h

openglserver/glextensionreg.cpp: ${top_srcdir}/utility/glextgen/glextensionreg.cpp
	@echo "Copying generated glextension sources"
	cp ${top_srcdir}/utility/glextgen/glextensionreg.cpp $@

openglserver/glextensionreg.h: ${top_srcdir}/utility/glextgen/glextensionreg.h
	cp ${top_srcdir}/utility/glextgen/glextensionreg.h $@

${top_srcdir}/utility/glextgen/glextensionreg.cpp ${top_srcdir}/utility/glextgen/glextensionreg.h:
	cd ${top_srcdir}/utility/glextgen/ && $(MAKE) $(AM_MAKEFLAGS) all

sources = \
agentaspect/agentaspect.cpp \
agentaspect/agentaspect_c.cpp \
agentaspect/effector.cpp \
agentaspect/effector_c.cpp \
agentaspect/forceeffector.cpp \
agentaspect/forceeffector_c.cpp \
agentaspect/perceptor.cpp \
agentaspect/perceptor_c.cpp \
agentaspect/visionperceptor.cpp \
agentaspect/visionperceptor_c.cpp \
controlaspect/controlaspect.cpp \
controlaspect/controlaspect_c.cpp \
fontserver/font.cpp \
fontserver/fontserver.cpp \
fontserver/fontserver_c.cpp \
fontserver/glyph.cpp \
imageserver/image.cpp \
imageserver/imageserver.cpp \
imageserver/imageserver_c.cpp \
inputserver/inputdevice.cpp \
inputserver/inputserver.cpp \
inputserver/inputserver_c.cpp \
inputserver/inputsystem.cpp \
inputserver/scancodemap.cpp \
kerosin.cpp \
materialserver/material.cpp \
materialserver/materialserver.cpp \
materialserver/materialserver_c.cpp \
sceneserver/axis.cpp \
sceneserver/axis_c.cpp \
sceneserver/basenode.cpp \
sceneserver/basenode_c.cpp \
sceneserver/camera.cpp \
sceneserver/camera_c.cpp \
sceneserver/helper/NVMeshMender.cpp \
sceneserver/indexbuffer.cpp \
sceneserver/light.cpp \
sceneserver/light_c.cpp \
sceneserver/scene.cpp \
sceneserver/scene_c.cpp \
sceneserver/sceneserver.cpp \
sceneserver/sceneserver_c.cpp \
sceneserver/sphere.cpp \
sceneserver/sphere_c.cpp \
sceneserver/staticmesh.cpp \
sceneserver/staticmesh_c.cpp \
sceneserver/transform.cpp \
sceneserver/transform_c.cpp \
soundserver/soundserver.cpp \
soundserver/soundserver_c.cpp \
textureserver/texture.cpp \
textureserver/texture2d.cpp \
textureserver/textureserver.cpp \
textureserver/textureserver_c.cpp \
openglserver/openglserver.cpp \
openglserver/openglserver_c.cpp 

#physicsserver/body.cpp \
#physicsserver/body_c.cpp \
#physicsserver/collider.cpp \
#physicsserver/collider_c.cpp \
#physicsserver/odeobject.cpp \
#physicsserver/odeobject_c.cpp \
#physicsserver/physicsserver.cpp \
#physicsserver/physicsserver_c.cpp \
#physicsserver/planecollider.cpp \
#physicsserver/planecollider_c.cpp \
#physicsserver/space.cpp \
#physicsserver/space_c.cpp \
#physicsserver/spherecollider.cpp \
#physicsserver/spherecollider_c.cpp \
#physicsserver/world.cpp \
#physicsserver/world_c.cpp 

#sceneserver/fpscontroller.cpp \
#sceneserver/fpscontroller_c.cpp 

nobase_libpkginclude_HEADERS = \
agentaspect/agentaspect.h \
agentaspect/effector.h \
agentaspect/forceeffector.h \
agentaspect/perceptor.h \
agentaspect/visionperceptor.h \
controlaspect/controlaspect.h \
fontserver/font.h \
fontserver/fontserver.h \
fontserver/glyph.h \
imageserver/image.h \
imageserver/imageserver.h \
inputserver/inputdevice.h \
inputserver/inputserver.h \
inputserver/inputsystem.h \
inputserver/scancodemap.h \
kerosin.h \
materialserver/material.h \
materialserver/materialserver.h \
sceneserver/axis.h \
sceneserver/basenode.h \
sceneserver/camera.h \
sceneserver/helper/NVMeshMender.h \
sceneserver/helper/nv_math/nv_algebra.h \
sceneserver/helper/nv_math/nv_math.h \
sceneserver/helper/nv_math/nv_mathdecl.h \
sceneserver/indexbuffer.h \
sceneserver/light.h \
sceneserver/scene.h \
sceneserver/sceneserver.h \
sceneserver/sphere.h \
sceneserver/staticmesh.h \
sceneserver/transform.h \
soundserver/soundeffect.h \
soundserver/soundmodule.h \
soundserver/soundobject.h \
soundserver/soundserver.h \
soundserver/soundstream.h \
soundserver/soundsystem.h \
textureserver/texture.h \
textureserver/texture2d.h \
textureserver/textureserver.h \
openglserver/openglserver.h

#physicsserver/body.h \
#physicsserver/collider.h \
#physicsserver/odeobject.h \
#physicsserver/physicsserver.h \
#physicsserver/planecollider.h \
#physicsserver/posbody.h \
#physicsserver/space.h \
#physicsserver/spherecollider.h \
#physicsserver/world.h 

#sceneserver/fpscontroller.h

#openglserver/glfnproto.h 
