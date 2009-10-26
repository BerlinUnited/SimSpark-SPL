########### next target ###############

set(sexp_STAT_SRCS
   cstring.c
   cstring.h
   faststack.c
   faststack.h
   io.c
   malloc_util.h
   parser.c
   sexp.c
   sexp.h
   sexp_ops.c
   sexp_ops.h
)

add_library(sexp STATIC ${sexp_STAT_SRCS})
if(UNIX)
set_target_properties( sexp PROPERTIES COMPILE_FLAGS -fPIC)
endif(UNIX)
