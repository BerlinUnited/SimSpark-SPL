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

#include "plugininstantiatormodel.h"
#include "pluginmanager.h"
#include "carbon.h"

//--------------------------------------------------------------
// PluginInstantiatorTableModel
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginInstantiatorTableModel::PluginInstantiatorTableModel(PluginFactory::TInstantiatorMap& list) :
        mInstantiators(list)
{
    mHeaderData.push_back(QVariant(tr("Name")));
    mHeaderData.push_back(QVariant(tr("Type")));
    mHeaderData.push_back(QVariant(tr("Source")));
    mHeaderData.push_back(QVariant(tr("Amount")));
    emit headerDataChanged(Qt::Horizontal, 0, 3);

    PluginManager* manager = Carbon::get()->getPluginManager();
    connect(manager, SIGNAL(pluginOfClassAdded(int)), this, SLOT(updateInstantiatorDisplay(int)));
    connect(manager, SIGNAL(pluginOfClassRemoved(int)), this, SLOT(updateInstantiatorDisplay(int)));
}

PluginInstantiatorTableModel::~PluginInstantiatorTableModel()
{
}

int PluginInstantiatorTableModel::rowCount (const QModelIndex & parent) const
{
    return mInstantiators.size();
}

int PluginInstantiatorTableModel::columnCount (const QModelIndex & parent) const
{
    return 4;
}

QVariant PluginInstantiatorTableModel::data (const QModelIndex & index, int role) const
{
    int row = index.row();
    int col = index.column();
    AbstractPluginInstantiator* instantiator = getInstantiatorAt(row);
    if (instantiator == 0)
        return QVariant(tr("#Error#"));

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        PluginManager* manager;
        int classId, count, max;
        QString lib;
        switch (col)
        {
        case 0:
            return QVariant(instantiator->getName());
            break;
        case 1:
            return QVariant(PluginType::getTypeString(instantiator->getPluginType()));
            break;
        case 2:
            lib = instantiator->getLibrary();
            return ((lib.compare("") == 0) ? QVariant("built-in") : QVariant(lib));
            break;
        case 3:
            manager = Carbon::get()->getPluginManager();
            classId = instantiator->getId();
            count = manager->getInstanceCount(classId);
            max = instantiator->getMaxInstanceCount();
            return QVariant(QString("%1/%2").arg(count).arg(max));
            break;
        }
    }
    else if (role == Qt::ToolTipRole)
    {
        switch(col)
        {
        case 0: 
            //Name
            return QVariant(tr("Name of the plugin class."));
            break;
        case 1: 
            //Type
            return QVariant(tr("Type of the plugin.\n" \
                "NORMAL: Plugins without visual representation and communication capability. Usually used for executable plugins that are used in Simulation-Tasks.\n" \
                "SIGNAL: Plugins without visual representation but with communication capability. Usually used as functionality library and for Menu or Tool extensions.\n" \
                "FRAME: Plugins with visual representation. These plugins are a visible frame that can be attached to attachment points of the gui."));
            break;
        case 2: 
            //Source
            return QVariant(tr("Source of the plugin. Plugins are either 'built-in' or loaded from a shared library in one of the include directories."));
            break;
        case 3: 
            //Amount
            return QVariant(tr("Actual and maximum amount of plugin instances."));
            break;
        default: 
            return QVariant(tr("#Illegal column Index#"));
            break;
        }
    }

    return QVariant();
}

bool PluginInstantiatorTableModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        int row = index.row();
        int col = index.column();
        AbstractPluginInstantiator* instantiator = getInstantiatorAt(row);
        if (instantiator == 0)
            return false;

        switch (col)
        {
        case 0:
            return false;
            break;
        case 1:
            return false;
            break;
        case 2:
            return false;
            break;
        case 3:
            return false;
            break;
        }
    }
    else
    {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}


bool PluginInstantiatorTableModel::setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section >= 0 && section < 4)
            {
                mHeaderData.at(section);
                emit headerDataChanged(Qt::Horizontal, section, section);
            }
        }
        else
        {
            LOG_WARNING() << "Vertical orientation header data cant be changed.";
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

QVariant PluginInstantiatorTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section >= 0 && section < 4)
            {
                return mHeaderData.at(section);
            }
            else
            {
                return QVariant();
            }
        }
        else
        {
            if (section >= 0 && section < (int)mInstantiators.size())
            {
                return QVariant(section);
            }
            else
            {
                return QVariant("#Error#");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags PluginInstantiatorTableModel::flags ( const QModelIndex & index ) const
{
    int row = index.row();
    int col = index.column();

    AbstractPluginInstantiator* instantiator = getInstantiatorAt(row);
    if (instantiator == 0)
        return 0;

    switch (col)
    {
    case 0:
        //Name
        return Qt::ItemIsEnabled;
        break;
    case 1:
        //Type
        return Qt::ItemIsEnabled;
        break;
    case 2:
        //Source
        return Qt::ItemIsEnabled;
        break;
    case 3:
        //Instance Amount
        return Qt::ItemIsEnabled;
        break;
    }
    return 0;
}

AbstractPluginInstantiator* PluginInstantiatorTableModel::getInstantiatorAt(int index) const
{
    PluginFactory::TInstantiatorMap::iterator it = mInstantiators.begin();
    if (it == mInstantiators.end())
        return 0;

    for (int i = 0; i < index; i++)
    {
        it++;
        if (it == mInstantiators.end())
            return 0;
    }

    return it->second;
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

void PluginInstantiatorTableModel::updateInstantiatorDisplay(int id)
{
    int i = 0;
    for (PluginFactory::TInstantiatorMap::iterator it = mInstantiators.begin(); it != mInstantiators.end(); it++)
    {
        if (it->second->getId() == id)
        {
            //update this row
            QModelIndex start = index(i, 0);
            QModelIndex end = index(i, 3);
            emit dataChanged(start, end);
            return;
        }
        i++;
    }
}
