if BUILD_GLUT
bin_PROGRAMS = rcssmonitor3D-lite
endif

if DEBUG
LDADD = -lpthread  \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	${top_builddir}/app/rcssmonitor3d/lib/librcssmonitor3D.la @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@ @BOOST_THREADS_LIB@
rcssmonitor3D_lite_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread  \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	${top_builddir}/app/rcssmonitor3d/lib/librcssmonitor3D.la @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@ @BOOST_THREADS_LIB@
rcssmonitor3D_lite_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib -I${top_srcdir}/plugin -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@

rcssmonitor3D_lite_SOURCES = \
	main.cpp \
	monitor.h monitor.cpp \
	camera.h camera.cpp \
	glserver.h glserver.cpp

dist_pkgdata_DATA = rcssmonitor3D-lite.rb grass.ppm
