########### next target ###############

set(bulletimps_LIB_SRCS
   export.cpp
   bulletangularmotor.cpp
   bulletangularmotor.h
   bulletangularmotor_c.cpp
   bulletballjoint.cpp
   bulletballjoint.h
   bulletballjoint_c.cpp
   bulletbody.cpp
   bulletbody.h
   bulletbody_c.cpp
   bulletboxcollider.cpp
   bulletboxcollider.h
   bulletboxcollider_c.cpp
   bulletcapsulecollider.cpp
   bulletcapsulecollider.h
   bulletcapsulecollider_c.cpp
   bulletcollider.cpp
   bulletcollider.h
   bulletcollider_c.cpp
   bulletcompoundcollider.cpp
   bulletcompoundcollider.h
   bulletcompoundcollider_c.cpp
   bulletconcavecollider.cpp
   bulletconcavecollider.h
   bulletconcavecollider_c.cpp
   bulletconecollider.cpp
   bulletconecollider.h
   bulletconecollider_c.cpp
   bulletconetwistjoint.cpp
   bulletconetwistjoint.h
   bulletconetwistjoint_c.cpp
   bulletcontactjointhandler.cpp
   bulletcontactjointhandler.h
   bulletcontactjointhandler_c.cpp
   bulletconvexcollider.cpp
   bulletconvexcollider.h
   bulletconvexcollider_c.cpp
   bulletcylindercollider.cpp
   bulletcylindercollider.h
   bulletcylindercollider_c.cpp
   bulletdynamicbody.cpp
   bulletdynamicbody.h
   bulletdynamicbody_c.cpp
   bulletemptycollider.cpp
   bulletemptycollider.h
   bulletemptycollider_c.cpp
   bulletfixedjoint.cpp
   bulletfixedjoint.h
   bulletfixedjoint_c.cpp
   bulletgeneric6dofjoint.cpp
   bulletgeneric6dofjoint.h
   bulletgeneric6dofjoint_c.cpp
   bullethinge2joint.cpp
   bullethinge2joint.h
   bullethinge2joint_c.cpp
   bullethingejoint.cpp
   bullethingejoint.h
   bullethingejoint_c.cpp
   bulletjoint.cpp
   bulletjoint.h
   bulletjoint_c.cpp
   bulletphysicsobject.cpp
   bulletphysicsobject.h
   bulletphysicsobject_c.cpp
   bulletphysicsserver.cpp
   bulletphysicsserver.h
   bulletphysicsserver_c.cpp
   bulletplanecollider.cpp
   bulletplanecollider.h
   bulletplanecollider_c.cpp
   bulletraycollider.cpp
   bulletraycollider.h
   bulletraycollider_c.cpp
   bulletrigidbody.cpp
   bulletrigidbody.h
   bulletrigidbody_c.cpp
   bulletsliderjoint.cpp
   bulletsliderjoint.h
   bulletsliderjoint_c.cpp
   bulletsoftbody.cpp
   bulletsoftbody.h
   bulletsoftbody_c.cpp
   bulletspace.cpp
   bulletspace.h
   bulletspace_c.cpp
   bulletspherecollider.cpp
   bulletspherecollider.h
   bulletspherecollider_c.cpp
   bulletstaticbody.cpp
   bulletstaticbody.h
   bulletstaticbody_c.cpp
   bullettransformcollider.cpp
   bullettransformcollider.h
   bullettransformcollider_c.cpp
   bulletuniversaljoint.cpp
   bulletuniversaljoint.h
   bulletuniversaljoint_c.cpp
   bulletworld.cpp
   bulletworld.h
   bulletworld_c.cpp
   bulletwrapper.h
)

add_library(bulletimps MODULE ${bulletimps_LIB_SRCS})

target_link_libraries(bulletimps ${BULLET_LIBRARY} ${spark_libs})

if (NOT APPLE)
   set_target_properties(bulletimps PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS bulletimps DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
