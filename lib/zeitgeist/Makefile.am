pkglib_LTLIBRARIES = libzeitgeist.la libzeitgeist_debug.la
AM_CPPFLAGS = -I${top_srcdir}/lib

libzeitgeist_la_SOURCES = \
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

libzeitgeist_la_CXXFLAGS = -O2

libzeitgeist_debug_la_SOURCES = \
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

libzeitgeist_debug_la_CXXFLAGS = -O -g

nobase_pkginclude_HEADERS = \
class.h \
class_m.h \
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
