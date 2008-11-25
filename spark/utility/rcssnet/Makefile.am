pkglib_LTLIBRARIES = librcssnet3D.la

librcssnet3D_la_SOURCES = \
addr.cpp  \
exception.cpp  \
handler.cpp \
socket.cpp  \
tcpsocket.cpp  \
udpsocket.cpp

libpkginclude_HEADERS = \
addr.hpp \
exception.hpp \
handler.hpp \
iosocketstream.hpp \
isocketstream.hpp \
osocketstream.hpp \
socket.hpp \
socketstreambuf.hpp \
tcpsocket.hpp \
udpsocket.hpp 

librcssnet3D_la_LIBADD = @NET_LIBS@
librcssnet3D_la_LDFLAGS = -version-info @rcssnet3D_version_info@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/rcssnet

EXTRA_DIST = README
