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

#include "openglmanager.h"
#include "carbon.h"
#include "glwidget.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

int OpenGLManager::mNextFlag = OpenGLManager::GWF_FLAG_0;

QString OpenGLManager::getErrorMessage(int error)
{
    switch (error)
    {
        case GL_NO_ERROR:          return "OpenGL Error: GL_NO_ERROR"; break;
        case GL_INVALID_ENUM:      return "OpenGL Error: GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:     return "OpenGL Error: GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: return "OpenGL Error: GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:    return "OpenGL Error: GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:   return "OpenGL Error: GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:     return "OpenGL Error: GL_OUT_OF_MEMORY"; break;
        //case GL_TABLE_TOO_LARGE:   return "OpenGL Error: GL_TABLE_TOO_LARGE"; break;
        default:
            return "OpenGL Error: Unknown Error";
            break;
    }
}

std::string OpenGLManager::getErrorMessageStd(int error)
{
    return getErrorMessage(error).toStdString();
}

int OpenGLManager::getUniqueFlag()
{
    int returning = mNextFlag;
    mNextFlag += OpenGLManager::GWF_FLAG_0;
    return returning;
}

void OpenGLManager::updateFlagUsed(int flag)
{
    //Increase to next pure flag combination (without reserved flags)
    if (flag >= mNextFlag)
    {
        mNextFlag = (flag / GWF_FLAG_0) * GWF_FLAG_0;
        mNextFlag = mNextFlag + GWF_FLAG_0;
    }
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

OpenGLManager::OpenGLManager(Carbon* parent) :
    mParent(parent),
    mWidgetMutex(QMutex::Recursive)
{
    mWidgets.clear();
}

OpenGLManager::~OpenGLManager()
{

}

int OpenGLManager::getGLWidgetFlags(GLWidget* widget)
{
    if (!lockWidgets())
        return GWF_UNDEFINED;

    int ret = GWF_UNDEFINED;
    for (auto it = mWidgets.begin(); it != mWidgets.end(); )
    {
        if (it->mWidget == widget)
        {
            ret = it->mFlags;
            break;
        }
    }

    unlockWidgets();
    return ret;
}

bool OpenGLManager::hasGLWidgetFlag(GLWidget* widget, EGLWidgetFlags flag)
{
    if (!lockWidgets())
        return false;

    bool ret = false;
    for (auto it = mWidgets.begin(); it != mWidgets.end(); )
    {
        if (it->mWidget == widget)
        {
            if ((it->mFlags & flag) == flag)
                ret = true;
            break;
        }
    }

    unlockWidgets();
    return ret;
}

bool OpenGLManager::hasWidget(GLWidget* widget)
{
    if (!lockWidgets())
        return false;

    for (auto it = mWidgets.begin(); it != mWidgets.end(); it++)
    {
        if (it->mWidget == widget)
        {
            unlockWidgets();
            return true;
        }
    }

    unlockWidgets();
    return false;
}

bool OpenGLManager::registerGLWidget(GLWidget* widget, int flags)
{
    if (!lockWidgets())
        return false;

    bool returning = false;
    if (flags == GWF_UNDEFINED)
    {
        LOG_ERROR() << "Cant register 'undefined' GLWidget";
    }
    else
    {
        for (auto it = mWidgets.begin(); it != mWidgets.end(); it++)
        {
            if (it->mWidget == widget)
            {
                //Change registration
                it->mFlags = flags;
                LOG_DEBUG() << "Changed GLWidget registration flags to " << flags;
                updateFlagUsed(flags);
                returning = true;
                break;
            }
        }
        if (returning == false)
        {
            //Add registration
            mWidgets.push_back(GLHandle(widget, flags));
            connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(glWidgetDestroyed(QObject*)));
            updateFlagUsed(flags);
            returning = true;
            LOG_DEBUG() << "Registered GLWidget with flags " << flags;
        }
    }

    unlockWidgets();
    return returning;

}

bool OpenGLManager::registerGLWidget(GLWidget* widget, EGLWidgetFlags flags)
{
    return registerGLWidget(widget, (int) flags);
}

bool OpenGLManager::unregisterGLWidget(GLWidget* widget)
{
    if (!lockWidgets())
        return false;

    for (auto it = mWidgets.begin(); it != mWidgets.end(); )
    {
        if (it->mWidget == widget)
        {
            LOG_DEBUG() << "Unregistering GLWidget " << widget->getName();
            disconnect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(glWidgetDestroyed(QObject*)));
            it = mWidgets.erase(it);
            unlockWidgets();
            return true;
        }
        else
            it++;
    }

    unlockWidgets();
    return false;
}

GLWidget* OpenGLManager::getRegisteredWidget(int flags)
{
    bool dontcareunique = false;
    return getRegisteredWidget(flags, dontcareunique);
}

GLWidget* OpenGLManager::getRegisteredWidget(int flags, bool& unique)
{
    if (!lockWidgets())
        return 0;

    bool found = false;
    GLWidget* returning = 0;
    for (auto it = mWidgets.begin(); it != mWidgets.end(); it++)
    {
        EGLWidgetFlags match = (EGLWidgetFlags)(flags & it->mFlags);
        if (match == flags) //all flags are contained in description of this widget
        {
            if (found)
            {
                unique = false;
            }
            else
            {
                found = true;
                unique = true;
                returning = it->mWidget;
            }
        }
    }

    unlockWidgets();
    return returning;
}

void OpenGLManager::connectGLStateSignal(GLWidget* widget, QObject* target, const char* member, bool queued, bool blocked)
{
    if (!queued && !blocked)
    {
        LOG_WARNING() << "Parameters 'queued' and 'blocked' are set to false. Function call has no effect.";
    }

    for (auto it = mWidgets.begin(); it != mWidgets.end(); it++)
    {
        if (it->mWidget == widget)
        {
            if (queued)
                connect(&*it, SIGNAL(glStateSignal(GLWidget*, OpenGLManager::EGLStateSignal)), target, member);
            if (blocked)
                connect(&*it, SIGNAL(glStateSignalBlocked(GLWidget*, OpenGLManager::EGLStateSignal)), target, member);
            break;
        }
    }
}

bool OpenGLManager::lockGLWidget(GLWidget* widget, EGLStateSignal lockSignal, bool blockedLock)
{
    if (!hasWidget(widget))
        LOG_WARNING() << "Called unlockGLWidget on widget that is not in managed widget list.";

     //Lock rendering access, in case other widgets with this context are created (or the following "using" Signal like GSS_ACCESS is not handled by the receiver)
    bool lockedContext = widget->tryLock(300);
    if (!lockedContext)
    {
        LOG_ERROR() << "Could not lock rendering to GLWidget with tryLock() because of timeout.";
        return false;
    }

    //Send signal for receiver thread to prepare external opengl access
    sendGLStateSignal(widget, lockSignal, blockedLock);

    //Disable resize/repaint updates
    widget->setUpdatesEnabled(false);

    //Make the target gl context current in this thread
    widget->makeCurrent();

    return true;
}

void OpenGLManager::unlockGLWidget(GLWidget* widget, EGLStateSignal unlockSignal, bool blockedUnlock)
{
    if (!hasWidget(widget))
        LOG_WARNING() << "Called unlockGLWidget on widget that is not in managed widget list.";

    widget->doneCurrent();           //free gl context
    widget->unlock();                //unlock rendering
    widget->setUpdatesEnabled(true); //re-enable update functions
    sendGLStateSignal(widget, unlockSignal, blockedUnlock); //tell receivers that the access is finished
}

bool OpenGLManager::lockWidgets()
{
    if (!mWidgetMutex.tryLock(100))
    {
        LOG_ERROR() << "Could not access GLWidget list because it is locked.";
        return false;
    }
    return true;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void OpenGLManager::unlockWidgets()
{
    mWidgetMutex.unlock();
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void OpenGLManager::sendGLStateSignal(GLWidget* widget, EGLStateSignal signal, bool waitReturn)
{
    if (!hasWidget(widget))
        LOG_WARNING() << "GLWidget " << widget->getName() << " is not managed by the opengl manager.";

    for (auto it = mWidgets.begin(); it != mWidgets.end(); it++)
    {
        if (it->mWidget == widget)
        {
            if (waitReturn)
                it->emitSignalBlocked(signal);
            else
                it->emitSignal(signal);
            break;
        }
    }


}

void OpenGLManager::glWidgetDestroyed(QObject* widget)
{
    if (!lockWidgets())
    {
        LOG_WARNING() << "Could not remove destroyed GlWidget from list because list access is locked.";
        return;
    }

    for (auto it = mWidgets.begin(); it != mWidgets.end(); )
    {
        if (it->mWidget == widget)
        {
            LOG_WARNING() << "Removing registered GLWidget 0x" << QString::number((intptr_t)widget, 16).toStdString().c_str() <<
                " because it was destroyed. Call unregisterGLWidget() before destruction.";
            it = mWidgets.erase(it);
        }
        else
            it++;
    }
}

//--------------------------------------------------------------
// GLHandle public
//--------------------------------------------------------------

void GLHandle::emitSignal(OpenGLManager::EGLStateSignal signal)
{
    emit glStateSignal(mWidget, signal);
}

void GLHandle::emitSignalBlocked(OpenGLManager::EGLStateSignal signal)
{
    emit glStateSignalBlocked(mWidget, signal);
}
