if BUILD_KEROSIN
noinst_PROGRAMS = fonttest
endif

all-local:
	mkdir -p ${srcdir}/sys/
	rm -f ${srcdir}/sys/script
	cd ${srcdir}/sys/ && \
	${LN_S} ../script .

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
fonttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
fonttest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

fonttest_SOURCES = main.cpp 


