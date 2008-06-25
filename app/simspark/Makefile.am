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

if NO_RPATH
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@ 
else
AM_LDFLAGS  = -Wl,--rpath,$(DESTDIR)$(pkglibdir) @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@ 
endif



simspark_SOURCES = \
	main.cpp 

dist_pkgdata_DATA = \
	simspark.rb \
	bindings.rb \
	soccersim.rb \
	naosoccersim.rb \
	internalsoccermonitor.rb \
	internalsoccerbindings.rb

install-data-local: $(top_builddir)/$(subdir)/rsg/agent/ball.rsg
	mkdir -p $(DESTDIR)$(pkgdatadir)/
	find . -type f -exec chmod a-x {} \;
	for res in rsg fonts scripts models textures materials;\
	do \
	rm -rf $(DESTDIR)$(pkgdatadir)/$$res;\
	cp -r $(srcdir)/$$res $(DESTDIR)$(pkgdatadir)/;\
	chmod -R a+rX $(DESTDIR)$(pkgdatadir)/$$res;\
	done

EXTRA_DIST = \
	rsg/agent/soccerplayer.rsg \
	rsg/agent/leggedsphere.rsg \
	rsg/agent/hoap2.rsg \
	rsg/agent/soccerbot055.rsg \
	rsg/agent/soccerbot056.rsg \
	rsg/agent/soccerbottorso.rsg \
	rsg/agent/soccerbotcomp.rsg \
	rsg/agent/soccerbotleftlowerarmcomp.rsg \
	rsg/agent/soccerbotrightlowerarmcomp.rsg \
	rsg/agent/soccerbottorsocomp.rsg \
	rsg/boxspheres/box_with_handler055.rsg \
	rsg/boxspheres/box_with_handler056.rsg \
	rsg/boxspheres/ccylinder.rsg \
	rsg/boxspheres/layer.rsg \
	rsg/boxspheres/row.rsg \
	rsg/boxspheres/staticbox.rsg \
	rsg/agent/soccer.rsg \
	rsg/agent/flag.rsg \
	rsg/agent/flag_no_viz.rsg \
	rsg/agent/flag_left.rsg \
	rsg/agent/flag_left1.rsg \
	rsg/agent/flag_left2.rsg \
	rsg/agent/flag_right.rsg \
	rsg/agent/flag_right1.rsg \
	rsg/agent/flag_right2.rsg \
	rsg/agent/simspark.rsg \
	rsg/agent/ball.rsg \
	rsg/boxspheres/staticsphere.rsg \
	rsg/boxspheres/staticccylinder.rsg \
	rsg/boxspheres/sphere.rsg \
	rsg/boxspheres/simspark.rsg \
	rsg/boxspheres/arena.rsg \
	rsg/boxspheres/box.rsg \
	rsg/boxspheres/box_with_handler.rsg \
	fonts/VeraMono.ttf\
	scripts/materials.rb\
	scripts/rcs-materials.rb\
	scripts/rcs-materials-textures.rb\
	scripts/rsgedit-materials.rb\
	models/soccerball.obj\
	models/soccerbottorso.obj\
	models/naosoccerfield.obj\
	models/lthigh.obj\
	models/rthigh.obj\
	models/lfoot.obj\
	models/lupperarm.obj\
	models/rfoot.obj\
	models/rupperarm.obj\
	models/llowerarm.obj\
	models/naobody.obj\
	models/rlowerarm.obj\
	models/lshank.obj\
	models/naohead.obj\
	models/rshank.obj\
	models/skybox.obj\
	models/leftgoal.obj\
	models/rightgoal.obj\
	materials/soccerball.mtl\
	materials/soccerbottorso.mtl\
	materials/naosoccerfield.mtl\
	materials/nao.mtl\
	materials/skybox.mtl\
	materials/leftgoal.mtl\
	materials/rightgoal.mtl\
	textures/rcs-grass-w-lines.tga\
	textures/rcs-grass.png\
	textures/rcs-grass3.png\
	textures/rcs-soccerball.tga\
	textures/rcs-soccerball.png\
	textures/rcs-torso-01.png\
	textures/rcs-torso-02.png\
	textures/rcs-torso-03.png\
	textures/rcs-torso-04.png\
	textures/rcs-torso-05.png\
	textures/rcs-torso-06.png\
	textures/rcs-torso-07.png\
	textures/rcs-torso-08.png\
	textures/rcs-torso-09.png\
	textures/rcs-torso-10.png\
	textures/rcs-torso-11.png\
	textures/rcs-unum01.png\
	textures/rcs-unum02.png\
	textures/rcs-unum03.png\
	textures/rcs-unum04.png\
	textures/rcs-unum05.png\
	textures/rcs-unum06.png\
	textures/rcs-unum07.png\
	textures/rcs-unum08.png\
	textures/rcs-unum09.png\
	textures/rcs-unum10.png\
	textures/rcs-unum11.png\
	textures/rcs-naofield.png\
	textures/num1.png\
	textures/num2.png\
	textures/num3.png\
	textures/num4.png\
	textures/num5.png\
	textures/num6.png\
	textures/num7.png\
	textures/num8.png\
	textures/num9.png\
	textures/num10.png\
	textures/num11.png\
	textures/skyrender0001.jpg\
	textures/skyrender0002.jpg\
	textures/skyrender0003.jpg\
	textures/skyrender0004.jpg\
	textures/skyrender0005.jpg\
	textures/skyrender0006.jpg\
	textures/naogoalnet.png\
	rsg/agent/nao/box_appearance.rsg\
	rsg/agent/nao/box_physics.rsg\
	rsg/agent/nao/box_physics_with_handler.rsg\
	rsg/agent/nao/box_physics_nocollider.rsg\
	rsg/agent/nao/ccylinder_appearance.rsg\
	rsg/agent/nao/ccylinder_physics.rsg\
	rsg/agent/nao/ccylinder_physics_nocollider.rsg\
        rsg/agent/nao/contactjointhandler.rsg\
	rsg/agent/nao/dragcontroller.rsg\
	rsg/agent/nao/hingejoint.rsg\
	rsg/agent/nao/universaljoint.rsg\
	rsg/agent/nao/nao.rsg\
	rsg/agent/nao/naoarm.rsg\
	rsg/agent/nao/naoleg.rsg\
	rsg/agent/nao/naoneckhead.rsg\
	rsg/agent/nao/sphere_appearance.rsg\
	rsg/agent/nao/sphere_physics.rsg\
	rsg/agent/nao/sphere_physics_nocollider.rsg\
        rsg/agent/nao/touchperceptorhandler.rsg\
	rsg/agent/nao/soccer.rsg\
	rsg/agent/nao/goal.rsg\
	rsg/agent/soccerbot058/box.rsg\
	rsg/agent/soccerbot058/box_with_uj.rsg\
	rsg/agent/soccerbot058/lowerarm_body.rsg\
	rsg/agent/soccerbot058/box_with_handler.rsg\
	rsg/agent/soccerbot058/lowerarm.rsg\
	rsg/agent/soccerbot058/box_with_hj.rsg\
	rsg/agent/soccerbot058/soccerbot.rsg\
	rsg/agent/soccerbot058/box_with_hj_with_ccylinder.rsg\
	rsg/agent/soccerbot058/foot.rsg\
	rsg/agent/soccerbot058/soccerbottorso.rsg\
	rsg/agent/soccerbot058/head.rsg\
	rsg/agent/soccerbot058/contactjointhandler.rsg

