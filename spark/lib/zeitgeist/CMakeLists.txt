########### target ###############
set(zeitgeist_LIB_HDRS
    class.h
    core.h
    corecontext.h
    leaf.h
    parameterlist.h
    node.h
    object.h
    object_c.h
    zeitgeist.h
    fileserver/fileserver.h
    fileserver/filesystem.h
    logserver/logserver.h
    logserver/logserverstreambuf.h
    randomserver/randomserver.h
    scriptserver/gcvalue.h
    scriptserver/scriptserver.h
    scriptserver/rubywrapper.h 
    
    #telnetserver/telnetdaemon.h
    #telnetserver/telnetserver.h
    #telnetserver/telnetsession.h
)
    
set(zeitgeist_LIB_SRCS
    class.cpp
    class_c.cpp
    core.cpp
    corecontext.cpp
    leaf.cpp
    leaf_c.cpp
    parameterlist.cpp
    node.cpp
    node_c.cpp
    object.cpp
    object_c.cpp
    zeitgeist.cpp
    fileserver/fileserver.cpp
    fileserver/fileserver_c.cpp
    fileserver/filesystem_c.cpp
    logserver/logserver.cpp
    logserver/logserver_c.cpp
    logserver/logserverstreambuf.cpp
    randomserver/randomserver.cpp
    randomserver/randomserver_c.cpp
    scriptserver/gcvalue.cpp
    scriptserver/rubywrapper.cpp
    scriptserver/scriptserver.cpp
    scriptserver/scriptserver_c.cpp
    #telnetserver/telnetdaemon.cpp
    #telnetserver/telnetserver.cpp
    #telnetserver/telnetserver_c.cpp
    #telnetserver/telnetsession.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/lib ${RUBY_INCLUDE_PATH})

add_library(zeitgeist SHARED ${zeitgeist_LIB_SRCS} ${zeitgeist_LIB_HDRS})

target_link_libraries(zeitgeist ${RUBY_LIBRARY})

set_target_properties(zeitgeist PROPERTIES VERSION ${ZEITGEIST_VERSION}
    SOVERSION ${ZEITGEIST_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS zeitgeist DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/zeitgeist-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/zeitgeist-config)

########### install files ###############

install(FILES  zeitgeist.rb DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/zeitgeist-config 
        DESTINATION ${BINDIR} PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/zeitgeist
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)


