if DEBUG
pkglib_LTLIBRARIES = liboxygen_debug.la
liboxygen_debug_la_SOURCES = $(sources)
liboxygen_debug_la_CXXFLAGS = -O -g -W -Wall
liboxygen_debug_la_LIBADD = -lode
else
pkglib_LTLIBRARIES = liboxygen.la
liboxygen_la_SOURCES = $(sources)
liboxygen_la_CXXFLAGS = -O2
liboxygen_la_LIBADD = -lode
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/oxygen

## architecture independent data (scripts) to be installed and distributed
dist_pkgdata_DATA = oxygen.rb

sources = \
agentaspect/agentaspect.cpp \
agentaspect/agentaspect_c.cpp \
agentaspect/collisionperceptor.cpp \
agentaspect/collisionperceptor_c.cpp \
agentaspect/effector.cpp \
agentaspect/effector_c.cpp \
agentaspect/forceeffector.cpp \
agentaspect/forceeffector_c.cpp \
agentaspect/perceptor.cpp \
agentaspect/perceptor_c.cpp \
controlaspect/controlaspect.cpp \
controlaspect/controlaspect_c.cpp \
oxygen.cpp \
physicsserver/body.cpp \
physicsserver/body_c.cpp \
physicsserver/boxcollider.cpp \
physicsserver/boxcollider_c.cpp \
physicsserver/ccylindercollider.cpp \
physicsserver/ccylindercollider_c.cpp \
physicsserver/collider.cpp \
physicsserver/collider_c.cpp \
physicsserver/odeobject.cpp \
physicsserver/odeobject_c.cpp \
physicsserver/physicsserver.cpp \
physicsserver/physicsserver_c.cpp \
physicsserver/planecollider.cpp \
physicsserver/planecollider_c.cpp \
physicsserver/space.cpp \
physicsserver/space_c.cpp \
physicsserver/spherecollider.cpp \
physicsserver/spherecollider_c.cpp \
physicsserver/world.cpp \
physicsserver/world_c.cpp \
sceneserver/basenode.cpp \
sceneserver/basenode_c.cpp \
sceneserver/fpscontroller.cpp \
sceneserver/fpscontroller_c.cpp \
sceneserver/scene.cpp \
sceneserver/scene_c.cpp \
sceneserver/sceneserver.cpp \
sceneserver/sceneserver_c.cpp \
sceneserver/indexbuffer.cpp \
sceneserver/transform.cpp \
sceneserver/transform_c.cpp \
sceneserver/camera.cpp \
sceneserver/camera_c.cpp 

nobase_libpkginclude_HEADERS = \
agentaspect/agentaspect.h \
agentaspect/collisionperceptor.h \
agentaspect/effector.h \
agentaspect/forceeffector.h \
agentaspect/perceptor.h \
controlaspect/controlaspect.h \
oxygen.h \
physicsserver/body.h \
physicsserver/boxcollider.h \
physicsserver/ccylindercollider.h \
physicsserver/collider.h \
physicsserver/odeobject.h \
physicsserver/physicsserver.h \
physicsserver/planecollider.h \
physicsserver/space.h \
physicsserver/spherecollider.h \
physicsserver/world.h \
sceneserver/basenode.h \
sceneserver/fpscontroller.h \
sceneserver/scene.h \
sceneserver/sceneserver.h \
sceneserver/indexbuffer.h \
sceneserver/transform.h \
sceneserver/camera.h 

