
########### next target ###############

set(libobject_STAT_SRCS
   libobj.c
)

add_library(libobject STATIC ${libobject_STAT_SRCS})


########### install files ###############




#original Makefile.am contents follow:

#noinst_LTLIBRARIES = liblibobject.la
#
#liblibobject_la_SOURCES = libobj.c
#
#liblibobject_la_LIBADD = @LTLIBOBJS@
#
#EXTRA_DIST = memcmp.c _vsnprintf.c
