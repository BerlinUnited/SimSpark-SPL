#include "guimainwindow.h"
#include "guimainwindowdockable.h"
#include "guimainwindowsimple.h"
#include "guicontroller.h"

#include <QtGui/QApplication>
#include <stdio.h>
#include <iostream>
#include <QDir>
#include <QObject>

void PrintGreeting()
{
   std::cout << "SimSpark GUI, controlling device for rcssserver3d, rcssmonitor3d, rcssagent3d and RoboViz.\n";
}

int main(int argc, char *argv[])
{
    PrintGreeting();
    
    int logreturning = GuiController::initLogging();
    if (logreturning == 0)
    {
        std::cerr << "Initializing Gui-Logger failed. Closing.";
        system("pause");
        return 0;
    }

    QApplication application(argc, argv);
    GuiMainWindow* gridMainWindow = 0;
    GuiMainWindowDockable* dockableMainWindow = 0;
    GuiMainWindowSimple* simpleMainWindow = 0;

    //Only one controller may be initialized.
    GuiController controller(application);
    bool success = controller.initApp(argc, argv); //Processes commandline
    if (!success)
    {
        return 0;
    }

    //Create main window with alternative layouts, they create attachment points for frames
    if (controller.getLayout() == GuiController::LAYOUT_GRID)
    {
        LOG_INFO() << "Starting GUI with Grid Layout.";
        gridMainWindow = new GuiMainWindow();
        gridMainWindow->show();
    }
    else if (controller.getLayout() == GuiController::LAYOUT_SIMPLE)
    {
        LOG_INFO() << "Starting GUI with Simple Layout.";
        simpleMainWindow = new GuiMainWindowSimple();
        simpleMainWindow->show();
    }
    else if (controller.getLayout() == GuiController::LAYOUT_DOCKABLE)
    {
        LOG_INFO() << "Starting GUI with Dockable Layout.";
        dockableMainWindow = new GuiMainWindowDockable();
        dockableMainWindow->show();
    }

    //Initialize Gui Layout
    controller.initGui();
    
    //Execute
    int returning = application.exec(); //quitting the application calls controller.close() and cleans everything up

    return returning;
}
