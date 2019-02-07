########### install files ###############

install(DIRECTORY rsg ./ 
    DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME}
          PATTERN ".svn" EXCLUDE)
