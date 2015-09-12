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

#include "menuobject.h"
#include "abstractplugin.h"

#include <QLayout>

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

MenuObject::MenuObject(const QString& name, MenuManager::EMenuObjectType type, MenuManager::EMenuObjectLocation location, const AbstractPlugin* creator)
{
    mDeleted = false;
    mAttached = false;
    mName = name;
    mType = type;
    mLocation = location;
    mCreator = const_cast<AbstractPlugin*>(creator);
}

MenuManager::EMenuObjectType MenuObject::getType() const
{
    return mType;
}

MenuManager::EMenuObjectLocation MenuObject::getLocation() const
{
    return mLocation;
}

AbstractPlugin* MenuObject::getCreator() const
{
    return mCreator;
}

const QString& MenuObject::getName() const
{
    return mName;
}

QObject* MenuObject::getObject() const
{
    return 0;
}

bool MenuObject::hasMenuBar() const
{
    return (hasLayout() && (getMenuBar() != 0));
}

bool MenuObject::hasLayout() const
{
    return getLayout() != 0;
}

QMenuBar* MenuObject::getMenuBar() const
{
    if (mLocation == MenuManager::MOL_MAIN)
        //return main MenuBar
        return MenuManager::getMenuBar();
    else
        //return local MenuBar
        return (QMenuBar*)mCreator->getVisualRepresentation()->layout()->menuBar();

    return 0;
}

QLayout* MenuObject::getLayout() const
{
    if (mLocation == MenuManager::MOL_MAIN)
        //return main layout
        return getMenuBar()->parentWidget()->layout();
    else
        //return local layout
        return mCreator->getVisualRepresentation()->layout();

    return 0;
}

bool MenuObject::isAttached() const
{
    return mAttached;
}

bool MenuObject::update(QObject* object)
{
    LOG_INFO() << "TODO";
    return true;
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

} //namespace MenuSystem