pkglib_LTLIBRARIES = librcssmonitor3D.la

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/rcssmonitor3d

librcssmonitor3D_la_SOURCES = \
	monitorlib.cpp \
	communit.cpp \
	logfileserver.cpp logfileserver_c.cpp \
	commserverbase_c.cpp \
	commserver.cpp commserver_c.cpp \
	gamestate.cpp gamestate_c.cpp

libpkginclude_HEADERS = \
	monitorlib.h \
	communit.h \
	logfileserver.h \
	commserver.h \
	commserverbase.h \
	types.h \
	gamestate.h 
