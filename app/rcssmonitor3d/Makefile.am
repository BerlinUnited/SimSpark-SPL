bin_PROGRAMS = rcssmonitor3D

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

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin/soccer @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssmonitor3D_SOURCES = \
	main.cpp \
	camera.h camera.cpp \
	glserver.h glserver.cpp \
	communit.h communit.cpp \
	commserver.h commserver.cpp commserver_c.cpp \
	tcpsocket.h tcpsocket.cpp \
	types.h 

dist_pkgdata_DATA = rcssmonitor3d.rb
