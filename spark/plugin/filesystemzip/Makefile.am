if HAVE_BOOST_REGEX
pkglib_LTLIBRARIES = filesystemzip.la
endif

filesystemzip_la_SOURCES = export.cpp \
filesystemzip.cpp filesystemzip.h \
filesystemzip_c.cpp 

filesystemzip_la_LIBADD = -lboost_regex

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
filesystemzip_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

