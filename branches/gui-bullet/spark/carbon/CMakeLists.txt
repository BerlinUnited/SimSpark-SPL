########### moc target files ###############
set(carbon_QOBJECT_HEADERS
    cutelogger/imessagereceiver.h
    process.h
    pluginthread.h
    simulationprocess.h
    simulationmanager.h
    serverthread.h
    simulationsetup.h
    simulation.h
    simulationthread.h
    sparksimulationthread.h
    simulationtask.h
    taskdefinition.h
    menumanager.h
    menuobjectconnection.h
    plugininstantiatormodel.h
    plugindefinition.h
    pluginmanager.h
    attachableframe.h
    windowdefinition.h
    frameattachmentpoint.h
    numberedactionreceiver.h
    guisettings_pluginsettings.h
    guisettings_simulationsettings.h
    namedactionreceiver.h
    matrixwidget.h
    toolseparatorbutton.h
    vector3widget.h
    vector4widget.h
    vector2x3widget.h
    typedlineedit.h
    carbon.h
    settingsdialog.h
    settingswidget.h
    plugindialog.h
    settings.h
    setupdialog.h
    abstractcommand.h
    sparkcontroller.h
    sparkcommands.h
    sparktreemodel.h
    sparktreeview.h
    sparkpropertylist.h
    sparkpropertywidget.h
    sparkpropertycontrol.h
    sparkpropertyexecuter.h
    sparkpropertymanager.h
    glwidget.h
    openglmanager.h
    communicationmanager.h
    topic.h
    )
	
set(carbon_HEADERS
	cutelogger/abstractappender.h
    cutelogger/abstractstringappender.h
    cutelogger/consoleappender.h
    cutelogger/debugappender.h
    cutelogger/fileappender.h
    cutelogger/functionappender.h
    cutelogger/logger.h
    cutelogger/logmessage.h
    cutelogger/logstream.h
    cutelogger/logstreambuffer.h
    cutelogger/messageobjectappender.h
    cutelogger/messagereceiverappender.h
    cutelogger/imessagereceiver.h
    sparkprocess.h
    process.h
    pluginthread.h
    simulationprocess.h
    simulationmanager.h
    serverthread.h
    simulationsetup.h
    simulation.h
    simulationthread.h
    sparksimulationthread.h
    simulationtask.h
    taskdefinition.h
    menuobject.h
    menuobjectcall.h
    menuobjectfile.h
    menuobjectmenu.h
    menuobjectmenuaction.h
    menuobjecttoolbar.h
    menumanager.h
    menuobjectconnection.h
    plugin.h
    plugintype.h
    signalplugin.h
    abstractplugin.h
    plugininstantiatormodel.h
    plugindefinition.h
    pluginmanager.h
    attachableframe.h
    windowmanager.h
    windowdefinition.h
    frameattachmentpoint.h
    aboutdialog.h
    treeitem.h
    numberedactionreceiver.h
    guisettings_pluginsettings.h
    guisettings_simulationsettings.h
    namedactionreceiver.h
    matrixwidget.h
    toolseparatorbutton.h
    vector3widget.h
    vector4widget.h
    vector2x3widget.h
    typedlineedit.h
    libraryloader.h
    carbon.h
    settingsdialog.h
    settingswidget.h
    plugindialog.h
    settings.h
    setupdialog.h
    abstractcommandqueue.h
    guisimcontrol.h
    isimcontrol.h
    sparkcontext.h
    simspark.h
    sparkproperty.h
    sparkpropertycontrolbasic.h
    sparkpropertydata.h
    sparkpropertytablewidget.h
    abstractcommand.h
    sparkcontroller.h
    sparkcommands.h
    sparkcommandqueue.h
    sparktreemodel.h
    sparktreeview.h
    sparkpropertylist.h
    sparkpropertywidget.h
    sparkpropertycontrol.h
    sparkpropertyexecuter.h
    sparkpropertymanager.h
    inputqt.h
    glwidget.h
    openglmanager.h
    topicregistration.h
    communicationmanager.h
    topic.h
    )

set(carbon_SOURCES
	cutelogger/abstractappender.cpp
    cutelogger/abstractstringappender.cpp
    cutelogger/consoleappender.cpp
    cutelogger/debugappender.cpp
    cutelogger/fileappender.cpp
    cutelogger/functionappender.cpp
    cutelogger/imessagereceiver.cpp
    cutelogger/logger.cpp
    cutelogger/logmessage.cpp
    cutelogger/logstream.cpp
    cutelogger/logstreambuffer.cpp
    cutelogger/messageobjectappender.cpp
    cutelogger/messagereceiverappender.cpp
    pluginthread.cpp
    process.cpp
    sparkprocess.cpp
    simulationprocess.cpp
    serverthread.cpp
    simulationmanager.cpp
    simulationsetup.cpp
    simulation.cpp
    simulationtask.cpp
    simulationthread.cpp
    sparksimulationthread.cpp
    taskdefinition.cpp
    menumanager.cpp
    menuobject.cpp
    menuobjectcall.cpp
    menuobjectconnection.cpp
    menuobjectfile.cpp
    menuobjectmenu.cpp
    menuobjectmenuaction.cpp
    menuobjecttoolbar.cpp
    abstractplugin.cpp
    attachableframe.cpp
    plugininstantiatormodel.cpp
    plugindefinition.cpp
    plugin.cpp
    pluginmanager.cpp
    plugintype.cpp
    signalplugin.cpp
    frameattachmentpoint.cpp
    windowdefinition.cpp
    windowmanager.cpp
    aboutdialog.cpp
    guisettings_pluginsettings.cpp
    matrixwidget.cpp
    namedactionreceiver.cpp
    guisettings_simulationsettings.cpp
    numberedactionreceiver.cpp
    toolseparatorbutton.cpp
    treeitem.cpp
    typedlineedit.cpp
    vector2x3widget.cpp
    vector3widget.cpp
    vector4widget.cpp
    libraryloader.cpp
    carbon.cpp
    plugindialog.cpp
    settings.cpp
    settingsdialog.cpp
    settingswidget.cpp
    setupdialog.cpp
    abstractcommand.cpp
    abstractcommandqueue.cpp
    guisimcontrol.cpp
    guisimcontrol_c.cpp
    isimcontrol.cpp
    sparkcontext.cpp
    sparkcontroller.cpp
    sparkcommandqueue.cpp
    sparkcommands.cpp
    simspark.cpp
    sparkproperty.cpp
    sparkpropertycontrol.cpp
    sparkpropertycontrolbasic.cpp
    sparkpropertydata.cpp
    sparkpropertyexecuter.cpp
    sparkpropertylist.cpp
    sparkpropertymanager.cpp
    sparkpropertytablewidget.cpp
    sparkpropertywidget.cpp
    sparktreemodel.cpp
    sparktreeview.cpp
    glwidget.cpp
    inputqt.cpp
    openglmanager.cpp
    communicationmanager.cpp
    topic.cpp
    topicregistration.cpp
    )

set(carbon_FORMS
    guisettings_simulationsettings.ui
    matrixwidget.ui
    guisettings_pluginsettings.ui
    about.ui
    plugindialog.ui
    settingsdialog.ui
    setupdialog.ui
    sparkpropertywidget.ui
    vector3widget.ui
    vector4widget.ui
    )

set(carbon_RESOURCES
	imageresources.qrc
    )

########### Qt properties ###############	

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})
	
########### includes and library creation ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(carbon_QOBJECT_HEADERS_MOC ${carbon_QOBJECT_HEADERS})
QT4_WRAP_UI(carbon_FORMS_HEADERS ${carbon_FORMS})
QT4_ADD_RESOURCES(carbon_RESOURCES_RCC ${carbon_RESOURCES})

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/lib 
	${CMAKE_SOURCE_DIR}/utility
	${FREETYPE_INCLUDE_DIRS}
	${IL_INCLUDE_DIR} 
	${OPENGL_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}) #intermediate directory

add_library(carbon STATIC
	${carbon_SOURCES} 
	${carbon_QOBJECT_HEADERS_MOC}
	${carbon_HEADERS}
	${carbon_FORMS_HEADERS}
	${carbon_RESOURCES_RCC}
	)

target_link_libraries(carbon 
	${FREETYPE_LIBRARIES} 
	${IL_LIBRARIES}
    ${OPENGL_LIBRARIES} 
	${QT_LIBRARIES}
	spark salt zeitgeist oxygen kerosin
	)

add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	
set_target_properties(carbon PROPERTIES VERSION ${CARBON_VERSION}
    SOVERSION ${CARBON_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS carbon DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/carbon-config.in ${CMAKE_CURRENT_BINARY_DIR}/carbon-config)

########### install files ###############

#install header files from actual directory
install(DIRECTORY ./ 
		DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/carbon
        FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
		
#install generated header files (ui_*.h files) 
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
		DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/carbon
        FILES_MATCHING PATTERN "*.h" 
		PATTERN ".svn" EXCLUDE
		PATTERN "carbon.dir" EXCLUDE
		PATTERN "CMakeFiles" EXCLUDE
		PATTERN "Debug" EXCLUDE
		PATTERN "Release" EXCLUDE)		
