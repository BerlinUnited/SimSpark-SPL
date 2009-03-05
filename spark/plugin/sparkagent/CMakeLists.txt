
########### next target ###############

set(sparkagent_LIB_SRCS
   hinge2action.h
   hinge2perceptor.h
   hinge2perceptor.cpp
   hinge2perceptor_c.cpp
   hinge2effector.h
   hinge2effector.cpp
   hinge2effector_c.cpp
   timeperceptor.h
   timeperceptor.cpp
   timeperceptor_c.cpp
   export.cpp
   hingeaction.h
   hingeeffector.h
   hingeeffector.cpp
   hingeeffector_c.cpp
   hingeperceptor.h
   hingeperceptor.cpp
   hingeperceptor_c.cpp
   universaljointaction.h
   universaljointeffector.h
   universaljointeffector.cpp
   universaljointeffector_c.cpp
   universaljointperceptor.h
   universaljointperceptor.cpp
   universaljointperceptor_c.cpp
)

add_library(sparkagent MODULE ${sparkagent_LIB_SRCS})

target_link_libraries(sparkagent ${spark_libs})

set_target_properties(sparkagent PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS sparkagent DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
