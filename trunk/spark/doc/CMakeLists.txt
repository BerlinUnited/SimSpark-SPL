add_subdirectory(devel)

find_package(Doxygen)
find_package(Perl)

if (DOXYGEN_DOT_EXECUTABLE)
  set(HAVE_DOT "YES")
else (DOXYGEN_DOT_EXECUTABLE)
  set(HAVE_DOT "NO")
endif (DOXYGEN_DOT_EXECUTABLE)

if (PERL_FOUND)
  set(PERL ${PERL_EXECUTABLE})
else (PERL_FOUND)
  set(PERL "perl")
endif (PERL_FOUND)

if (DOXYGEN_EXECUTABLE)
  # variables needed by Doxyfile.in
  set(PACKAGE ${CMAKE_PROJECT_NAME})
  set(VERSION ${PACKAGE_VERSION})
  set(top_srcdir ${CMAKE_SOURCE_DIR})
  set(top_builddir ${CMAKE_BINARY_DIR})

  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  add_custom_target(doc rm -rf api COMMAND mkdir -p api/developers
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/devel/howtos/*.* api/developers/
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile)

  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "api")

endif (DOXYGEN_EXECUTABLE)

add_custom_target(comment ALL echo '***'
  COMMAND echo '*** To \(re-\)build the Doxygen API documentation, use \"make
doc\"'
  COMMAND echo '***')

