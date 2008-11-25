if DEBUG
pkglib_LTLIBRARIES = libzeitgeist_debug.la
libzeitgeist_debug_la_SOURCES = $(sources)
libzeitgeist_debug_la_CXXFLAGS = -O -g -W -Wall
libzeitgeist_debug_la_LIBADD = @RUBY_LDFLAGS@ @RUBY_LDADD@
libzeitgeist_debug_la_LDFLAGS = -version-info @zeitgeist_version_info@
else
pkglib_LTLIBRARIES = libzeitgeist.la 
libzeitgeist_la_SOURCES = $(sources)
libzeitgeist_la_CXXFLAGS = -O2
libzeitgeist_la_LIBADD = @RUBY_LDFLAGS@ @RUBY_LDADD@
libzeitgeist_la_LDFLAGS = -version-info @zeitgeist_version_info@
endif

bin_SCRIPTS = zeitgeist-config

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/zeitgeist

## architecture independent data (scripts) to be installed and distributed
dist_pkgdata_DATA = zeitgeist.rb

sources = \
class.cpp \
class_c.cpp \
core.cpp \
corecontext.cpp \
leaf.cpp \
leaf_c.cpp \
parameterlist.cpp \
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
randomserver/randomserver.cpp \
randomserver/randomserver_c.cpp \
scriptserver/gcvalue.cpp \
scriptserver/rubywrapper.cpp \
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
parameterlist.h \
node.h \
object.h \
object_c.h \
zeitgeist.h \
fileserver/fileserver.h \
fileserver/filesystem.h \
logserver/logserver.h \
logserver/logserverstreambuf.h \
randomserver/randomserver.h \
scriptserver/gcvalue.h \
scriptserver/scriptserver.h \
scriptserver/rubywrapper.h 

#telnetserver/telnetdaemon.h \
#telnetserver/telnetserver.h \
#telnetserver/telnetsession.h
