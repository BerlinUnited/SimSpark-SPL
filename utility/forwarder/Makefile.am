noinst_LTLIBRARIES = libforwarder.la
libforwarder_la_SOURCES = forwarder.cc forwarder.h  \
                          forwarderstreambuf.cc forwarderstreambuf.h

noinst_PROGRAMS = example
example_SOURCES = example.cc
example_LDADD   = libforwarder.la
