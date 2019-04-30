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

#ifndef SPARKTREEVIEW_H
#define SPARKTREEVIEW_H
/**
 * \file sparktreeview.h
 * \brief Contains SparkTreeView class, which displays spark scene graph data of a SparkTreeModel.
 */
 
#include <QTreeView>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class QSettings;
class SparkTreeModel;

/*!
    \brief Displays spark scene graph data of a SparkTreeModel.
*/
class SparkTreeView : public QTreeView
{
    Q_OBJECT
 
public:
    //public functions
 
    /*!
     \brief Constructor.
    */
    SparkTreeView(QWidget* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~SparkTreeView();

    /*!
     \brief Connect with a model and keep its reference.

     To be used instead of setModel() when there should be direct interaction with SimSpark.

     \param model model to connect
    */
    void connectModel(SparkTreeModel* model);
 
    /*!
     \brief Expands all children starting from the given index recursively.

     \param index index to expand children from
     \param expandself if true, the item is itself is expanded too
    */
    void expandChildren(const QModelIndex& index, bool expandself = true);
    /*!
     \brief Collapses all children starting from the given index recursively.

     \param index index to collapse children from
     \param collapseself if true, the item is itself is collapsed too
    */
    void collapseChildren(const QModelIndex& index, bool collapseself = true);
    
private:
    //private members

    SparkTreeModel* mModel; /*!< Pointer to the Model. */
 
}; 

#endif //SPARKTREEVIEW_H
