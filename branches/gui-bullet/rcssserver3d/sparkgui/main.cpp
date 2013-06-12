/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of the rcssserver3d carbon gui
   Author: Patrick Geib
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <carbon/abstractplugin.h>

#include <carbon/carbon.h>
#include <carbon/windowmanager.h>

#include <QtGui/QApplication>
#include <stdio.h>
#include <iostream>
#include <QDir>
#include <QObject>
#include <QLibrary>
#include <QMainWindow>

#include "sparkgui/sparkgui_config.h"

void PrintGreeting()
{
   std::cout << "Welcome to the SimSpark 'Carbon' Gui.\nAn extensible graphical user interface and simulation controlling device for SimSpark simulations.\n";
}

int main(int argc, char *argv[])
{
    PrintGreeting();

    //Set data directory
    Carbon::setUserDataFolder(QDir::homePath() + "/.sparkgui/");
    
    //Init logging
    int logreturning = Carbon::initLogging();
    if (logreturning == 0)
    {
        std::cerr << "Initializing Gui-Logger failed. Closing.";
        system("pause");
        return 1;
    }

    //Init application
    QApplication application(argc, argv);
    Q_INIT_RESOURCE(imageresources);

    //Init the controller
    Carbon carbon(application);
    bool success = carbon.initApp(argc, argv); //Processes commandline
    if (!success)
    {
        std::cerr << "Initializing application failed. Closing.";
        return 1;
    }

    //Add setup, layout and plugin include paths as well as script and executable search paths before initialization (preset definitions)
    QDir layoutDir(GUI_LAYOUT_DIRECTORY);
    QDir pluginDir1(GUI_PLUGIN_DIRECTORY_1); //plugins from spark project
	QDir pluginDir2(GUI_PLUGIN_DIRECTORY_2); //plugins from rcssserver3d project
    QDir setupDir(GUI_SETUP_DIRECTORY);
    QDir scriptDir1(SCRIPT_INCLUDE_DIRECTORY_1); //scripts from spark project
	QDir scriptDir2(SCRIPT_INCLUDE_DIRECTORY_2); //scripts from rcssserver3d project
    QDir executableDir(EXECUTABLE_INCLUDE_DIRECTORY);
    
    if (!layoutDir.exists())
    {
        LOG_ERROR() << "Could not find include directory for gui layout libraries: Path '" << GUI_LAYOUT_DIRECTORY << "' does not exist.";
        return 1;
    }
    else
        carbon.mInitialLayoutDirectories << layoutDir.absolutePath();

    if (!pluginDir1.exists())
        LOG_WARNING() << "Could not find include directory for gui plugin libraries: Path '" << GUI_PLUGIN_DIRECTORY_1 << "' does not exist.";   
    else
        carbon.mInitialPluginDirectories << pluginDir1.absolutePath();

    if (!pluginDir2.exists())
        LOG_WARNING() << "Could not find include directory for gui plugin libraries: Path '" << GUI_PLUGIN_DIRECTORY_2 << "' does not exist.";   
    else
        carbon.mInitialPluginDirectories << pluginDir2.absolutePath();
		
    if (!setupDir.exists())  
        LOG_WARNING() << "Could not find include directory for gui setup files: Path '" << GUI_SETUP_DIRECTORY << "' does not exist.";   
    else
        carbon.mInitialSetupDirectories << setupDir.absolutePath();

    if (QString(SCRIPT_INCLUDE_DIRECTORY_1).compare("") != 0)
        carbon.mInitialScriptDirectories << scriptDir1.absolutePath();
    if (QString(SCRIPT_INCLUDE_DIRECTORY_2).compare("") != 0)
	    carbon.mInitialScriptDirectories << scriptDir2.absolutePath();
    if (QString(EXECUTABLE_INCLUDE_DIRECTORY).compare("") != 0)
        carbon.mInitialExecutableDirectories << executableDir.absolutePath();

    //Load and initialize Gui layout
    success = carbon.loadLayout(); //Layout to load defined by commandline parameters
    if (!success)
    {
        std::cerr << "Loading Gui layout failed. Closing.";
        return 1;
    }
    carbon.getWindowManager()->getMainWindow()->setWindowIcon(QIcon(":spark"));
    
    //Initialize Gui elements
    carbon.initGui();
    
    //Execute
    int returning = application.exec(); //quitting the application calls carbon.close() and cleans everything up

    return returning;
    return 0;
}
