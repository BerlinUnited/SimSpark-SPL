noinst_PROGRAMS = zeitgeisttest

if DEBUG
LDADD = ${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	-lruby @RUBY_LDADD@
zeitgeisttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = ${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	-lruby @RUBY_LDADD@ 
zeitgeisttest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

zeitgeisttest_SOURCES = main.cpp
