bin_PROGRAMS = simulator

if DEBUG
LDADD = ${top_srcdir}/lib/kerosin/libkerosin_debug.la
else
LDADD = ${top_srcdir}/lib/kerosin/libkerosin.la
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@

simulator_SOURCES = main.cpp
