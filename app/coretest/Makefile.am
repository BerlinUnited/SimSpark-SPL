noinst_PROGRAMS = coretest

if BUILD_KEROSIN
if DEBUG
my_ldadd=${top_srcdir}/lib/kerosin/libkerosin_debug.la
else
my_ldadd=${top_srcdir}/lib/kerosin/libkerosin.la
endif
else
my_ldadd= 
endif


if DEBUG
LDADD = ${my_ldadd} \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
coretest_CXXFLAGS = -O -g -W -Wall
else
LDADD = ${my_ldadd} \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
coretest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

coretest_SOURCES = main.cpp 
EXTRA_DIST = data/monkey.wav

