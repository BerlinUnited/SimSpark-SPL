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

 #include <QStringList>
 #include "treeitem.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent, void* pointer)
{
    mParentItem = parent;
    mItemData = data;
    mPointer = pointer;
}

TreeItem::~TreeItem()
{
    clearChildren();
}

int TreeItem::childIndex() const
{
    if (mParentItem)
    {
        int run = 0;
        for (auto it = mParentItem->childrenBegin(); it != mParentItem->childrenEnd(); it++)
        {
            if (*it == this)
                return run;
            run++;
        }
    }
    return 0;
}

void TreeItem::appendChild(TreeItem *item, bool unique)
{
    if (unique)
    {
        //If item should be unique, search first occurence in list
        int pos = mChildItems.indexOf(item);
        if (pos != -1)
        {
            //if found, delete old and replace with new
            delete mChildItems.at(pos);
            mChildItems.replace(pos, item);
            return;
        }
    }

    //not found or not unique -> append
    mChildItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return mChildItems.value(row);
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::columnCount() const
{
    return mItemData.count();
}

QVariant TreeItem::data(int column) const
{
    return mItemData.value(column);
}

TreeItem *TreeItem::parent()
{
    return mParentItem;
}

void* TreeItem::pointer()
{
    return mPointer;
}

bool TreeItem::containsChildWithPointer(void* ptr) const
{
    for (auto it = mChildItems.begin(); it != mChildItems.end(); it++)
    {
        if ((*it)->pointer() == ptr)
            return true;
    }
    return false;
}

void TreeItem::deleteChild(TreeItem* item)
{
    for (auto it = mChildItems.begin(); it != mChildItems.end(); it++)
    {
        if (*it == item)
        {
            mChildItems.erase(it);
            return;
        }
    }
}

void TreeItem::clearChildren()
{
    while (mChildItems.size() > 0)
    {
        delete mChildItems.back();
        mChildItems.pop_back();
    }
}

QList<TreeItem*>::Iterator TreeItem::childrenBegin()
{
    return mChildItems.begin();
}

QList<TreeItem*>::Iterator TreeItem::childrenEnd()
{
    return mChildItems.end();
}

QList<QVariant>::Iterator TreeItem::dataBegin()
{
    return mItemData.begin();
}

QList<QVariant>::Iterator TreeItem::dataEnd()
{
    return mItemData.end();
}

QList<TreeItem*>::Iterator TreeItem::eraseChild(QList<TreeItem*>::Iterator pos)
{
    delete *pos;
    return mChildItems.erase(pos);
}

int TreeItem::row() const
{
    if (mParentItem)
        return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}