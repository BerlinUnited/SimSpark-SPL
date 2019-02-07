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

#ifndef MENUOBJECT_H
#define MENUOBJECT_H
/**
 * \file menuobject.h
 * \brief Contains MenuSystem::MenuObject class, the abstract superclass for menu objects handled by the MenuManager.
 */

#include "menumanager.h"

#include <QString>

#include <QString>

class QWidget;
class AbstractPlugin;

namespace MenuSystem
{

/*!
 \brief Abstract class containing information on every Qt menu object created by or provided to the MenuManager.
*/
class MenuObject
{

public:
    // public functions
    /*!
     \brief Basic constructor.

     \param name locally unique name
     \param type type of the menu object
     \param location location, either 'main' or 'local'
     \param creator creator of the menuobject, pointer to Plugin or 0 for MenuManager
    */
    MenuObject(const QString& name, MenuManager::EMenuObjectType type, MenuManager::EMenuObjectLocation location, const AbstractPlugin* creator = 0);

    /*!
     \brief Returns object type.
    */
    MenuManager::EMenuObjectType getType() const;
    /*!
     \brief Returns the location.
    */
    MenuManager::EMenuObjectLocation getLocation() const;
    /*!
     \brief Returns the creator pointer.
    */
    AbstractPlugin* getCreator() const;
    /*!
     \brief Returns the name.
    */
    const QString& getName() const;

    /*!
     \brief Returns true if the creator already has a MenuBar.
    */
    bool hasMenuBar() const;
    /*!
     \brief Returns true if the creator already has a Layout.
    */
    bool hasLayout() const;
    /*!
     \brief Returns the MenuBar of the creator.

     Works even if the creator is 0 (MenuManager).
    */
    QMenuBar* getMenuBar() const;
    /*!
     \brief Returns the main Layout of the creator.

     Works even if the creator is 0 (MenuManager).
    */
    QLayout* getLayout() const;

    /*!
     \brief Returns true if the MenuObject has already been attached to its target.
    */
    virtual bool isAttached() const;

    /*!
     \brief Abstract function to delete the MenuObject from its location.
    */
    virtual bool cleanEntry() = 0;
    /*!
     \brief Abstract function to update the MenuObject with new data using the same name.

     The MenuObject used has to be of the same type than the already contained one.
     \param object new object
    */
    virtual bool update(QObject* object) = 0;
    /*!
     \brief Abstract function to attach this MenuObject's object to the target.
    */
    virtual bool attach() = 0;

    /*!
     \brief Returns the contained object.
    */
    virtual QObject* getObject() const;

protected:
    // protected members

    MenuManager::EMenuObjectType mType; /*!< Type of the Object. */
    MenuManager::EMenuObjectLocation mLocation; /*!< Location of the Object. */
    AbstractPlugin* mCreator; /*!< Pointer to Plugin that provided the object, or 0 for MenuManager. */
    QString mName; /*!< Name of the Menu object. Has to be locally unique. */

    bool mDeleted; /*!< If mDeleted is true, the object will be invisible and inactive. */
    bool mAttached; /*!< True after attaching the contained object to its target position. */
};

} //namespace MenuSystem


#endif // MENUOBJECT_H
