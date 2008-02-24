/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparktree.h,v 1.1 2008/02/24 16:20:25 rollmark Exp $

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
#ifndef SPARKTREE_H__
#define SPARKTREE_H__

#ifdef WIN32
#include <winsock2.h>
#endif

#include <zeitgeist/core.h>
#include <wx/wx.h>
#include <wx/treectrl.h>

class SparkTree
{
public:
    class ItemData : public wxTreeItemData
    {
    public:
        boost::weak_ptr<zeitgeist::Leaf> leaf;

    public:
        ItemData(boost::weak_ptr<zeitgeist::Leaf> l)
            : wxTreeItemData(), leaf(l)
        {}
    };

public:
    SparkTree();
    virtual ~SparkTree();

    void Init(wxTreeCtrl* tree);
    bool CreateChildren(const wxTreeItemId id);
    boost::weak_ptr<zeitgeist::Leaf> GetLeaf(const wxTreeItemId id);

    bool GetLocation(const wxTreeItemId id, wxString& location);
    bool SelectLeaf(boost::weak_ptr<zeitgeist::Leaf> leaf);

    void MarkItem(const wxTreeItemId id);

protected:
    void SetItemData(wxTreeItemId id, boost::weak_ptr<zeitgeist::Leaf> leaf);
    ItemData* GetItemData(wxTreeItemId id);
    void MarkItemInternal(const wxTreeItemId id, bool mark);

protected:
    wxTreeCtrl* mTree;
    boost::shared_ptr<zeitgeist::Core> mCore;
    wxTreeItemId mRootId;
    wxTreeItemId mLastMarkedNode;
};

#endif // SPARKTREE_H__
