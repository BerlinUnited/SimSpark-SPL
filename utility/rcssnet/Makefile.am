pkglib_LTLIBRARIES = librcssnet3D.la

librcssnet3D_la_SOURCES = \
addr.cpp  \
exception.cpp  \
socket.cpp  \
tcpsocket.cpp  \
udpsocket.cpp

libpkginclude_HEADERS = \
addr.hpp \
exception.hpp \
iosocketstream.hpp \
isocketstream.hpp \
osocketstream.hpp \
socket.hpp \
socketstreambuf.hpp \
tcpsocket.hpp \
udpsocket.hpp 

librcssnet3D_la_LIBADD = @NET_LIBS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/rcssnet

EXTRA_DIST = README
