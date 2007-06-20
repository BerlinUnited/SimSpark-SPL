noinst_PROGRAMS = gendot

if BUILD_KEROSIN

if DEBUG
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
gendot_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
gendot_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib -I${top_srcdir}/utility -I${top_srcdir}/plugin @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

else

if DEBUG
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la 
gendot_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la
gendot_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/app/rcssmonitor3d/lib -I${top_srcdir}/lib -I${top_srcdir}/utility -I${top_srcdir}/plugin @RUBY_CPPFLAGS@
endif

AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@

gendot_SOURCES = \
	main.cpp

dist_pkgdata_DATA = gendot.rb include.dot

zeitgeist.ps: zeitgeist.dot
	dot -Tps zeitgeist.dot -o zeitgeist.ps

zeitgeist.gif: zeitgeist.dot
	dot -Tgif zeitgeist.dot -o zeitgeist.gif

zeitgeist.dot: gendot include.dot 
	./gendot
