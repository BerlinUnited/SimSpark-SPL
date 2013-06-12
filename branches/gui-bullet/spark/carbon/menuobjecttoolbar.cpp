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

#include "menuobjecttoolbar.h"
#include "abstractplugin.h"
#include "windowmanager.h"

#include <QToolBar>
#include <QMainWindow>
#include <QLayout>
#include <QWidget>

namespace MenuSystem
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

ToolBar::ToolBar(const QString& name, QToolBar* toolBar, MenuManager::EMenuObjectLocation location, const AbstractPlugin* creator)
    : MenuObject(name, MenuManager::MOT_TOOLBAR, location, creator)
{
    mToolBar = toolBar;
}

ToolBar::~ToolBar()
{
    cleanEntry();
}

QObject* ToolBar::getObject() const
{
    return mToolBar;
}

QToolBar* ToolBar::getToolBar() const
{
    return mToolBar;
}

bool ToolBar::attach()
{
    // The object is attached to its target position
    if (isAttached())
    {
        LOG_WARNING() << "Menu Object " << mName << " is already attached to parent " << mCreator->getCaption() << ".";
        return false;
    }
    
    //Add Menu to MenuBar
    if (mLocation == MenuManager::MOL_MAIN)
        Carbon::get()->getWindowManager()->getMainWindow()->addToolBar(mToolBar);
    else
    {
        QWidget* widget = mCreator->getVisualRepresentation();
        QWidget* parent = widget->parentWidget();

        //Add top bar
        //QVBoxLayout* newLayout = new QVBoxLayout;
        //newLayout->addWidget(mToolBar);
        //newLayout->addWidget(widget);
        //parent->setLayout(newLayout);
        widget->layout()->addWidget(mToolBar);
        widget->layout()->setAlignment(mToolBar, Qt::AlignTop);
    }


    mAttached = true;

    return true;
}

bool ToolBar::cleanEntry()
{
    if (mDeleted)
        return false;

    mToolBar->deleteLater();
    mDeleted = true;

    return true;
}

bool ToolBar::update(QObject* object)
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
