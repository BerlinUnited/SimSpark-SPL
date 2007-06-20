if BUILD_KEROSIN
noinst_PROGRAMS = inputtest
endif

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
inputtest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
inputtest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
AM_LDFLAGS = @IL_LDFLAGS@ @SDL_LDFLAGS@ @GL_LDFLAGS@

inputtest_SOURCES = main.cpp
EXTRA_DIST = inputtest.rb init.rb german.scan.rb
