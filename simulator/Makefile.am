SUBDIRS = nodecreator sensor

AM_CPPFLAGS = \
-I$(top_srcdir)/utility/mathstuff/ \
-I$(top_srcdir)/utility/forwarder/ \
-I$(top_srcdir)/utility/console/

noinst_PROGRAMS = example
example_SOURCES = example.cc \
simulator.cc simulator.h \
basenode.cc basenode.h \
dirnode.cc dirnode.h \
entitytree.cc entitytree.h \
objects/bodynode.cc objects/bodynode.h \
objects/geometrynode.cc objects/geometrynode.h \
objects/planenode.cc objects/planenode.h \
objects/spherenode.cc objects/spherenode.h \
objects/worldnode.cc objects/worldnode.h 


example_LDADD = -lode -ldrawstuff -lpthread -lX11 -lGL -lGLU -lm \
$(top_srcdir)/utility/console/libconsole.la \
$(top_srcdir)/utility/forwarder/libforwarder.la \
$(top_srcdir)/utility/classserver/libclassserver.la \
$(top_srcdir)/simulator/nodecreator/libnodecreator.la

MOSTLYCLEANFILES = core.*
