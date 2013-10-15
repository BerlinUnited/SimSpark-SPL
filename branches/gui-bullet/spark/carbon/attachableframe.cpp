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

#include "attachableframe.h"
#include "windowmanager.h"
#include "menumanager.h"
#include "carbon.h"

#include <iostream>
#include <stdio.h>
#include <QLayout>
#include <vector>

//--------------------------------------------------------------
// Static 
//--------------------------------------------------------------

//--------------------------------------------------------------
// AttachableFrame
//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

AttachableFrame::AttachableFrame() :
    QFrame(0),
    AbstractPlugin(PT_FRAME)
{
    mHasVisualRepresentation = true;
    mIsQObject = true;
    setExecutionType(ET_NEVER);
}

AttachableFrame::~AttachableFrame()
{
    //Delete all menu entries that have been created by this frame
    Carbon::get()->getMenuManager()->cleanEntries(this);
}

void AttachableFrame::init(const QStringList& parameters)
{
    //do nothing
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void AttachableFrame::loadData(const QSettings& data)
{
    LOG_DEBUG() << "No data to load...";
}

void AttachableFrame::saveData(QSettings& data) const
{
    LOG_DEBUG() << "No data to save...";
}

QWidget* AttachableFrame::getVisualRepresentation() const
{
    return (QWidget*)this;
}

QObject* AttachableFrame::getQObject() const
{
    return (QObject*)this;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void AttachableFrame::loadFrameDesign()
{
    //Load basic frame design for all frames attached to a FrameAttachmentpoint

    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
}

bool AttachableFrame::run()
{
    LOG_ERROR() << "Function run() not implemented in AttachableFrame " << getClassName() << 
        ". Add the function or specify another execution type to prevent it from being called.";

    return false;
}

bool AttachableFrame::stop()
{
    LOG_ERROR() << "Function stop() not implemented in AttachableFrame " << getClassName() << 
        ". Add the function or specify another execution type to prevent it from being called.";

    return false;
}

bool AttachableFrame::pause(bool state)
{
    LOG_ERROR() << "Function pause() not implemented in AttachableFrame " << getClassName() << 
        ". Add the function or specify another execution type to prevent it from being called.";

    return false;
}

bool AttachableFrame::restart()
{
    LOG_ERROR() << "Function reset() not implemented in AttachableFrame " << getClassName() << 
        ". Add the function or specify another execution type to prevent it from being called.";

    return false;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void AttachableFrame::logDebug(QString text)
{
    LOG_DEBUG() << text;
}

void AttachableFrame::logInfo(QString text)
{
    LOG_INFO() << text;
}

void AttachableFrame::logWarning(QString text)
{
    LOG_WARNING() << text;
}

void AttachableFrame::logError(QString text)
{
    LOG_ERROR() << text;
}

void AttachableFrame::logFatal(QString text)
{
    LOG_FATAL() << text;
}

void AttachableFrame::sendMessage(int port, QString message)
{
    LOG_INFO() << "TODO: Send Message " << message << " to port " << port << ".";
}

//--------------------------------------------------------------
// AttachableFrameInstantiator
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

AttachableFrameInstantiator::AttachableFrameInstantiator(int id, const QString& name, const QString& caption, AttachableFrame* (*instantiateFunc)(void),
                                                         int maxInstanceCount, const char* library) :
    AbstractPluginInstantiator(id, name, caption, PT_FRAME, maxInstanceCount, library),
    mInstantiateFunc(instantiateFunc)
{

}

AttachableFrameInstantiator::~AttachableFrameInstantiator()
{

}

AttachableFrame* AttachableFrameInstantiator::instantiate(const QString& caption) const
{
    AttachableFrame* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(caption);
    return newPlugin;
}

AttachableFrame* AttachableFrameInstantiator::instantiate() const
{
    AttachableFrame* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(mDefaultCaption);
    return newPlugin;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------
