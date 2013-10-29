
set(b64_SRCS
  cdecode.c
  cdecode.h
  cencode.c
  cencode.h
  )

set(base64_SRCS
  base64.cc
  )

add_library(b64 STATIC ${b64_SRCS})
if(UNIX)
  set_target_properties( b64 PROPERTIES COMPILE_FLAGS -fPIC)
endif(UNIX)

add_executable(base64 ${base64_SRCS})
target_link_libraries(base64 b64)
