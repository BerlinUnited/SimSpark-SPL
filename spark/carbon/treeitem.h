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

#ifndef TREEITEM_H
#define TREEITEM_H
/**
 * \file treeitem.h
 * \brief Contains TreeItem class.
 */

#include <QList>
#include <QVariant>

/*!
 \brief A class to build a Tree that can be used to create a proxy model for any tree structure to be displayed in a TreeView.

 The TreeItems can be used to recursively create a duplicate of a tree structure that should be displayed or accessed via a
 QAbstractItemView. Can be used to represent and navigate any tree structure as long as the structure contains a parent() function,
 a child(int index) function and a childCount() function.\n

 The item contains a list of DataObjects to store any data that should be interfaced by the proxy model. For example in a model containing
 several kinds of numeric values, the TreeItems could hold the formatted text composed of these values and their titles to be displayed in a view.
 The item also contains a void* pointer, which can be used to identify children of a TreeItem that contain this pointer, or to store external data.\n

 For an example how to create and navigate a TreeItem tree, see its usage in SparkTreeModel and SparkTreeView.

 \sa SparkTreeModel, SparkTreeItem
*/
class TreeItem
{

public:
    //public functions

    /*!
     \brief Complete constructor.

     \param data data to store
     \param parent parent treeitem, or 0 if its a root item
     \param pointer pointer to data, or 0
    */
    TreeItem(const QList<QVariant> &data, TreeItem *parent = 0, void* pointer = 0);
    /*!
     \brief Destructor. Automatically deletes all child items.
    */
    virtual ~TreeItem();

    /*!
     \brief Add a new child at the end of the child list.

     \param child child to append
     \param unique if true, the first occurence of the item will be replaced with the new child if there is one
    */
    void appendChild(TreeItem *child, bool unique = false);

    /*!
     \brief Returns the index of this item in the parent treeitem, or 0 if this item has no parent.
    */
    int childIndex() const;

    /*!
     \brief Returns the child at the given index.
    */
    TreeItem* child(int row);
    /*!
     \brief Return the number of children.
    */
    int childCount() const;
    /*!
     \brief Returns the amount of data in the list.
    */
    int columnCount() const;
    /*!
     \brief Returns the data object at the given column index.
    */
    QVariant data(int column) const;
    /*!
     \brief Returns the index of this item in the parent treeitem, or 0 if this item has no parent.
    */
    int row() const;
    /*!
     \brief Returns the parent or 0 if there is none.
    */
    TreeItem *parent();
    /*!
     \brief Returns the stored pointer.
    */
    void* pointer();

    /*!
     \brief Returns true if one of the children contains the given pointer.
    */
    bool containsChildWithPointer(void* pointer) const;
    /*!
     \brief Deletes the given item from the child list, if it is contained.

     \param item pointer to item to delete
    */
    void deleteChild(TreeItem* item);
    /*!
     \brief Deletes all children from the child list.
    */
    void clearChildren();

    /*!
     \brief Returns begin iterator to children list.
    */
    QList<TreeItem*>::Iterator childrenBegin();
    /*!
     \brief Returns end iterator to children list.
    */
    QList<TreeItem*>::Iterator childrenEnd();
    /*!
     \brief Returns begin iterator to data list.
    */
    QList<QVariant>::Iterator dataBegin();
    /*!
     \brief Returns end iterator to data list.
    */
    QList<QVariant>::Iterator dataEnd();

    /*!
     \brief Deletes the item at the given position from the child list and returns the iterator of the following item.

     \param pos which item to erase
    */
    QList<TreeItem*>::Iterator eraseChild(QList<TreeItem*>::Iterator pos);

private:
    //private members

    QList<TreeItem*> mChildItems; /*!< Child items. */
    QList<QVariant> mItemData; /*!< Data of the item. */
    TreeItem *mParentItem; /*!< Pointer to parent. */
    void* mPointer; /*!< Stored pointer. */
};

#endif //TREEITEM_H
