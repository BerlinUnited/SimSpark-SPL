########### next target ###############

set(odeimps_LIB_SRCS
   odeangularmotor.cpp
   odeangularmotor.h
   odeballjoint.cpp
   odeballjoint.h
   odebody.cpp
   odebody.h
   odeboxcollider.cpp
   odeboxcollider.h
   odecapsulecollider.cpp
   odecapsulecollider.h
   odecollider.cpp
   odecollider.h
   odecompoundcollider.cpp
   odecompoundcollider.h
   odeconcavecollider.cpp
   odeconcavecollider.h
   odeconecollider.cpp
   odeconecollider.h
   odeconetwistjoint.cpp
   odeconetwistjoint.h
   odecontactjointhandler.cpp
   odecontactjointhandler.h
   odeconvexcollider.cpp
   odeconvexcollider.h
   odecylindercollider.cpp
   odecylindercollider.h
   odedynamicbody.cpp
   odedynamicbody.h
   odeemptycollider.cpp
   odeemptycollider.h
   odefixedjoint.cpp
   odefixedjoint.h
   odegeneric6dofjoint.cpp
   odegeneric6dofjoint.h
   odehinge2joint.cpp
   odehinge2joint.h
   odehingejoint.cpp
   odehingejoint.h
   odejoint.cpp
   odejoint.h
   odephysicsobject.cpp
   odephysicsobject.h
   odephysicsserver.cpp
   odephysicsserver.h
   odeplanecollider.cpp
   odeplanecollider.h
   oderaycollider.cpp
   oderaycollider.h
   oderigidbody.cpp
   oderigidbody.h
   odesliderjoint.cpp
   odesliderjoint.h
   odesoftbody.cpp
   odesoftbody.h
   odespace.cpp
   odespace.h
   odespherecollider.cpp
   odespherecollider.h
   odestaticbody.cpp
   odestaticbody.h
   odetransformcollider.cpp
   odetransformcollider.h
   odeuniversaljoint.cpp
   odeuniversaljoint.h
   odeworld.cpp
   odeworld.h
   odewrapper.h
)

add_library(odeimps ${odeimps_LIB_SRCS})

target_link_libraries(odeimps ${spark_libs})

if (NOT APPLE)
   set_target_properties(odeimps PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS odeimps DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
