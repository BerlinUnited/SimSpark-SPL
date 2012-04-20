
########### next target ###############

set(scenetest_SRCS
   main.cpp
   md5mesh.cpp
   md5mesh.h
   md5mesh_c.cpp
)

include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS}
    ${IL_INCLUDE_DIR})

if (NOT WIN32)
  add_executable(scenetest ${scenetest_SRCS})
  target_link_libraries(scenetest salt zeitgeist oxygen kerosin)
endif (NOT WIN32)


########### install files ###############




#original Makefile.am contents follow:

#if BUNDLE_SUPPORT
#all-local:
#	${top_srcdir}/macosX/mkapp scenetest
#	${top_srcdir}/macosX/fwcopy libkerosin ${top_srcdir}/lib/kerosin/.libs scenetest
#	${top_srcdir}/macosX/fwcopy liboxygen ${top_srcdir}/lib/oxygen/.libs scenetest
#	${top_srcdir}/macosX/fwcopy libzeitgeist ${top_srcdir}/lib/zeitgeist/.libs scenetest
#	${top_srcdir}/macosX/fwcopy libsalt ${top_srcdir}/lib/salt/.libs scenetest
#	${top_srcdir}/macosX/fwcopy librcssnet3D ${top_srcdir}/utility/rcssnet/.libs scenetest liboxygen
#	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/filesystemstd/.libs/filesystemstd.so scenetest
#	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/sexpparser/.libs/sexpparser.so scenetest
#	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/inputsdl/.libs/inputsdl.so scenetest
#endif
