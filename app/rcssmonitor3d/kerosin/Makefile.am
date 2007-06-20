if BUILD_KEROSIN
bin_PROGRAMS = rcssmonitor3D-kerosin

if DEBUG
LDADD = -lpthread \
	${top_builddir}/lib/spark/libspark_debug.la \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	${top_builddir}/app/rcssmonitor3d/lib/librcssmonitor3D.la \
	@BOOST_THREADS_LIB@
rcssmonitor3D_kerosin_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread \
	${top_builddir}/lib/spark/libspark.la \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	${top_builddir}/app/rcssmonitor3d/lib/librcssmonitor3D.la \
	@BOOST_THREADS_LIB@
rcssmonitor3D_kerosin_CXXFLAGS = -O2
endif

# -lXi -lXmu
endif

AM_CPPFLAGS = \
	-I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib\
	-I${top_srcdir}/plugin -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@

AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@

rcssmonitor3D_kerosin_SOURCES = \
	main.cpp \
	monitorclientcontrol.cpp \
	monitorclientcontrol_c.cpp \
	monitorinputcontrol.cpp \
	monitorinputcontrol_c.cpp

dist_pkgdata_DATA = \
	rcssmonitor3D-kerosin.rb \
	bindings.rb \
	monitorclientcontrol.h \
	monitorinputcontrol.h

