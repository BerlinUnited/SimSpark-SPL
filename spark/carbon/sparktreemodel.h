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

#ifndef SPARKTREEMODEL_H
#define SPARKTREEMODEL_H
/**
 * \file sparktreemodel.h
 * \brief Contains SparkTreeModel and SparkTreeItem classes.
 */
 
#include <QAbstractItemModel>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#include <vector>

#include "treeitem.h"

namespace zeitgeist
{
    class Core;
    class Leaf;
}

/*!
 \brief Representative TreeItem to store and reference a zeitgeist leaf.

 Contains the data to be displayed, parent and child information, and a weak pointer to the zeitgeist::Leaf to check and lock it when accessing.
*/
class SparkTreeItem : public TreeItem
{
public:
    //public functions

    /*!
     \brief Complete Constructor.

     \param data data to display (columns)
     \param parent parent node
     \param leaf leaf to represent
    */
    SparkTreeItem(const QList<QVariant>& data, SparkTreeItem* parent, boost::weak_ptr<zeitgeist::Leaf> leaf) :
      TreeItem(data, parent, (void*)leaf.lock().get()),
      mLeaf(leaf){}

    /*!
     \brief Basic Destructor.
    */
    virtual ~SparkTreeItem(){}

    /*!
     \brief Returns the leaf pointer.
    */
    boost::weak_ptr<zeitgeist::Leaf> leaf(){return mLeaf;}

private:
    //private members

    boost::weak_ptr<zeitgeist::Leaf> mLeaf; /*!< Pointer to zeitgeist::Leaf. */
};

/*!
 \brief A Qt Model/View tree model for a zeitgeist tree.

 Has a reference to the spark tree root node and a representative SparkTreeItem tree. The representative tree is essentially a copy or
 proxy model of the tree structure with all the formatted data to be displayed in the view. Every node contains a weak reference to the
 zeitgeist node and can therefore check if the node is still available or not and lock and access to it if needed.\n

 A Proxy model allows reacting to any call to the model with a simple casting of the contained zeitgeist::Leaf pointer and creating a new
 model index with the parent/child or childCount and the new pointer respectively, instead of recursively calling functions like rowCount() or
 parent() up to the root node. This also prevents synchronization issues between the proxy model and the zeitgeist tree. We can always investigate
 the tree at a given state and check if a node has been deleted any time without having it suddenly vanish from the displayed proxy model.\n

 The SparkTreeModel can be updated via updateModelData(), efficiently re-searching the zeitgeist tree and updating the proxy model.
 When updating, all SparkTreeItems referencing a vanished zeitgeist::Leaf will be removed from the proxy model, so dont store any references
 to these items.\n

 \todo To give feedback on which elements have been deleted the item deletion handling could be extended. When a zeitgeist::Leaf is removed and this is
 registerd by the SparkTreeModel during the next update, instead of deleting the SparkTreeItem right away there could be a delete flag set for the item.
 If a deleted item is currently selected its color in the assigned view could be changed to red to show the user that this node was removed without
 simply removing the node and taking away the focused object he wanted to inspect.
 Further extending, the SparkTreeModel could store deleted items temporarily, changing their display color and removing them after a short time,
 like with a process manager. The items in the view will briefly  change their color and then vanish. This way the user gets efficent feedback
 about how the scene graph is changing in a dynamic environment.
*/
class SparkTreeModel : public QAbstractItemModel
{
    Q_OBJECT
 
public:
    //public functions
 
    /*!
     \brief Complete Constructor.

     \param core pointer to zeitgeist core.
     \param parent parent object
    */
    SparkTreeModel(boost::shared_ptr<zeitgeist::Core> core, QObject* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~SparkTreeModel();

    /*!
     \brief Returns row count of a model index. This represents its child count.
    */
    int rowCount (const QModelIndex & parent = QModelIndex()) const;
    /*!
     \brief Returns column count of a model index. This is the amount of date in each tree item.
    */
    int columnCount (const QModelIndex & parent = QModelIndex()) const;
    
    /*!
     \brief Returns a model index for a child of index parent at the given row and column.
    */
    QModelIndex index(int row, int col, const QModelIndex & parent = QModelIndex()) const;
    /*!
     \brief Returns the model index of the given childs parent.
    */
    QModelIndex parent (const QModelIndex & child) const;
    /*!
     \brief Returns the model data at the given index.
    */
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
    /*!
     \brief Returns header data.

     \param section row or column index
     \param orientation row or column
     \param role data role
    */
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*!
     \brief Returns flags at the given index.
    */
    Qt::ItemFlags flags(const QModelIndex & index) const;

    /*!
     \brief Changes model data at the given index. (Currently no effect on the zeitgeist tree)

     \param index model index to change data in
     \param value new value
     \param role data role
    */
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    /*!
     \brief Updates the whole tree by searching it iteratively, adding new zeitgeist nodes and deleting representatives of vanished nodes.

     \note This function only adds nodes at the end of the child list. When there should be changes of node orders within zeitgeist (inserting
     nodes at a specific position in a node instead of adding them at the end) the indexing might fail here and should therefore be rewritten.
    */
    void updateModelData();

private:
    //private functions
    /*!
     \brief Initializes the proxy model.
    */
    void setupModelData();
    /*!
     \brief Updates a specific item by adding new children, deleting vanished children and recursively updating.

     \param item item to check
     \return false if the item has been deleted; used to indentify children that have to be deleted
    */
    bool updateModelData(SparkTreeItem* item);

private:
    //private members

    boost::shared_ptr<zeitgeist::Core> mCore; /*!< Zeitgeist core. */
    SparkTreeItem* mRoot; /*!< Tree item representing the cores root node. */
}; 

#endif //SPARKTREEMODEL_H
