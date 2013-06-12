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

#ifndef MENUOBJECT_MENU_H
#define MENUOBJECT_MENU_H
/**
 * \file menuobjectmenu.h
 * \brief Contains MenuSystem::Menu class.
 */

#include "menuobject.h"

class QMenu;

namespace MenuSystem
{

/*!
 \brief Class defining a Menu created by the MenuManager or provided by a Plugin.

*/
class Menu : public MenuObject
{
public:
    // public functions

    /*!
     \brief Basic constructor.

     \param name locally unique name
     \param menu menu to attach to target location
     \param location location, either in main window or local
     \param creator creator of the menuobject, pointer to Plugin or 0 for MenuManager
    */
    Menu(const QString& name, QMenu* menu, MenuManager::EMenuObjectLocation location = MenuManager::MOL_MAIN, const AbstractPlugin* creator = 0);

    virtual ~Menu();

    /*!
     \brief Deletes the MenuObject from its attached position.
    */
    virtual bool cleanEntry();
    /*!
     \brief Updates the MenuObject with new information, replacing the old ones.

     The Object type has to be the same than the one already contained.
     \param object new object
    */
    virtual bool update(QObject* object);
    /*!
     \brief Attaches the contained object to its target location.
    */
    virtual bool attach();

protected:
    // protected functions

    /*!
     \brief Returns the object.
    */
    virtual QObject* getObject() const;

private:
    // private members

    QMenu* mMenu; /*!< Pointer to the object. */
};

} //namespace MenuSystem

#endif // MENUOBJECT_MENU_H
