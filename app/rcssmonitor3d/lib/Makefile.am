noinst_LIBRARIES = librcssmonitor3D.a

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin/soccer @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

librcssmonitor3D_a_SOURCES = \
	communit.h communit.cpp \
	commserver.h commserver.cpp commserver_c.cpp \
	tcpsocket.h tcpsocket.cpp
