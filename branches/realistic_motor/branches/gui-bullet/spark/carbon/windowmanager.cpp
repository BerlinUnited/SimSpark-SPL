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

#include "windowmanager.h"
#include "carbon.h"
#include "windowdefinition.h"
#include "frameattachmentpoint.h"
#include "attachableframe.h"

#include <QtGui>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

WindowManager::WindowManager(Carbon* parent)
{
    //Default name, can be changed by the MainWindow
    mParent = parent;
}

WindowManager::~WindowManager()
{
    while (mFrameAttachmentPoints.size() > 0)
    {
        delete mFrameAttachmentPoints.begin()->second;
        mFrameAttachmentPoints.erase(mFrameAttachmentPoints.begin());
    }

    while (mWindowDefinitions.size() > 0)
    {
        delete mWindowDefinitions.begin()->second;
        mWindowDefinitions.erase(mWindowDefinitions.begin());
    }
}

bool WindowManager::hasWindow(int id)
{
    return (mWindowDefinitions.find(id) != mWindowDefinitions.end());
}

WindowDefinition* WindowManager::getWindow(int id)
{
    TWindowDefinitionMap::iterator it = mWindowDefinitions.find(id);
    if (it == mWindowDefinitions.end())
        return 0;
    return it->second;
}

void WindowManager::setMainWindow(QMainWindow* window)
{
    mMainWindow = window;
}

QMainWindow* WindowManager::getMainWindow()
{
    return mMainWindow;
}

bool WindowManager::canAttach(QString attachmentName)
{
    //Search attachment point
    FrameAttachmentPoint* attachmentPoint = findFrameAttachmentPoint(attachmentName);
    FrameAttachmentPoint* defaultAttachmentPoint = findDefaultFrameAttachmentPoint(true);
    if (attachmentPoint == 0 && defaultAttachmentPoint == 0)
        return 0;

    return (((attachmentPoint        != 0) && attachmentPoint->isAttachable()) ||
            ((defaultAttachmentPoint != 0) && defaultAttachmentPoint->isAttachable()));
}

bool WindowManager::createDefaultAttachment(AttachableFrame* frame, QString caption)
{
    LOG_DEBUG() << "Creating attachment between frame " << frame->getClassName() << " and default attachment point with caption " << caption << ".";

    FrameAttachmentPoint* attachmentPoint = findDefaultFrameAttachmentPoint(true);
    if (attachmentPoint == 0)
    {
        LOG_ERROR() << "Could not find a free default attachment point to attach frame " << caption << ".";
        return false;
    }

    //Attach frame to Widget
    bool attached = attachmentPoint->attach(frame);

    if(!attached)
    {
        LOG_ERROR() << "Attaching frame to Attachment Point failed.";
        return false;
    }

    //Create WindowDefinition
    mWindowDefinitions.insert(std::make_pair(frame->getPluginId(), new WindowDefinition(caption, frame, attachmentPoint)));

    return true;
}

bool WindowManager::createAttachment(AttachableFrame* frame, QString attachmentName, QString caption)
{
    FrameAttachmentPoint* attachmentPoint = findFrameAttachmentPoint(attachmentName);
    if (attachmentPoint == 0 || !attachmentPoint->isAttachable())
    {
        LOG_DEBUG() << "Could not find frame attachment point " << attachmentName << ". Using default attachment.";
        return createDefaultAttachment(frame, caption);
    }

    LOG_DEBUG() << "Creating attachment between frame " << frame->getClassName() << " and attachment " << attachmentName << " with caption " << caption << ".";

    //Attach frame to Widget
    bool attached = attachmentPoint->attach(frame);

    if(!attached)
    {
        LOG_ERROR() << "Attaching frame to Attachment Point failed.";
        return false;
    }

    //Create WindowDefinition
    mWindowDefinitions.insert(std::make_pair(frame->getPluginId(), new WindowDefinition(caption, frame, attachmentPoint)));

    return true;
}

bool WindowManager::changeAttachment(AttachableFrame* frame, QString attachmentName)
{
    LOG_DEBUG() << "Attempt to change attachment of frame " << frame->getCaption() << " to attachment " << attachmentName << ".";

    FrameAttachmentPoint* attachmentPoint = findFrameAttachmentPoint(attachmentName);
    if (attachmentPoint == 0)
    {
        LOG_ERROR() << "Could not find frame attachment point " << attachmentName << ".";
        return false;
    }

    FrameAttachmentPoint* oldAttachmentPoint = this->findFrameAttachmentPoint(frame);
    if (oldAttachmentPoint == 0)
    {
        LOG_ERROR() << "Could not find old attachment point.";
        return false;
    }

    //Check if source and targets differ
    if (oldAttachmentPoint->getName() == attachmentName)
    {
        return false;
    }

    //Check if attachable
    if (!attachmentPoint->isAttachable())
    {
        return false;
    }

    //Unattach old point
    oldAttachmentPoint->unattach(frame);
    auto pos = mWindowDefinitions.find(frame->getPluginId());
    delete pos->second;
    mWindowDefinitions.erase(pos);

    //Attach frame to Widget
    bool attached = attachmentPoint->attach(frame);

    if(!attached)
    {
        LOG_ERROR() << "Attaching frame to Attachment Point failed.";

        //Try reconnecting to old point
        attached = oldAttachmentPoint->attach(frame);
        if (!attached)
        {
            LOG_ERROR() << "Could not recover. Delete frame.";
            delete frame;
        }
        else
        {
            LOG_INFO() << "Recovered old state.";
        }
        return false;
    }

    //Create WindowDefinition
    mWindowDefinitions.insert(std::make_pair(frame->getPluginId(), new WindowDefinition(frame->getCaption(), frame, attachmentPoint)));

    return true;
}

FrameAttachmentPoint* WindowManager::findDefaultFrameAttachmentPoint(bool free)
{
    //Search for window definition of this frame to any attachment point
    for (TFrameAttachmentMap::iterator it = mFrameAttachmentPoints.begin(); it != mFrameAttachmentPoints.end(); it++)
    {
        if (it->second->isDefaultAttachment() && (!free || it->second->isAttachable()))
            return it->second;
    }
    return 0;
}

FrameAttachmentPoint* WindowManager::findFrameAttachmentPoint(AttachableFrame* frame)
{
    //Search for window definition of this frame to any attachment point
    for (TWindowDefinitionMap::iterator it = mWindowDefinitions.begin(); it != mWindowDefinitions.end(); it++)
    {
        if (it->second->getFrame() == frame)
        {
            return it->second->getFrameAttachmentPoint();
        }
    }
    return 0;
}

FrameAttachmentPoint* WindowManager::findFrameAttachmentPoint(QString name)
{
    for (TFrameAttachmentMap::iterator it = mFrameAttachmentPoints.begin(); it != mFrameAttachmentPoints.end(); it++)
    {
        if (it->second->getName().compare(name) == 0)
            return it->second;
    }
    return 0;
}

bool WindowManager::hasFrameAttachmentPoint(QString name)
{
    return findFrameAttachmentPoint(name) != 0;
}

WindowManager::TFrameAttachmentMap& WindowManager::getFrameAttachmentPoints()
{
    return mFrameAttachmentPoints;
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QWidget* attachmentPoint, bool defaultPoint)
{
    registerFrameAttachmentPoint(attachmentName, attachmentPoint, FrameAttachmentPoint::ATTACHMENT_WIDGET, defaultPoint);
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QFrame* attachmentPoint, bool defaultPoint)
{
    registerFrameAttachmentPoint(attachmentName, attachmentPoint, FrameAttachmentPoint::ATTACHMENT_FRAME, defaultPoint);
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QTabWidget* attachmentPoint, bool defaultPoint)
{
    registerFrameAttachmentPoint(attachmentName, attachmentPoint, FrameAttachmentPoint::ATTACHMENT_TABWIDGET, defaultPoint);
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QDockWidget* attachmentPoint, bool defaultPoint)
{
    registerFrameAttachmentPoint(attachmentName, attachmentPoint, FrameAttachmentPoint::ATTACHMENT_DOCKWIDGET, defaultPoint);
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QMainWindow* attachmentPoint, bool defaultPoint)
{
    registerFrameAttachmentPoint(attachmentName, attachmentPoint, FrameAttachmentPoint::ATTACHMENT_MAINWINDOW, defaultPoint);
}

void WindowManager::registerFrameAttachmentPoint(QString attachmentName, QWidget* attachmentPoint, FrameAttachmentPoint::EFrameAttachmentType type, bool defaultPoint)
{
    LOG_DEBUG() << "Creating FrameAttachmentPoint with name " << attachmentName << " and type " << type << ".";

    if (attachmentPoint == 0)
    {
        LOG_ERROR() << "Cannot register Null Pointer as frame attachment point.";
        return;
    }
    if (findFrameAttachmentPoint(attachmentName) != 0)
    {
        LOG_WARNING() << "Frame attachment point " << attachmentName << " already specified. Use ChangeFrameAttachmentPoint() instead.";
        return;
    }

    mFrameAttachmentPoints.insert(std::make_pair(attachmentName, 
        FrameAttachmentPoint::createFrameAttachmentPoint(attachmentName, attachmentPoint, type, defaultPoint)));
}

void WindowManager::changeToNeutralLayout(QLayout* layout)
{
    if (layout == 0) return;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
}

QLayout* WindowManager::createNeutralLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;
    changeToNeutralLayout(layout);
    return layout;
}

void WindowManager::notifyDestruction(AttachableFrame* frame)
{
    for (TWindowDefinitionMap::iterator it = mWindowDefinitions.begin(); it != mWindowDefinitions.end(); it++)
    {
        if (it->second->getFrame() == frame)
        {
            //Unattach the frame
            FrameAttachmentPoint* point = it->second->getFrameAttachmentPoint();
            LOG_DEBUG() << "Unattaching frame " << frame->getPluginId() << " (" << frame->getCaption() << ") from attachment point " << point->getName() << ".";
            point->unattach(frame);

            //Remove the frame
            delete it->second;
            mWindowDefinitions.erase(it);
            return;
        }
    }
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
