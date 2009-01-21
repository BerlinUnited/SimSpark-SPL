find_package(LATEX)

if (PDFLATEX_COMPILER AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex)
  include(../../cmake/UseLATEX.cmake)
  add_latex_document(manual.tex INPUTS conventions.tex  introduction.tex
      rsg.tex  definitions.tex  manual.tex zeitgeist.tex DEFAULT_PDF)
endif (PDFLATEX_COMPILER AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/manual.tex)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
      "definitions.aux;manual.log;manual.pdf;manual.aux;manual.out;manual.toc")
