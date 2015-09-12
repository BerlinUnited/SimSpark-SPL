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

#include "signalplugin.h"
#include "pluginmanager.h"
#include "carbon.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

SignalPlugin::SignalPlugin()
    : AbstractPlugin(PT_SIGNAL)
{
    mHasVisualRepresentation = false;
    mIsQObject = true;

    setExecutionType(ET_ANY);
}

SignalPlugin::~SignalPlugin()
{
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void SignalPlugin::loadData(const QSettings& data)
{
    LOG_DEBUG() << "No data to load...";
}

void SignalPlugin::saveData(QSettings& data) const
{
    LOG_DEBUG() << "No data to save...";
}

//--------------------------------------------------------------
// SignalPluginInstantiator
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SignalPluginInstantiator::SignalPluginInstantiator(int id, const QString& name, const QString& caption, SignalPlugin* (*instantiateFunc)(void),
    int maxInstanceCount, const char* library) :
    AbstractPluginInstantiator(id, name, caption, PT_SIGNAL, maxInstanceCount, library),
    mInstantiateFunc(instantiateFunc)
{
}

SignalPluginInstantiator::~SignalPluginInstantiator()
{

}

SignalPlugin* SignalPluginInstantiator::instantiate(const QString& caption) const
{
    SignalPlugin* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(caption);
    return newPlugin;
}

SignalPlugin* SignalPluginInstantiator::instantiate() const
{
    SignalPlugin* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(mDefaultCaption);
    return newPlugin;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------
