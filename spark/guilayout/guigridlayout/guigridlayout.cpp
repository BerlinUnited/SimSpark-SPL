/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
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

#include "guigridlayout.h"

#include <carbon.h>
#include <windowmanager.h>
#include <menumanager.h>
#include <simulationmanager.h>

#include "export.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

GuiGridLayout::GuiGridLayout(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    WindowManager* windowManager = Carbon::get()->getWindowManager();

    //Define default plugin setup document (.xml-File) for this layout
    Carbon::get()->getSimulationManager()->setDefaultSetup("DefaultSetupGrid");

    //Setup Ui Layout
    ui.setupUi(this);

    //########## Create Attachmentpoints for default and custom Widgets ##########//
    //The WindowManager will attach Windows to these Attachmentpoints in a way depending on which type they are.

    //Control Frames (left_attachment)
    leftTabWidget = new QTabWidget;
    QVBoxLayout *controlFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(controlFrameLayout);
    controlFrameLayout->addWidget(leftTabWidget);
    ui.controlFrame->setLayout(controlFrameLayout);
    leftTabWidget->show();
    windowManager->registerFrameAttachmentPoint("left_attachment", leftTabWidget, true); // flag this as default attachment 

    //DisplayFrames (center_attachment)
    centerTabWidget = new QTabWidget;
    QVBoxLayout *monitorFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(monitorFrameLayout);
    monitorFrameLayout->addWidget(centerTabWidget);
    ui.monitorFrame->setLayout(monitorFrameLayout);
    centerTabWidget->show();
    windowManager->registerFrameAttachmentPoint("center_attachment", centerTabWidget);

    //Log and dialog frames (bottom_attachment)
    bottomTabWidget = new QTabWidget;
    QVBoxLayout *consoleFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(consoleFrameLayout);
    consoleFrameLayout->addWidget(bottomTabWidget);
    ui.consoleFrame->setLayout(consoleFrameLayout);
    bottomTabWidget->show();
    windowManager->registerFrameAttachmentPoint("bottom_attachment", bottomTabWidget);

    //Second control frames (right_attachment)
    rightTabWidget = new QTabWidget;
    QVBoxLayout *editorFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(editorFrameLayout);
    editorFrameLayout->addWidget(rightTabWidget);
    ui.editorFrame->setLayout(editorFrameLayout);
    rightTabWidget->show();
    windowManager->registerFrameAttachmentPoint("right_attachment", rightTabWidget);
}

GuiGridLayout::~GuiGridLayout()
{
    //QObjects are deleted automatically
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------
