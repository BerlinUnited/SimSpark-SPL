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

#include "guisimplelayout.h"

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

GuiSimpleLayout::GuiSimpleLayout(QWidget *parent, Qt::WFlags flags)
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
    mLeftTabWidget = new QTabWidget;
    QVBoxLayout *controlFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(controlFrameLayout);
    controlFrameLayout->addWidget(mLeftTabWidget);
    ui.leftFrame->setLayout(controlFrameLayout);
    mLeftTabWidget->show();
    windowManager->registerFrameAttachmentPoint("left_attachment", mLeftTabWidget); 

    //Content frames (right_attachment)
    mRightTabWidget = new QTabWidget;
    QVBoxLayout *editorFrameLayout = new QVBoxLayout;
    WindowManager::changeToNeutralLayout(editorFrameLayout);
    editorFrameLayout->addWidget(mRightTabWidget);
    ui.rightFrame->setLayout(editorFrameLayout);
    mRightTabWidget->show();
    windowManager->registerFrameAttachmentPoint("right_attachment", mRightTabWidget, true); // flag this as default attachment 
}

GuiSimpleLayout::~GuiSimpleLayout()
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
