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

#include "frameattachmentpoint.h"
#include "attachableframe.h"
#include "carbon.h"
#include "windowmanager.h"
#include "numberedactionreceiver.h"
#include "pluginmanager.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QFrame>
#include <QTabWidget>
#include <QDockWidget>
#include <QMainWindow>

//--------------------------------------------------------------
// FrameAttachmentPoint
//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

FrameAttachmentPoint* FrameAttachmentPoint::createFrameAttachmentPoint(QString name, QWidget* attachmentPoint, EFrameAttachmentType type, bool defaultPoint)
{
    //Create point
    FrameAttachmentPoint* point;
    switch (type)
    {
    case ATTACHMENT_NONE:               
        LOG_ERROR() << "Illegal FrameAttachmentType 'NONE'. Using QWidget instead."; 
        //fall 
    case ATTACHMENT_WIDGET:             //fall    
    case ATTACHMENT_FRAME:              //fall    
    case ATTACHMENT_TABWIDGET:          //fall    
    case ATTACHMENT_DOCKWIDGET:         //fall
    case ATTACHMENT_MAINWINDOW:         
        point = new FrameAttachmentPoint(); 
        break;
    case ATTACHMENT_DOCKWIDGET_CREATOR: 
        point = new ExtendingFrameAttachmentPoint(ExtendingFrameAttachmentPoint::ET_DOCKWIDGET); 
        break;
    default:
        LOG_ERROR() << "Unknown FrameAttachmentType " << type << ". Assuming QWidget.";
        point = new FrameAttachmentPoint(); 
        break; 
    }
    
    //Init point
    point->initialize(name, attachmentPoint, type, defaultPoint);
    return point;
}

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

FrameAttachmentPoint::FrameAttachmentPoint()
{
}

FrameAttachmentPoint::~FrameAttachmentPoint()
{
    clearActionReceivers();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

QString FrameAttachmentPoint::getName()
{
    return mName;
}

QWidget* FrameAttachmentPoint::getAttachmentPoint()
{
    return mAttachmentPoint;
}

FrameAttachmentPoint::EFrameAttachmentType FrameAttachmentPoint::getEFrameAttachmentType()
{
    return mType;
}

int FrameAttachmentPoint::getAttachmentCount()
{
    return mAttachedFrames.size();
}

int FrameAttachmentPoint::getMaxAttachmentCount()
{
    return mMaxAttachmentCount;
}

bool FrameAttachmentPoint::isAttachable()
{
    return mMaxAttachmentCount > mAttachedFrames.size();
}

bool FrameAttachmentPoint::isDefaultAttachment()
{
    return mDefaultPoint;
}

bool FrameAttachmentPoint::attach(AttachableFrame* frame)
{
    QLayout* layout;
    QWidget* attWidget;
    QFrame* attFrame;
    QDockWidget* attDockWidget;
    QTabWidget* attTabWidget;
    QMainWindow* attMainWindow;
    int index = 0;

    if ((int)mAttachedFrames.size() >= mMaxAttachmentCount)
    {
        LOG_ERROR() << "FrameAttachmentPoint::attach(): cannot attach more than " << mMaxAttachmentCount <<
                       " Frame" << (mMaxAttachmentCount > 1 ? "s" : "") << " to Attachment Point " << mName << ".";
       return false;
    }

    //Attach the frame in a way depending on its type
    switch (mType)
    {
    case ATTACHMENT_NONE:
         LOG_ERROR() << "FrameAttachmentPoint::attach(): cannot attach to illegal Attachment Point.";
         return false;
        break;
    case ATTACHMENT_WIDGET:
        //Attach to Widget with adding a new layout
        layout = WindowManager::createNeutralLayout();
        layout->addWidget(&*frame);
        frame->show();
        attWidget = (dynamic_cast<QWidget*>(mAttachmentPoint));
        attWidget->setLayout(layout);
        attWidget->show();
        break;
    case ATTACHMENT_FRAME:
        //Attach to Frame with adding a new layout
        layout = WindowManager::createNeutralLayout();
        layout->addWidget(&*frame);
        frame->show();
        attFrame = (dynamic_cast<QFrame*>(mAttachmentPoint));
        attFrame->setLayout(layout);
        attFrame->show();
        break;
    case ATTACHMENT_TABWIDGET:
        //Attach to TabWidget with adding a new page and (automatically) a layout
        attTabWidget = (dynamic_cast<QTabWidget*>(mAttachmentPoint));
        index = attTabWidget->addTab(&*frame, frame->getCaption());
        WindowManager::changeToNeutralLayout(attTabWidget->widget(index)->layout());
        break;
    case ATTACHMENT_DOCKWIDGET:
        //Attach to DockWidget with adding a new layout
        layout = WindowManager::createNeutralLayout();
        layout->addWidget(&*frame);
        frame->show();
        attDockWidget = (dynamic_cast<QDockWidget*>(mAttachmentPoint));
        attDockWidget->setLayout(layout);
        attDockWidget->show();
        break;
    case ATTACHMENT_MAINWINDOW:
        //Attach to MainWindow with adding a new layout
        layout = WindowManager::createNeutralLayout();
        layout->addWidget(&*frame);
        frame->show();
        attMainWindow = (dynamic_cast<QMainWindow*>(mAttachmentPoint));
        attMainWindow->setLayout(layout);
        attMainWindow->show();
        break;
    default:
         LOG_ERROR() << "FrameAttachmentPoint::attach(): unknown Attachment Point.";
        return false;
        break;
    }

    //Store the attachment pointer
    mAttachedFrames.insert(frame->getPluginId(), frame);

    return true;
}

void FrameAttachmentPoint::unattach(AttachableFrame* frame)
{
    for (auto it = mAttachedFrames.begin(); it != mAttachedFrames.end(); it++)
    {
        if (*it == frame)
        {
            //found frame, remove

            //Most of the attachment points handle deletion of the widget by an external source automatically
            QWidget* widget = (QWidget*)frame;
            switch (mType)
            {
            case ATTACHMENT_NONE:
                 LOG_ERROR() << "Cannot unattach from illegal Attachment Point.";
                break;
            case ATTACHMENT_WIDGET:
            case ATTACHMENT_FRAME:
            case ATTACHMENT_TABWIDGET:
            case ATTACHMENT_DOCKWIDGET:
            case ATTACHMENT_MAINWINDOW:
                break;
            default:
                 LOG_ERROR() << "Cannot unattach from unknown Attachment Point.";
                break;
            }

            mAttachedFrames.erase(it);
            return;
        }
    }
    
    LOG_DEBUG() << "Frame to unattach not found.";
}

void FrameAttachmentPoint::clearActionReceivers()
{
    while (mActionReceivers.size() > 0)
    {
        mActionReceivers.back()->deleteLater();
        mActionReceivers.pop_back();
    }
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//Constructor for derived creator-attachmentpoints
FrameAttachmentPoint::FrameAttachmentPoint(EFrameAttachmentType type, QString name, QWidget* parentWidget, bool defaultPoint)
{
    initialize(name, parentWidget, type, defaultPoint);
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void FrameAttachmentPoint::initialize(QString name, QWidget* attachmentPoint, EFrameAttachmentType type, bool defaultPoint)
{
    //Called by constructor

    mName = name;
    if (attachmentPoint == 0)
        LOG_ERROR() << "Handed 0-pointer to FrameAttachmentPoint constructor.";
    mAttachmentPoint = attachmentPoint;
    mType = type;
    mDefaultPoint = defaultPoint;

    switch(type)
    {
    case ATTACHMENT_NONE:
        mMaxAttachmentCount = 0;
        LOG_ERROR() << "Initializing with undefined type ATTACHMENT_NONE.";
        break;
    case ATTACHMENT_WIDGET: mMaxAttachmentCount = 1; break;
    case ATTACHMENT_FRAME: mMaxAttachmentCount = 1; break;
    case ATTACHMENT_TABWIDGET: mMaxAttachmentCount = INT_MAX; break;
    case ATTACHMENT_DOCKWIDGET: mMaxAttachmentCount = 1; break;
    case ATTACHMENT_MAINWINDOW: mMaxAttachmentCount = 1; break;
    default:
        mMaxAttachmentCount = 0;
        LOG_ERROR() << "Initializing with illegal type.";
        break;
    }

    if (mMaxAttachmentCount == INT_MAX)
    {
        attachmentPoint->setToolTip("Attachment point for Gui plugins.");
        attachmentPoint->setWhatsThis("Attachment point for many Gui plugins. You can create a plugin from the plugin dialog or the toolbar.\n" \
            "Right clicking opens a context menu to move, copy or delete the plugin.");
    }
    else
    {
        attachmentPoint->setToolTip("Attachment point for " + QString::number(mMaxAttachmentCount) + " Gui plugins.");
        attachmentPoint->setWhatsThis("Attachment point for " + QString::number(mMaxAttachmentCount) + " Gui plugins. " \
            "You can create a plugin from the plugin dialog or the toolbar.\n" \
            "Right clicking opens a context menu to move, copy or delete the plugin.");
    }
    attachmentPoint->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(attachmentPoint, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void FrameAttachmentPoint::showContextMenu(QPoint point)
{
    //Show menus with all attached frames and submenu where to attach it to
    clearActionReceivers();

    WindowManager* windowManager = Carbon::get()->getWindowManager();
    QMenu menu(windowManager->getMainWindow());
    QMenu* submenu;
    QAction* action;

    //For all attached frames, add frame menu
    int frame = 0;
    for (auto it = mAttachedFrames.begin(); it != mAttachedFrames.end(); it++) 
    {
        submenu = menu.addMenu((*it)->getCaption());
     
        //for all possible targets, add commands
        {
            NumberedActionReceiver* receiver;
            
            //Copy command
            receiver = new NumberedActionReceiver(frame, COMMAND_COPY);
            mActionReceivers.push_back(receiver);
            connect(receiver, SIGNAL(actionReceived(int,int)), this, SLOT(onContextMenuClick(int,int)));
            action = submenu->addAction(QIcon(":copy"), "Copy", receiver, SLOT(receiveAction()));
            action->setToolTip("Create a copy of the plugin.");
            if (!PluginFactory::getFactory().canCreatePlugin((*it)->getClassId())) //Disable copy if plugin cant be created anymore
                action->setEnabled(false);

            //Delete command
            receiver = new NumberedActionReceiver(frame, COMMAND_DELETE);
            mActionReceivers.push_back(receiver);
            connect(receiver, SIGNAL(actionReceived(int,int)), this, SLOT(onContextMenuClick(int,int)));
            action = submenu->addAction(QIcon(":delete"), "Delete", receiver, SLOT(receiveAction()));
            action->setToolTip("Delete the plugin.");
        }

        submenu->addSeparator();

        //for all possible targets, add target frames
        int target = COMMAND_MAX_ID;
        for (auto tar_it = windowManager->getFrameAttachmentPoints().begin(); tar_it != windowManager->getFrameAttachmentPoints().end(); tar_it++)
        {
            NumberedActionReceiver* receiver = new NumberedActionReceiver(frame, target);
            mActionReceivers.push_back(receiver);

            connect(receiver, SIGNAL(actionReceived(int,int)), this, SLOT(onContextMenuClick(int,int)));

            if (tar_it->second == this)
                action = submenu->addAction(QIcon(":accept"), tar_it->second->getName(), receiver, SLOT(receiveAction()));
            else
                action = submenu->addAction(tar_it->second->getName(), receiver, SLOT(receiveAction()));
            
            target++;
        }
        frame++;
    }

    menu.exec(mAttachmentPoint->mapToGlobal(point));
}

void FrameAttachmentPoint::onContextMenuClick(int frame, int command)
{
    if (frame >= 0 && frame < mAttachedFrames.size())
    {
        auto framePos = mAttachedFrames.begin();
        for (int i = 0; i < frame; i++) framePos++;

        //Check for command
        if (command == COMMAND_COPY)
        {
            //Copy the plugin
            PluginManager* manager = Carbon::get()->getPluginManager();
            AbstractPlugin* plugin = manager->createPlugin(*(*framePos)->getPluginDefinition(), true);
        }
        else if (command == COMMAND_DELETE)
        {
            //Delete the plugin
            PluginManager* manager = Carbon::get()->getPluginManager();
            manager->deletePlugin((*framePos)->getPluginId(), true);
        }
        else if (command >= COMMAND_MAX_ID)
        {
            //Reattach the plugin
            int target = command - COMMAND_MAX_ID;
            WindowManager* manager = Carbon::get()->getWindowManager();
            if ((int)manager->getFrameAttachmentPoints().size() > target && target >= 0)
            {
                auto pos = manager->getFrameAttachmentPoints().begin(); 
                for (int i = 0; i < target; i++) pos++;

                manager->changeAttachment(*framePos, pos->second->getName());
            }
        }
    }
    else
    {
        LOG_ERROR() << "Illegal frame index " << frame;
    }
}

//--------------------------------------------------------------
// ExtendingFrameAttachmentPoint
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

ExtendingFrameAttachmentPoint::ExtendingFrameAttachmentPoint(EExtendingType type)
    : mExtendingType(type)
{
}

ExtendingFrameAttachmentPoint::~ExtendingFrameAttachmentPoint()
{
}

bool ExtendingFrameAttachmentPoint::attach(AttachableFrame* frame)
{
    int index = 0;

    if ((int)mAttachedFrames.size() >= mMaxAttachmentCount)
    {
        LOG_ERROR() << "FrameAttachmentPoint::attach(): cannot attach more than " << mMaxAttachmentCount <<
                       " Frame" << (mMaxAttachmentCount > 1 ? "s" : "") << " to Attachment Point " << mName << ".";
       return false;
    }

    //Attach the frame in a way depending on its type
    if (mType == ATTACHMENT_DOCKWIDGET_CREATOR)
    {
        //Create new dockwidget and attach frame to it
        QDockWidget* newWidget = new QDockWidget(mName, mAttachmentPoint, Qt::WindowMinMaxButtonsHint);
        mWidgetMap.insert(frame, newWidget);

        //QLayout* layout = WindowManager::createNeutralLayout();
        //newWidget->setLayout(layout);
        //WindowManager::changeToNeutralLayout(newWidget->layout());
        newWidget->setWidget(frame);
        //newWidget->layout()->addWidget(frame);
        newWidget->show();
        frame->show();
    }
    else
    {
        LOG_ERROR() << "ExtendingFrameAttachmentPoint::attach(): unknown Attachment Point.";
        return false;
    }

    //Store the attachment pointer
    mAttachedFrames.insert(frame->getPluginId(), frame);

    return true;
}

void ExtendingFrameAttachmentPoint::unattach(AttachableFrame* frame)
{
    if (mType == ATTACHMENT_DOCKWIDGET_CREATOR)
    {
        //Search parent dockwidget
        auto pos = mWidgetMap.find(frame);

        if (pos == mWidgetMap.end())
        {
            LOG_ERROR() << "Cannot unattach AttachableFrame. Parent Dockwidget was not found.";
            return;
        }

        //Remove
        (*pos)->deleteLater();
        mWidgetMap.erase(pos);
    }
    else
    {
        LOG_ERROR() << "Cannot unattach from unknown Attachment Point.";
        return;
    }

    //Erase from list
    bool found = false;
    for (auto it = mAttachedFrames.begin(); it != mAttachedFrames.end(); it++)
    {
        if (*it == frame)
        {
            //found frame, remove
            mAttachedFrames.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
        LOG_DEBUG() << "Frame to unattach not found.";
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void ExtendingFrameAttachmentPoint::initialize(QString name, QWidget* attachmentPoint, EFrameAttachmentType type, bool defaultPoint)
{
    //Called by constructor

    mName = name;
    if (attachmentPoint == 0)
        LOG_ERROR() << "Handed 0-pointer to ExtendingFrameAttachmentPoint constructor.";
    mAttachmentPoint = attachmentPoint;
    mType = type;
    mDefaultPoint = defaultPoint;

    switch(type)
    {
    case ATTACHMENT_DOCKWIDGET_CREATOR:
        mMaxAttachmentCount = INT_MAX;
        break;
    default:
        mMaxAttachmentCount = 0;
        LOG_ERROR() << "Initializing with illegal type.";
        break;
    }
}