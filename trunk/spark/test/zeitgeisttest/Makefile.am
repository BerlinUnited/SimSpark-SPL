noinst_PROGRAMS = zeitgeisttest

if DEBUG
LDADD = ${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la 
zeitgeisttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = ${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la 
zeitgeisttest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

zeitgeisttest_SOURCES = main.cpp
