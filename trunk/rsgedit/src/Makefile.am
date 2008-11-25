bin_PROGRAMS = rsgedit

rsgedit_LDADD = -lpthread ${top_builddir}/wxutil/libwxutil.la \
                 @WX_LIBS@

if DEBUG
rsgedit_CXXFLAGS = -O -g -W -Wall
else
rsgedit_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir} \
              @WX_CPPFLAGS@ -I${top_srcdir}/wxutil/include @SALT_CPPFLAGS@ \
                  @SPARK_CPPFLAGS@ @ZEITGEIST_CPPFLAGS@ \
                  @OXYGEN_CPPFLAGS@ @KEROSIN_CPPFLAGS@

if NO_RPATH
AM_LDFLAGS  = @WX_LIBS@ -L${top_builddir}/wxutil @SALT_LDFLAGS@ \
                  @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@
else
AM_LDFLAGS  = -Wl,--rpath,$(DESTDIR)$(pkglibdir) @WX_LIBS@ \
                  -L${top_builddir}/wxutil @SALT_LDFLAGS@ \
                  @SPARK_LDFLAGS@ @ZEITGEIST_LDFLAGS@ \
                  @OXYGEN_LDFLAGS@ @KEROSIN_LDFLAGS@
endif

dist_pkgdata_DATA = rsgedit.rb

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
