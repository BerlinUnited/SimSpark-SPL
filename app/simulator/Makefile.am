bin_PROGRAMS = simulator

if DEBUG
LDADD = ${top_srcdir}/lib/kerosin/libkerosin_debug.la \
${top_srcdir}/lib/oxygen/liboxygen_debug.la \
${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la
else
LDADD = ${top_srcdir}/lib/kerosin/libkerosin.la \
${top_srcdir}/lib/oxygen/liboxygen.la \
${top_srcdir}/lib/zeitgeist/libzeitgeist.la
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@

simulator_SOURCES = main.cpp
