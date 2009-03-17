include_directories(${CMAKE_SOURCE_DIR}/utility ${Boost_INCLUDE_DIRS}
    ${ODE_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})


if (USE_COREFOUNDATION)
   add_definitions(-DUSE_COREFOUNDATION)
endif (USE_COREFOUNDATION)

if (USE_COREFOUNDATION)
   set(salt_require_libs  /System/Library/Frameworks/CoreFoundation.framework)
endif (USE_COREFOUNDATION)
set(zeitgeist_require_libs  salt ${salt_require_libs})
set(oxygen_require_libs zeitgeist ${zeitgeist_require_libs})
set(kerosin_require_libs  oxygen ${oxygen_require_libs})

add_subdirectory(salt)
add_subdirectory(zeitgeist)
add_subdirectory(oxygen)
add_subdirectory(kerosin)
