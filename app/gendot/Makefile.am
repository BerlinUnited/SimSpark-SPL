noinst_PROGRAMS = gendot

if BUILD_KEROSIN

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
gendot_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la
gendot_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib -I${top_srcdir}/plugin/soccer @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

else

if DEBUG
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
gendot_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la
gendot_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib -I${top_srcdir}/plugin/soccer @RUBY_CPPFLAGS@
endif

AM_LDFLAGS  = @RUBY_LDFLAGS@

gendot_SOURCES = \
	main.cpp

dist_pkgdata_DATA = gendot.rb include.dot
