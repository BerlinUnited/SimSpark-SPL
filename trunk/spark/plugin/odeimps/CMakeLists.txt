########### next target ###############

set(odeimps_LIB_SRCS
   export.cpp
   odeangularmotor.cpp
   odeangularmotor.h
   odeangularmotor_c.cpp
   odeballjoint.cpp
   odeballjoint.h
   odeballjoint_c.cpp
   odebody.cpp
   odebody.h
   odebody_c.cpp
   odeboxcollider.cpp
   odeboxcollider.h
   odeboxcollider_c.cpp
   odecapsulecollider.cpp
   odecapsulecollider.h
   odecapsulecollider_c.cpp
   odecollider.cpp
   odecollider.h
   odecollider_c.cpp
   odecompoundcollider.cpp
   odecompoundcollider.h
   odecompoundcollider_c.cpp
   odeconcavecollider.cpp
   odeconcavecollider.h
   odeconcavecollider_c.cpp
   odeconecollider.cpp
   odeconecollider.h
   odeconecollider_c.cpp
   odeconetwistjoint.cpp
   odeconetwistjoint.h
   odeconetwistjoint_c.cpp
   odecontactjointhandler.cpp
   odecontactjointhandler.h
   odecontactjointhandler_c.cpp
   odeconvexcollider.cpp
   odeconvexcollider.h
   odeconvexcollider_c.cpp
   odecylindercollider.cpp
   odecylindercollider.h
   odecylindercollider_c.cpp
   odedynamicbody.cpp
   odedynamicbody.h
   odedynamicbody_c.cpp
   odeemptycollider.cpp
   odeemptycollider.h
   odeemptycollider_c.cpp
   odefixedjoint.cpp
   odefixedjoint.h
   odefixedjoint_c.cpp
   odegeneric6dofjoint.cpp
   odegeneric6dofjoint.h
   odegeneric6dofjoint_c.cpp
   odehinge2joint.cpp
   odehinge2joint.h
   odehinge2joint_c.cpp
   odehingejoint.cpp
   odehingejoint.h
   odehingejoint_c.cpp
   odejoint.cpp
   odejoint.h
   odejoint_c.cpp
   odephysicsobject.cpp
   odephysicsobject.h
   odephysicsobject_c.cpp
   odephysicsserver.cpp
   odephysicsserver.h
   odephysicsserver_c.cpp
   odeplanecollider.cpp
   odeplanecollider.h
   odeplanecollider_c.cpp
   oderaycollider.cpp
   oderaycollider.h
   oderaycollider_c.cpp
   oderigidbody.cpp
   oderigidbody.h
   oderigidbody_c.cpp
   odesliderjoint.cpp
   odesliderjoint.h
   odesliderjoint_c.cpp
   odesoftbody.cpp
   odesoftbody.h
   odesoftbody_c.cpp
   odespace.cpp
   odespace.h
   odespace_c.cpp
   odespherecollider.cpp
   odespherecollider.h
   odespherecollider_c.cpp
   odestaticbody.cpp
   odestaticbody.h
   odestaticbody_c.cpp
   odetransformcollider.cpp
   odetransformcollider.h
   odetransformcollider_c.cpp
   odeuniversaljoint.cpp
   odeuniversaljoint.h
   odeuniversaljoint_c.cpp
   odeworld.cpp
   odeworld.h
   odeworld_c.cpp
   odewrapper.h
)

add_library(odeimps MODULE ${odeimps_LIB_SRCS})

target_link_libraries(odeimps ${ODE_LIBRARY} ${spark_libs})

if (NOT APPLE)
   set_target_properties(odeimps PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS odeimps DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
