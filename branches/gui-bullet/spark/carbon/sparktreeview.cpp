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

#include "sparktreeview.h"
#include "sparktreemodel.h"

SparkTreeView::SparkTreeView(QWidget * parent) :
    QTreeView(parent)
{
    mModel = 0;
}

SparkTreeView::~SparkTreeView()
{
}

void SparkTreeView::connectModel(SparkTreeModel* model)
{
    mModel = model;
    setModel(mModel);
}
 
void SparkTreeView::expandChildren(const QModelIndex& index, bool expandself)
{
    if (index.isValid())
    {
        //Expand the item
        if (expandself)
            expand(index);

        //Recursively expand the children (depth-first search)
        int childCount = mModel->rowCount(index);
        for (int i = 0; i < childCount; i++)
        {
            QModelIndex child = mModel->index(i, 0, index);
            expandChildren(child);
        }
    }
}

void SparkTreeView::collapseChildren(const QModelIndex& index, bool collapseself)
{
    if (index.isValid())
    {
        //Expand the item
        if (collapseself)
            collapse(index);

        //Recursively expand the children (depth-first search)
        int childCount = mModel->rowCount(index);
        for (int i = 0; i < childCount; i++)
        {
            QModelIndex child = mModel->index(i, 0, index);
            collapseChildren(child);
        }
    }
}