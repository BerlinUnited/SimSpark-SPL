SUBDIRS = sensor

AM_CPPFLAGS = \
-I$(top_srcdir)/utility/mathstuff/ \
-I$(top_srcdir)/utility/forwarder/ \
-I$(top_srcdir)/utility/console/

noinst_PROGRAMS = example
example_SOURCES = example.cc \
simulator.h simulator.cc
example_LDADD = -lode -lpthread \
$(top_srcdir)/utility/console/libconsole.la \
$(top_srcdir)/utility/forwarder/libforwarder.la \
$(top_srcdir)/utility/classserver/libclassserver.la
