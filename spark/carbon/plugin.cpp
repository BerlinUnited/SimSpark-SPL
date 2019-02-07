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

#include "plugin.h"
#include "pluginmanager.h"
#include "carbon.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

Plugin::Plugin()
    : AbstractPlugin(PT_NORMAL)
{
    mHasVisualRepresentation = false;
    mIsQObject = false;

    setExecutionType(ET_ANY);
}

Plugin::~Plugin()
{
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void Plugin::loadData(QSettings& data)
{
    LOG_DEBUG() << "No data to load...";
}

void Plugin::saveData(QSettings& data)
{
    LOG_DEBUG() << "No data to save...";
}

//--------------------------------------------------------------
// PluginInstantiator
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginInstantiator::PluginInstantiator(int id, const QString& name, const QString& caption, Plugin* (*instantiateFunc)(void), 
    int maxInstanceCount, const char* library) :
    AbstractPluginInstantiator(id, name, caption, PT_NORMAL, maxInstanceCount, library),
    mInstantiateFunc(instantiateFunc)
{
}

PluginInstantiator::~PluginInstantiator()
{
}

Plugin* PluginInstantiator::instantiate(const QString& caption) const
{
    Plugin* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(caption);
    return newPlugin;
}

Plugin* PluginInstantiator::instantiate() const
{
    Plugin* newPlugin = (*mInstantiateFunc)();
    newPlugin->setCaption(mDefaultCaption);
    return newPlugin;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------
