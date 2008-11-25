pkglib_LTLIBRARIES = rosimporter.la

rosimporter_la_SOURCES =\
	export.cpp \
	roselements.cpp\
	roselements.h \
	rosimporter.cpp\
	rosimporter.h \
	rosimporter_c.cpp 

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
rosimporter_la_LDFLAGS = -module -avoid-version -L${top_builddir}/utility/tinyxml
rosimporter_la_LIBADD = -ltinyxml_ex

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@
