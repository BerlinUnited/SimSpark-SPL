pkglib_LTLIBRARIES = gyrorateperceptor.la

gyrorateperceptor_la_SOURCES = export.cpp \
                                    gyrorateperceptor.h \
                                    gyrorateperceptor.cpp \
                                    gyrorateperceptor_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
gyrorateperceptor_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
