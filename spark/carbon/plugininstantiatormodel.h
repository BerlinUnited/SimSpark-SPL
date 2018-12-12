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

#ifndef PLUGININSTANTIATORMODEL_H
#define PLUGININSTANTIATORMODEL_H
/**
 * \file plugininstantiatormodel.h
 * \brief Contains PluginInstantiatorTableModel class.
 */

#include "plugintype.h"
#include "abstractplugin.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <vector>
#include <QAbstractTableModel>

class PluginDefinition;

/*!
 \brief Table model for the plugin factories instantiator list.

 Created by the plugin factory.
*/
class PluginInstantiatorTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Basic constructor.

     \param list data reference
    */
    PluginInstantiatorTableModel(PluginFactory::TInstantiatorMap& list);
    /*!
     \brief Basic destructor.
    */
    virtual ~PluginInstantiatorTableModel();

    /*!
     \brief Returns row count.
    */
    int rowCount (const QModelIndex & parent = QModelIndex() ) const;
    /*!
     \brief Returns column count.
    */
    int columnCount (const QModelIndex & parent = QModelIndex() ) const;

    /*!
     \brief Change model data.

     \param index index
     \param value value
     \param role role of the call
    */
    bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    /*!
     \brief Returns model data.

     \param index index
     \param role role of the call
    */
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole ) const;

    /*!
     \brief Changes header data.

     \param section index
     \param orientation axis
     \param value value
     \param role role (not used)
    */
    bool setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);
    /*!
     \brief Reads header data.

     \param section index
     \param orientation axis
     \param role role (not used)
    */
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /*!
     \brief Returns handle flags for cells.

     \param index index
    */
    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    /*!
     \brief Returns instantiator at model index.

     \param index index
     \return 0 if index is illegal
    */
    AbstractPluginInstantiator* getInstantiatorAt(int index) const;

public slots:

    /*!
     \brief Updates an instantitator display when the model data changed.

     \param classId id of the plugin class
    */
    void updateInstantiatorDisplay(int classId);

private:
    //private members

    PluginFactory::TInstantiatorMap& mInstantiators; /*!< Model data reference. */
    std::vector<QVariant> mHeaderData; /*!< Model header data. */
};

#endif //PLUGININSTANTIATORMODEL_H
