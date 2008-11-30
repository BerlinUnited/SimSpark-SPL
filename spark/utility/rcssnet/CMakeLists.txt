
########### next target ###############

set(rcssnet3D_LIB_SRCS
   addr.cpp
   exception.cpp
   handler.cpp
   socket.cpp
   tcpsocket.cpp
   udpsocket.cpp
)

add_library(rcssnet3D SHARED ${rcssnet3D_LIB_SRCS})

target_link_libraries(rcssnet3D)

set_target_properties(rcssnet3D PROPERTIES VERSION ${RCSSNET_VERSION}
    SOVERSION ${RCSSNET_SO_VERSION})
install(TARGETS rcssnet3D DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})


########### install files ###############

install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/rcssnet
          FILES_MATCHING PATTERN "*.hpp")



#original Makefile.am contents follow:

#libpkginclude_HEADERS = \
#addr.hpp \
#exception.hpp \
#handler.hpp \
#iosocketstream.hpp \
#isocketstream.hpp \
#osocketstream.hpp \
#socket.hpp \
#socketstreambuf.hpp \
#tcpsocket.hpp \
#udpsocket.hpp 
#
#librcssnet3D_la_LIBADD = @NET_LIBS@
### define include directory local to the pkgincludedir
#EXTRA_DIST = README
