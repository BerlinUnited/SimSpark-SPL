bin_PROGRAMS = simulator

if DEBUG
LDADD = ${top_srcdir}/lib/kerosin/libkerosin_debug.la \
${top_srcdir}/lib/oxygen/liboxygen_debug.la \
${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
-lGL
else
LDADD = ${top_srcdir}/lib/kerosin/libkerosin.la \
${top_srcdir}/lib/oxygen/liboxygen.la \
${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
-lGL
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

simulator_SOURCES = main.cpp
