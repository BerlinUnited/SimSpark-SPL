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

#include "menuobjectmenuaction.h"
#include "abstractplugin.h"

#include <QAction>

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

MenuAction::MenuAction(const QString& name, QAction* action, MenuManager::EMenuObjectLocation location, AbstractPlugin* creator)
    : MenuObject(name, MenuManager::MOT_ACTION, location, creator)
{
    mAction = action;
}

MenuAction::~MenuAction()
{
    cleanEntry();
}

QObject* MenuAction::getObject() const
{
    return mAction;
}

bool MenuAction::attach()
{
    // The object is attached to its target position
    if (isAttached())
    {
        LOG_WARNING() << "Menu Object '" << mName << "' is already attached to parent " << mCreator->getCaption() << ".";
        return false;
    }

    LOG_INFO() << "TODO";

    mAttached = true;

    return true;
}

bool MenuAction::cleanEntry()
{
    if (mDeleted)
        return false;

    mAction->deleteLater();
    mDeleted = true;

    return true;
}

bool MenuAction::update(QObject* object)
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
