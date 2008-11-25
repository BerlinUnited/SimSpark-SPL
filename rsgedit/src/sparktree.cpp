/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#include "sparktree.h"
#include "main.h"
#include "simspark.h"

//! wxWidgets and zeitgeist both use a 'DECLARE_CLASS' macro
#undef DECLARE_CLASS

#include <zeitgeist/leaf.h>
#include <zeitgeist/node.h>
#include <zeitgeist/class.h>

using namespace boost;
using namespace zeitgeist;

SparkTree::SparkTree() : mTree(0)
{
}

SparkTree::~SparkTree()
{
    // remove but don't delete us from the event handler stack of the
    // tree
    mTree->PopEventHandler(false);
}

void SparkTree::SetItemData(wxTreeItemId id, weak_ptr<Leaf> leaf)
{
    ItemData* data = new ItemData(leaf);
    mTree->SetItemData(id, data);
}

SparkTree::ItemData* SparkTree::GetItemData(wxTreeItemId id)
{
    ItemData* data = static_cast<ItemData*>(mTree->GetItemData(id));

    if (
        (data == 0) ||
        (data->leaf.expired())
        )
        {
            return 0;
        }

    return data;
}

void SparkTree::Init(wxTreeCtrl* ctrl)
{
    mTree = ctrl;
    mTree->DeleteAllItems();

    shared_ptr<SimSpark> spark = wxGetApp().GetSpark();
    if (spark.get() == 0)
        {
            return;
        }

    mCore = spark->GetCore();
    mRootId = mTree->AddRoot(_T("/"));

    shared_ptr<Leaf> root = mCore->GetRoot();
    SetItemData(mRootId, root);

    if (! root->IsLeaf())
        {
            mTree->SetItemHasChildren(mRootId,true);
            mTree->Expand(mRootId);
        }
}

bool SparkTree::CreateChildren(const wxTreeItemId id)
{
    if (mTree->GetChildrenCount(id) > 0)
        {
            // already expanded
            return true;
        }

    ItemData* data = GetItemData(id);
    if (data == 0)
        {
            return false;
        }

    const Leaf& leaf = (*data->leaf.lock());
    if (leaf.IsLeaf())
        {
            return false;
        }

    for (
         Leaf::TLeafList::const_iterator iter = leaf.begin();
         iter != leaf.end();
         ++iter
         )
        {
            shared_ptr<Leaf> child = (*iter);
            wxString label(child->GetName().c_str(), wxConvUTF8);

            shared_ptr<Class> childClass = child->GetClass();
            if (childClass.get() != 0)
                {
                    label << _T(" (")
                          << wxString(childClass->GetName().c_str(), wxConvUTF8)
                          << _T(")");
                }

            wxTreeItemId childId = mTree->AppendItem(id, label);
            SetItemData(childId, child);

            if (
                (! child->IsLeaf()) &&
                (child->begin() != child->end())
                )
                {
                    // allow expansion but don't add children yet
                    mTree->SetItemHasChildren(childId,true);
                }
        }

    return true;
}

weak_ptr<Leaf> SparkTree::GetLeaf(const wxTreeItemId id)
{
    if (! id.IsOk())
    {
        return weak_ptr<Leaf>();
    }

    ItemData* data = GetItemData(id);
    if (data == 0)
        {
            return weak_ptr<Leaf>();
        }

    return data->leaf;
}

bool SparkTree::GetLocation(const wxTreeItemId id, wxString& location)
{
    weak_ptr<Leaf> leaf = GetLeaf(id);

    if (leaf.expired())
        {
            return false;
        }

    location = wxString(leaf.lock()->GetFullPath().c_str(), wxConvUTF8);
    return true;
}

bool SparkTree::SelectLeaf(weak_ptr<Leaf> leaf)
{
    if (leaf.expired())
        {
            return false;
        }

    // build parent chain
    Leaf::TLeafList leafList;
    leafList.push_back(leaf.lock());

    weak_ptr<Leaf> curLeaf(leaf);
    for (;;)
        {
            weak_ptr<Leaf> parent = curLeaf.lock()->GetParent();
            if (! parent.expired())
                {
                    leafList.push_back(parent.lock());
                    curLeaf = parent;
                } else
                {
                    break;
                }
        }


    if (leafList.size() <= 1)
    {
        return false;
    }

    // pop root nodea
    leafList.pop_back();

    // expand nodes in reverse order
    wxTreeItemId curItem = mRootId;

    for (
         Leaf::TLeafList::reverse_iterator iter = leafList.rbegin();
         iter != leafList.rend();
         ++iter
         )
        {
            CreateChildren(curItem);
            mTree->Expand(curItem);

            weak_ptr<Leaf> leaf = (*iter);
            if (leaf.expired())
                {
                    assert(false);
                    return false;
                }

            wxTreeItemIdValue cookie;
            wxTreeItemId item = mTree->GetFirstChild(curItem, cookie);

            bool foundChild = false;

            while (item.IsOk())
                {
                    ItemData* data = GetItemData(item);
                    if (data != 0)
                        {
                            weak_ptr<Leaf> child = data->leaf;
                            if (child.expired())
                                {
                                    assert(false);
                                    return false;
                                }

                            if (leaf.lock() == child.lock())
                                {
                                    curItem = item;
                                    foundChild = true;
                                    break;
                                }
                        }
                    item = mTree->GetNextSibling(item);
                }

            if (! foundChild)
                {
                    assert(false);
                    return false;
                }
        }

    mTree->SelectItem(curItem);
    mTree->EnsureVisible(curItem);

    return true;
}

void SparkTree::MarkItemInternal(const wxTreeItemId id, bool mark)
{
    if (! id.IsOk())
        {
            return;
        }
    if (mark)
        {
            mTree->SetItemTextColour(id, wxColour(230, 26, 230));
            mTree->SetItemBold(id,true);
        } else
        {
            mTree->SetItemTextColour(id, wxNullColour);
            mTree->SetItemBold(id,false);
        }
}

void SparkTree::MarkItem(const wxTreeItemId id)
{
    MarkItemInternal(mLastMarkedNode,false);
    MarkItemInternal(id, true);
    mLastMarkedNode = id;
}
