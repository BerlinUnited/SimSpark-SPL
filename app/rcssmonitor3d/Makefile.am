noinst_PROGRAMS = rcssmonitor3D

if DEBUG
LDADD = \
	-lpthread -lGL -lGLU -lglut -lrcssnet \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
rcssmonitor3D_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	-lpthread -lGL -lGLU -lglut -lrcssnet \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
rcssmonitor3D_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssmonitor3D_SOURCES = \
	main.cpp \
	camera.cpp camera.h \
	glserver.cpp glserver.h \
	communit.cpp communit.h \
	commserver.cpp commserver.h commserver_c.cpp \
	tcpsocket.cpp tcpsocket.h \
	types.h

EXTRA_DIST = rcssmonitor3d.rb
