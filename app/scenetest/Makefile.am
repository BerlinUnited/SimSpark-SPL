noinst_PROGRAMS = scenetest

all-local:
	mkdir -p ${srcdir}/sys/
	cd ${srcdir}/sys/ && \
	${LN_S} ../script .

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	-lIL -lGL -lruby @RUBY_LDADD@ \
	${top_srcdir}/utility/libobj/liblibobject.la
scenetest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	-lIL -lGL -lruby @RUBY_LDADD@ \
	${top_srcdir}/utility/libobj/liblibobject.la
scenetest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

scenetest_SOURCES = main.cpp \
md5mesh.cpp md5mesh.h md5mesh_c.cpp 

