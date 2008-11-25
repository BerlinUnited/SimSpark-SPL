pkglib_LTLIBRARIES = forceeffector.la

forceeffector_la_SOURCES = export.cpp \
forceeffector.cpp forceeffector.h \
forceeffector_c.cpp \
forceaction.h

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
forceeffector_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
