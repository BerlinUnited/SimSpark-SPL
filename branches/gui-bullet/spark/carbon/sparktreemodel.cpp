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

#include "sparktreemodel.h"
#include "treeitem.h"
#include "zeitgeist\core.h"
#include "zeitgeist\leaf.h"
#include "zeitgeist\class.h"
#include "carbon.h"

#include <QStringList>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SparkTreeModel::SparkTreeModel(boost::shared_ptr<zeitgeist::Core> core, QObject * parent) :
    QAbstractItemModel(parent),
    mCore(core),
    mRoot(0)
{
    setupModelData();
}

SparkTreeModel::~SparkTreeModel()
{
    delete mRoot;
}

#ifdef XXXXX

QModelIndex SparkTreeModel::index(int row, int col, const QModelIndex & parent) const
{
    qDebug("index row=%d col=%d",row,col);

    /*
    if (!parent.isValid())
        return createIndex(row, col, (void*)&mKeyTree->mChildren.at(row));
    
    SettingsKey* in = (SettingsKey*)parent.internalPointer();

    return createIndex(row, col, (void*)&in->mChildren.at(row));*/

    return QModelIndex();
}
 
QModelIndex SparkTreeModel::parent(const QModelIndex & child) const
{
    qDebug("parent");

    /*
    if (!child.isValid())
        return QModelIndex();

    SettingsKey* in = (SettingsKey*)child.internalPointer();
    //Root node or first level
    if (in->mParent == 0 || in->mParent->mParent == 0 )
        return QModelIndex();

    return createIndex(in->mParent->mParent->findChildIndex(in->mParent), 0, (void*)in->mParent); 
    */

    return QModelIndex();
}
 
int SparkTreeModel::rowCount(const QModelIndex & parent) const
{
    qDebug("rowCount");
    /*

    if (!parent.isValid())
        return mKeyTree->mChildren.size();

    SettingsKey* in = (SettingsKey*)parent.internalPointer();
    return in->mChildren.size();
    */

    return 0;
}
 
int SparkTreeModel::columnCount(const QModelIndex & parent) const
{
    qDebug("columnCount");

    return 2;
}
 
QVariant SparkTreeModel::data(const QModelIndex & index, int role) const
{
    qDebug("data role=%d",role);

    if (role == Qt::DisplayRole)
    {
        return QVariant("test");
    }

    return QVariant();
}

QVariant SparkTreeModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    //qDebug("data role=%d",role);

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
            {
                return tr("Node");
            }
            else if (section == 1)
            {
                return tr("Class");
            }
        }
    }

    return QVariant();
}

#endif

bool SparkTreeModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
    return 0;
}

int SparkTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return mRoot->columnCount();
}

QVariant SparkTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags SparkTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SparkTreeModel::headerData(int section, Qt::Orientation orientation,
                            int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return mRoot->data(section);

    return QVariant();
}

QModelIndex SparkTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mRoot;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SparkTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (parentItem == mRoot)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SparkTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRoot;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void SparkTreeModel::updateModelData()
{
    updateModelData(mRoot);
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void SparkTreeModel::setupModelData()
{
    if (mRoot != 0)
        delete mRoot;

    QList<QVariant> root;
    root << QString("Name") << QString("Class");
    
    mRoot = new SparkTreeItem(root, 0, mCore->GetRoot());
    
    updateModelData(mRoot);
}

bool SparkTreeModel::updateModelData(SparkTreeItem* item)
{
    boost::shared_ptr<zeitgeist::Leaf> leaf(item->leaf().lock());
    if (leaf.get() == 0)
    {
        return true;
    }
    else
    {
        //Create children
        auto list_pos = item->childrenBegin();
        SparkTreeItem* list_at_pos;
        auto leaf_child_it = leaf->begin();
        bool hasItems;
        boost::shared_ptr<zeitgeist::Leaf> comp;
        int row = 0;
        bool at_leaf_end = leaf_child_it == leaf->end();
        bool seek = (item->childCount() > 0 || !at_leaf_end);
        while (seek)
        {
            at_leaf_end = (leaf_child_it == leaf->end());
            hasItems = (item->childCount() > row);

            if (hasItems)
            {
                //If leaf pointer of item at list_pos does not match the leaf_it, 
                //Delete children from items list until indices match (checks if nodes in leaf have been deleted)
                list_at_pos = (SparkTreeItem*)*list_pos;
                comp = boost::shared_ptr<zeitgeist::Leaf>(list_at_pos->leaf().lock());
                if (at_leaf_end || //no more leafs in list
                    !comp.get() || //leaf does not exist anymore
                    comp.get() != leaf_child_it->get()) //leaf is different from the top item in our list
                {
                    //An item was deleted in zeitgeist, remove the item reference and search the next item
                    QModelIndex child_index = createIndex(row, 0, *list_pos);
                    QModelIndex item_index = (item == mRoot ? QModelIndex() : parent(child_index));
                    beginRemoveRows(item_index, row, row);
                    //Erase the tree item
                    list_pos = item->eraseChild(list_pos);
                    endRemoveRows();

                    continue;
                }
            }

            //No more items in the leaf list and in the item list -> end 
            if (at_leaf_end)
            {
                seek = false;
                break;
            }

            //If the leaf item matches the current lists item, they belong together. got to the next item (they are updated later)
            if (hasItems && (comp.get() == leaf_child_it->get()))
            {
                leaf_child_it++;
                list_pos++;
                row++;
                continue;
            }

            //Here the list_pos is at the end. So the current leaf item is not in the item list. It is a new entry, add it (at the end).
            int pos = item->childCount();

            //Data to fill in
            QList<QVariant> data;
            data << leaf_child_it->get()->GetName().c_str() << leaf_child_it->get()->GetClass()->GetName().c_str();

            //Create new child item
            SparkTreeItem* childItem = new SparkTreeItem(data, item, *leaf_child_it);
            QModelIndex child_index = createIndex(pos, 0, childItem);
            QModelIndex item_index = (item == mRoot ? QModelIndex() : createIndex(item->childIndex(), 0, item));

            //Add to proxy model & model
            beginInsertRows(item_index, pos, pos);
            item->appendChild(childItem, true);
            endInsertRows();

            leaf_child_it++;
            list_pos++;
            row++;
        }

        //Update Recursion
        for (auto it = item->childrenBegin(); it != item->childrenEnd(); it++)
        {
            updateModelData((SparkTreeItem*)*it);
        }
    }

    return false;
    /*

    boost::shared_ptr<zeitgeist::Leaf> leaf(item->leaf().lock());
    if (leaf.get() == 0)
    {
        //Illegal pointer, delete this item (from the parent node)
        if (item != mRoot)
            return true;
    }
    else
    {
        //Create children
        int position = 0;
        for (auto it = leaf->begin(); it != leaf->end(); it++)
        {
            //Only update but do not recreate existant leafs
            
            if (item->containsChildWithPointer(it->get()))
            {
                position++;
                continue;
            }

            //Data to fill in
            QList<QVariant> data;
            data << it->get()->GetName().c_str() << it->get()->GetClass()->GetName().c_str();

            //Create new child item
            SparkTreeItem* childItem = new SparkTreeItem(data, item, *it);
            QModelIndex child_index = createIndex(position, 0, childItem);
            QModelIndex item_index = (item == mRoot ? QModelIndex() : createIndex(item->childIndex(), 0, item));

            //Add to proxy model & model
            beginInsertRows(item_index, position, position);
            item->appendChild(childItem, true);
            endInsertRows();
            position++;
        }

        //Update Recursion and delete old nodes
        bool erase = false;
        int row = 0;
        for (auto it = item->childrenBegin(); it != item->childrenEnd(); )
        {
            //Update returns true if item has to be erased
            erase = updateModelData((SparkTreeItem*)*it);
            if (erase)
            {
                //Remove the item from the model, so it wont update from an erased treeitem pointer
                QModelIndex child_index = createIndex(row, 0, *it);
                QModelIndex item_index = (item == mRoot ? QModelIndex() : parent(child_index));
                beginRemoveRows(item_index, row, row);
                //Erase the tree item
                it = item->eraseChild(it);
                endRemoveRows();
            }
            else
            {
                //Dont erase, update next item
                it++;
                row++;
            }
        }
    }

    return false;*/
}