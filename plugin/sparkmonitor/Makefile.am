pkglib_LTLIBRARIES = sparkmonitor.la

sparkmonitor_la_SOURCES = \
	export.cpp \
	sparkmonitor.cpp \
	sparkmonitor_c.cpp \
	sparkmonitor.h \
	sparkmonitorclient.h \
	sparkmonitorclient.cpp \
	sparkmonitorclient_c.cpp \
	sparkmonitorlogfileserver.h \
	sparkmonitorlogfileserver.cpp \
	sparkmonitorlogfileserver_c.cpp

sparkmonitor_la_LDFLAGS = -module -version-info 0:0:0 -L${top_builddir}/utility/sfsexp/
sparkmonitor_la_LIBADD = -lsexp

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@

