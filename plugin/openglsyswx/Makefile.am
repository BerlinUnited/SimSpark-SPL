if BUILD_KEROSIN
if BUILD_WXWIDGETS
pkglib_LTLIBRARIES = openglsyswx.la
endif
endif

openglsyswx_la_SOURCES = \
	export.cpp \
        openglsystemwx.cpp \
        openglsystemwx.h \
        openglsystemwx_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
openglsyswx_la_LDFLAGS = -module

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ @WX_CPPFLAGS@

