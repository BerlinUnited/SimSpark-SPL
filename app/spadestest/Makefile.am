noinst_PROGRAMS = spadestest

if DEBUG
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
spadestest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
spadestest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

spadestest_SOURCES = main.cpp
EXTRA_DIST = spadestest.rb agentdb.list

