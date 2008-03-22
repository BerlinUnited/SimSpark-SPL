if BUILD_WXWIDGETS
bin_PROGRAMS = rsgedit
endif

if DEBUG
rsgedit_LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark_debug.la \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
        ${top_builddir}/utility/wxutil/libwxutil.la \
	@BOOST_THREADS_LIB@ @WX_LIBS@
rsgedit_CXXFLAGS = -O -g -W -Wall
else
rsgedit_LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark.la \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
        ${top_builddir}/utility/wxutil/libwxutil.la \
	@BOOST_THREADS_LIB@ @WX_LIBS@
rsgedit_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility -I${top_srcdir}/app \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @WX_CPPFLAGS@ -I${top_srcdir}/utility/wxutil/include

if BUNDLE_SUPPORT
AM_LDFLAGS  = @RUBY_LDFLAGS@ @WX_LIBS@ @IL_LDFLAGS@ @GL_LDFLAGS@ -L${top_builddir}/utility/wxutil
else
AM_LDFLAGS  = -Wl,--rpath,$(DESTDIR)$(pkglibdir) @RUBY_LDFLAGS@ @WX_LIBS@ @IL_LDFLAGS@ @GL_LDFLAGS@
endif

dist_pkgdata_DATA = rsgedit.rb

rsgedit_SOURCES = \
	aboutDlg.cpp \
	inputwx.cpp \
	platform.cpp \
	mainframe.cpp \
	propertylist.cpp \
	sparkcontextevent.cpp \
	sparkglrender.cpp \
	agentframe.cpp \
	kinematicframe.cpp \
	property.cpp \
	simspark.cpp \
	sparkedit.cpp \
	sparktree.cpp \
	constants.cpp \
	main.cpp \
	propertyframe.cpp \
	sparkcontext.cpp \
	sparkglcanvas.cpp
