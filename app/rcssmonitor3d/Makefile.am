noinst_PROGRAMS = rcssmonitor3D

if DEBUG
LDADD = \
	/usr/lib/libGL.so \
	/usr/lib/libGLU.so \
	/usr/lib/libglut.so \
	/usr/local/lib/librcssnet.la \
	/usr/X11R6/lib/libX11.so \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
rcssmonitor3D_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	/usr/lib/libGL.so \
	/usr/lib/libGLU.so \
	/usr/lib/libglut.so \
	/usr/local/lib/librcssnet.la \
	/usr/X11R6/lib/libX11.so \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
rcssmonitor3D_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssmonitor3D_SOURCES = \
	main.cpp \
	camera.cpp \
	glserver.cpp \
	communit.cpp \
	commserver.cpp \
	tcpsocket.cpp

EXTRA_DIST = rcssmonitor3D.rb zeitgeist.rb
