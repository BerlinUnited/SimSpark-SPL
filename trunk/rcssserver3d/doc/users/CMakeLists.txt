find_package(LATEX)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/user-manual.tex)
    if (PDFLATEX_COMPILER)
      include(../../cmake/UseLATEX.cmake)
      add_latex_document(user-manual.tex INPUTS agentspark.tex gettingstarted.tex
        robots.tex introduction.tex monitorspark.tex simspark.tex license.tex
        overview.tex simulations.tex definitions.tex resources.tex user-manual.tex
        BIBFILES references.bib IMAGE_DIRS fig DEFAULT_PDF)
    endif (PDFLATEX_COMPILER)
else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/user-manual.tex)
    add_custom_target(comment2 ALL echo '***'
      COMMAND echo '*** The users manual \(PDF\) can be found in doc/users.'
      COMMAND echo '***')
endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/user-manual.tex)

set(CLEAN_FILES
   definitions.aux 
   user-manual.log 
   user-manual.pdf
   user-manual.aux
   user-manual.toc
   user-manual.out
   user-manual.bbl
   user-manual.blg
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES}")
