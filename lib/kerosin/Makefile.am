if BUILD_KEROSIN
if DEBUG
pkglib_LTLIBRARIES = libkerosin_debug.la
libkerosin_debug_la_SOURCES = $(sources) 
libkerosin_debug_la_CXXFLAGS = -O -g -W -Wall
libkerosin_debug_la_LIBADD = @FREETYPE_LIBADD@ @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@ 
libkerosin_debug_la_LDFLAGS = -version-info @kerosin_version_info@ 
else
pkglib_LTLIBRARIES = libkerosin.la
libkerosin_la_SOURCES = $(sources) 
libkerosin_la_CXXFLAGS = -O2
libkerosin_la_LIBADD = @FREETYPE_LIBADD@ @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@
libkerosin_la_LDFLAGS = -version-info @kerosin_version_info@
endif

bin_SCRIPTS = kerosin-config

endif

AM_CPPFLAGS = -I${top_srcdir}/lib @SDL_CPPFLAGS@ @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/kerosin

## architecture independent data (scripts) to be installed and distributed
dist_pkgdata_DATA = \
	kerosin.rb \
	inputserver/german.scan.rb \
	openglserver/glbase.h 

sources = \
fontserver/font.cpp \
fontserver/fontserver.cpp \
fontserver/fontserver_c.cpp \
fontserver/glyph.cpp \
imageserver/image.cpp \
imageserver/imageserver.cpp \
imageserver/imageserver_c.cpp \
inputserver/input.cpp \
inputserver/inputdevice.cpp \
inputserver/inputdevice_c.cpp \
inputserver/inputserver.cpp \
inputserver/inputserver_c.cpp \
inputserver/inputsystem.cpp \
inputserver/inputsystem_c.cpp \
inputserver/scancodemap.cpp \
inputserver/inputcontrol.cpp \
inputserver/inputcontrol_c.cpp \
inputserver/inputitem.cpp \
inputserver/inputitem_c.cpp \
kerosin.cpp \
materialserver/material.cpp \
materialserver/material_c.cpp \
materialserver/material2dtexture.cpp \
materialserver/material2dtexture_c.cpp \
materialserver/materialexternal.cpp \
materialserver/materialexternal_c.cpp \
materialserver/materialsolid.cpp \
materialserver/materialsolid_c.cpp \
materialserver/materialserver.cpp \
materialserver/materialserver_c.cpp \
materialserver/materialexporter_c.cpp \
sceneserver/axis.cpp \
sceneserver/axis_c.cpp \
sceneserver/light.cpp \
sceneserver/light_c.cpp \
sceneserver/singlematnode.cpp \
sceneserver/singlematnode_c.cpp \
sceneserver/box.cpp \
sceneserver/box_c.cpp \
sceneserver/sphere.cpp \
sceneserver/sphere_c.cpp \
sceneserver/ccylinder.cpp \
sceneserver/ccylinder_c.cpp \
sceneserver/cylinder.cpp \
sceneserver/cylinder_c.cpp \
sceneserver/staticmesh.cpp \
sceneserver/staticmesh_c.cpp \
soundserver/soundserver.cpp \
soundserver/soundserver_c.cpp \
textureserver/texture.cpp \
textureserver/texture2d.cpp \
textureserver/textureserver.cpp \
textureserver/textureserver_c.cpp \
openglserver/openglserver.cpp \
openglserver/openglserver_c.cpp \
openglserver/openglsystem_c.cpp \
renderserver/baserenderserver.cpp \
renderserver/baserenderserver_c.cpp \
renderserver/renderserver.cpp \
renderserver/renderserver_c.cpp \
renderserver/rendernode.cpp \
renderserver/rendernode_c.cpp \
renderserver/rendercontrol.cpp \
renderserver/rendercontrol_c.cpp \
renderserver/customrender.cpp \
renderserver/customrender_c.cpp

nobase_libpkginclude_HEADERS = \
fontserver/font.h \
fontserver/fontserver.h \
fontserver/glyph.h \
imageserver/image.h \
imageserver/imageserver.h \
inputserver/inputdevice.h \
inputserver/inputserver.h \
inputserver/inputsystem.h \
inputserver/scancodemap.h \
inputserver/inputcontrol.h \
inputserver/inputitem.h \
inputserver/input.h \
kerosin.h \
materialserver/material.h \
materialserver/materialserver.h \
sceneserver/axis.h \
materialserver/material2dtexture.h \
materialserver/materialexternal.h \
materialserver/materialsolid.h \
materialserver/materialexporter.h \
sceneserver/singlematnode.h \
sceneserver/light.h \
sceneserver/sphere.h \
sceneserver/box.h \
sceneserver/ccylinder.h \
sceneserver/cylinder.h \
sceneserver/staticmesh.h \
soundserver/soundeffect.h \
soundserver/soundmodule.h \
soundserver/soundobject.h \
soundserver/soundserver.h \
soundserver/soundstream.h \
soundserver/soundsystem.h \
textureserver/texture.h \
textureserver/texture2d.h \
textureserver/textureserver.h \
openglserver/openglwrapper.h \
openglserver/openglsystem.h \
openglserver/openglserver.h \
openglserver/glbase.h \
renderserver/baserenderserver.h \
renderserver/renderserver.h \
renderserver/rendernode.h \
renderserver/rendercontrol.h \
renderserver/customrender.h

