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

dist_pkgdata_DATA = rsgedit.rb \
	res/xpm_open.xpm \
	res/xpm_rsg.xpm \
	res/xpm_step.xpm \
	res/xpm_agent.xpm \
	res/xpm_paste.xpm \
	res/xpm_ruby.xpm \
	res/xpm_stop.xpm \
	res/xpm_copy.xpm \
	res/xpm_pause.xpm \
	res/xpm_save.xpm \
	res/xpm_undo.xpm \
	res/xpm_cut.xpm \
	res/xpm_play.xpm \
	res/xpm_slider.xpm \
	res/xpm_universal.xpm \
	res/xpm_hinge.xpm \
	res/xpm_redo.xpm \
	res/xpm_sparklogo.xpm \
	res/xpm_new.xpm \
	res/xpm_reload.xpm \
	res/xpm_spark.xpm

rsgedit_SOURCES = \
	aboutDlg.h aboutDlg.cpp \
	inputwx.h inputwx.cpp \
	platform.h platform.cpp \
	mainframe.h mainframe.cpp \
	propertylist.h propertylist.cpp \
	sparkcontextevent.h sparkcontextevent.cpp \
	sparkglrender.h sparkglrender.cpp \
	agentframe.h agentframe.cpp \
	kinematicframe.h kinematicframe.cpp \
	property.h property.cpp \
	simspark.h simspark.cpp \
	sparkedit.h sparkedit.cpp \
	sparktree.h sparktree.cpp \
	constants.h constants.cpp \
	main.h main.cpp \
	propertyframe.h propertyframe.cpp \
	sparkcontext.h sparkcontext.cpp \
	sparkglcanvas.h sparkglcanvas.cpp
