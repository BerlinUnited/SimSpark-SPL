if BUILD_KEROSIN
if DEBUG
pkglib_LTLIBRARIES = libkerosin_debug.la
libkerosin_debug_la_SOURCES = $(sources)
libkerosin_debug_la_CXXFLAGS = -O -g -W -Wall
libkerosin_debug_la_LIBADD = @FREETYPE_LIBADD@ -lSDL -lIL -lGL 
else
pkglib_LTLIBRARIES = libkerosin.la
libkerosin_la_SOURCES = $(sources)
libkerosin_la_CXXFLAGS = -O2
libkerosin_la_LIBADD = @FREETYPE_LIBADD@ -lSDL -lIL -lGL 
endif
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I$(top_srcdir)/lib/kerosin/sceneserver/helper @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/kerosin

## architecture independent data (scripts) to be installed and distributed
dist_pkgdata_DATA = kerosin.rb

if BUILD_KEROSIN
BUILT_SOURCES = openglserver/glextensionreg.cpp openglserver/glextensionreg.h
endif
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
sceneserver/helper/NVMeshMender.cpp \
sceneserver/helper/nv_algebra.cpp \
sceneserver/light.cpp \
sceneserver/light_c.cpp \
sceneserver/sphere.cpp \
sceneserver/sphere_c.cpp \
sceneserver/staticmesh.cpp \
sceneserver/staticmesh_c.cpp \
soundserver/soundserver.cpp \
soundserver/soundserver_c.cpp \
sceneserver/transform.cpp \
sceneserver/transform_c.cpp \
textureserver/texture.cpp \
textureserver/texture2d.cpp \
textureserver/textureserver.cpp \
textureserver/textureserver_c.cpp \
openglserver/openglserver.cpp \
openglserver/openglserver_c.cpp \
renderserver/renderserver.cpp \
renderserver/renderserver_c.cpp \
openglserver/glextensionreg.cpp

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
kerosin.h \
materialserver/material.h \
materialserver/materialserver.h \
sceneserver/axis.h \
sceneserver/helper/NVMeshMender.h \
sceneserver/helper/nv_math/nv_algebra.h \
sceneserver/helper/nv_math/nv_math.h \
sceneserver/helper/nv_math/nv_mathdecl.h \
sceneserver/light.h \
sceneserver/sphere.h \
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
openglserver/openglserver.h \
renderserver/renderserver.cpp

#sceneserver/camera.h 
