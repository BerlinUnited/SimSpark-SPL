if BUILD_KEROSIN
bin_PROGRAMS = rcssmonitor3D-kerosin

if DEBUG
LDADD = -lpthread -lGL -lGLU -lglut \
	${top_srcdir}/lib/spark/libspark_debug.la \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	${top_srcdir}/app/rcssmonitor3d/lib/librcssmonitor3D.la 
rcssmonitor3D_kerosin_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread -lGL -lGLU -lglut \
	${top_srcdir}/lib/spark/libspark.la \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	${top_srcdir}/app/rcssmonitor3d/lib/librcssmonitor3D.la
rcssmonitor3D_kerosin_CXXFLAGS = -O2
endif

endif

AM_CPPFLAGS = \
	-I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib\
	-I${top_srcdir}/plugin -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssmonitor3D_kerosin_SOURCES = \
	main.cpp

dist_pkgdata_DATA = rcssmonitor3D-kerosin.rb bindings.rb
