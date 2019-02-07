
########### next target ###############

set(libobject_STAT_SRCS
   libobj.c
)

add_library(libobject STATIC ${libobject_STAT_SRCS})


#
#EXTRA_DIST = memcmp.c _vsnprintf.c
