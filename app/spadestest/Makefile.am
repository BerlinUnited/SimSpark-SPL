noinst_PROGRAMS = spadestest

all-local:
	mkdir -p ${srcdir}/sys/script/
	rm -f ${srcdir}/sys/script/*.rb
	cd ${srcdir}/sys/script/ && \
	${LN_S} ../../*.rb .

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	-lIL -lGL -lruby @RUBY_LDADD@ \
	${top_srcdir}/utility/libobj/liblibobject.la
spadestest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	-lIL -lGL -lruby @RUBY_LDADD@ \
	${top_srcdir}/utility/libobj/liblibobject.la
spadestest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

spadestest_SOURCES = main.cpp
EXTRA_DIST = spadestest.rb zeitgeist.rb
