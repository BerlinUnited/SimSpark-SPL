pkglib_LIBRARIES = librcssmonitor3D.a
librcssmonitor3D_a_LIBADD = @RUBY_LDFLAGS@
AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin/soccer @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/rcssmonitor3d

librcssmonitor3D_a_SOURCES = \
	monitorlib.cpp \
	communit.cpp \
	commserver.cpp commserver_c.cpp \
	tcpsocket.cpp \
	monitorparser.cpp monitorparser_c.cpp\
	gameparam.cpp \
	gamestate.cpp

libpkginclude_HEADERS = \
	monitorlib.h \
	communit.h \
	commserver.h \
	tcpsocket.h \
	monitorparser.h \
	gameparam.h \
	gamestate.h 
