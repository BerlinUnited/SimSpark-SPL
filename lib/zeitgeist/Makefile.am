if DEBUG
pkglib_LTLIBRARIES = libzeitgeist_debug.la
libzeitgeist_debug_la_SOURCES = $(sources)
libzeitgeist_debug_la_CXXFLAGS = -O -g -W -Wall
else
pkglib_LTLIBRARIES = libzeitgeist.la 
libzeitgeist_la_SOURCES = $(sources)
libzeitgeist_la_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/zeitgeist

sources = \
class.cpp \
core.cpp \
corecontext.cpp \
leaf.cpp \
leaf_c.cpp \
node.cpp \
node_c.cpp \
object.cpp \
object_c.cpp \
zeitgeist.cpp \
fileserver/fileserver.cpp \
fileserver/fileserver_c.cpp \
fileserver/filesystem_c.cpp \
logserver/logserver.cpp \
logserver/logserver_c.cpp \
logserver/logserverstreambuf.cpp \
scriptserver/scriptserver.cpp \
scriptserver/scriptserver_c.cpp

#telnetserver/telnetdaemon.cpp \
#telnetserver/telnetserver.cpp \
#telnetserver/telnetserver_c.cpp \
#telnetserver/telnetsession.cpp

nobase_libpkginclude_HEADERS = \
class.h \
core.h \
corecontext.h \
leaf.h \
node.h \
object.h \
object_c.h \
zeitgeist.h \
fileserver/fileserver.h \
fileserver/filesystem.h \
logserver/logserver.h \
logserver/logserverstreambuf.h \
scriptserver/scriptserver.h

#telnetserver/telnetdaemon.h \
#telnetserver/telnetserver.h \
#telnetserver/telnetsession.h
