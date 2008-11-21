if BUILD_WXWIDGETS
pkglib_LTLIBRARIES = inputwx.la
endif

inputwx_la_SOURCES = \
	export.cpp \
	inputsystemwx.h \
	inputsystemwx.cpp \
	inputsystemwx_c.cpp \
	inputwx.h \
	inputwx.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
inputwx_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ @WX_CPPFLAGS@

