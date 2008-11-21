if BUILD_KEROSIN
pkglib_LTLIBRARIES = openglsyssdl.la
endif

openglsyssdl_la_SOURCES = \
	export.cpp \
        openglsystemsdl.cpp \
        openglsystemsdl.h \
        openglsystemsdl_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
openglsyssdl_la_LDFLAGS = -module

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@

