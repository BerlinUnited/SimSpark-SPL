if BUILD_KEROSIN
bin_PROGRAMS = simspark
endif

if DEBUG
LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark_debug.la \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
simspark_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark.la \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
simspark_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@

if BUNDLE_SUPPORT
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@ 
else
AM_LDFLAGS  = -Wl,--rpath,$(DESTDIR)$(pkglibdir) @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@ 
endif



simspark_SOURCES = \
	main.cpp 

# Original
#dist_pkgdata_DATA = \
#	simspark.rb \
#	bindings.rb \
#	materials.rb \
#	soccersim.rb \
#	rsg/agent/soccerplayer.rsg \
#	rsg/agent/leggedsphere.rsg \
#	rsg/agent/hoap2.rsg \
#	rsg/agent/soccerbot.rsg \
#	rsg/agent/soccer.rsg \
#	rsg/agent/flag.rsg \
#	rsg/agent/flag_no_viz.rsg \
#	rsg/agent/flag_left.rsg \
#	rsg/agent/flag_right.rsg \
#	rsg/agent/simspark.rsg \
#	rsg/agent/ball.rsg \
#	rsg/boxspheres/staticsphere.rsg \
#	rsg/boxspheres/staticccylinder.rsg \
#	rsg/boxspheres/sphere.rsg \
#	rsg/boxspheres/simspark.rsg \
#	rsg/boxspheres/arena.rsg \
#	rsg/boxspheres/box.rsg \
#	rsg/boxspheres/box_with_handler.rsg

# JAN
dist_pkgdata_DATA = \
	simspark.rb \
	bindings.rb \
	materials.rb \
	materials-textures.rb \
	soccersim.rb \
	internalsoccermonitor.rb \
	internalsoccerbindings.rb

install-data-local: $(top_builddir)/$(subdir)/rsg/agent/ball.rsg
	rm -rf $(DESTDIR)$(pkgdatadir)/rsg
	mkdir -p $(DESTDIR)$(pkgdatadir)/
	cp -r $(srcdir)/rsg $(DESTDIR)$(pkgdatadir)/
	cp -r $(top_builddir)/$(subdir)/rsg $(DESTDIR)$(pkgdatadir)/
	chmod -R a+rX $(DESTDIR)$(pkgdatadir)/rsg

EXTRA_DIST = \
	rsg/agent/soccerplayer.rsg \
	rsg/agent/leggedsphere.rsg \
	rsg/agent/hoap2.rsg \
	rsg/agent/soccerbot.rsg \
	rsg/agent/soccerbottorso.rsg \
	rsg/agent/soccer.rsg \
	rsg/agent/flag.rsg \
	rsg/agent/flag_no_viz.rsg \
	rsg/agent/flag_left.rsg \
	rsg/agent/flag_right.rsg \
	rsg/agent/simspark.rsg \
	rsg/agent/ball.rsg \
	rsg/boxspheres/staticsphere.rsg \
	rsg/boxspheres/staticccylinder.rsg \
	rsg/boxspheres/sphere.rsg \
	rsg/boxspheres/simspark.rsg \
	rsg/boxspheres/arena.rsg \
	rsg/boxspheres/box.rsg \
	rsg/boxspheres/box_with_handler.rsg
