pkglib_LTLIBRARIES = librcssmonitor3D.la
librcssmonitor3D_la_LIBADD = ${top_srcdir}/utility/rcssnet/librcssnet3D.la

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/rcssmonitor3d

librcssmonitor3D_la_SOURCES = \
	monitorlib.cpp \
	communit.cpp \
	commserver.cpp commserver_c.cpp \
	monitorparser.cpp monitorparser_c.cpp\
	gameparam.cpp \
	gamestate.cpp

libpkginclude_HEADERS = \
	monitorlib.h \
	communit.h \
	commserver.h \
	types.h \
	monitorparser.h \
	gameparam.h \
	gamestate.h 
