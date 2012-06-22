########### install files ###############

install(DIRECTORY models scripts materials rsg textures
    DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME}
          PATTERN ".svn" EXCLUDE)