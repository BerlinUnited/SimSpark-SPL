
########### install files ###############

install(DIRECTORY fonts ros rsg scripts 
    DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME}
          PATTERN ".svn" EXCLUDE)
