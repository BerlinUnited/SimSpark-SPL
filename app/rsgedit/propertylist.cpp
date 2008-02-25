/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: propertylist.cpp,v 1.2 2008/02/25 12:52:55 rollmark Exp $

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
#include "propertylist.h"
#include <wx/listctrl.h>
#include "main.h"

//! wxWidgets and zeitgeist both use a 'DECLARE_CLASS' macro
#undef DECLARE_CLASS

#include <zeitgeist/leaf.h>

using namespace boost;
using namespace zeitgeist;

PropertyList::PropertyList()
{
    mCtrList = 0;
}

void PropertyList::Init(weak_ptr<Leaf> leaf, wxListCtrl* ctrList)
{
    mLeaf.reset();
    mCtrList = 0;

    if (
        (leaf.expired()) ||
        (ctrList == 0)
        )
        {
            return;
        }

    mLeaf = leaf;
    mCtrList = ctrList;

    shared_ptr<Property> property = wxGetApp().GetProperty();
    if (property.get() != 0)
        {
            property->GetClassList(mLeaf.lock(), mClassList);
        }

    mCtrList->ClearAll();
    mCtrList->InsertColumn(0, _T("name"), wxLIST_FORMAT_LEFT, 120);
    mCtrList->InsertColumn(1, _T("value"), wxLIST_FORMAT_LEFT, 600);
}

void PropertyList::Refresh()
{
    if (
        (mCtrList == 0) ||
        (mLeaf.expired())
        )
        {
            return;
        }

    shared_ptr<Property> property = wxGetApp().GetProperty();
    if (property.get() == 0)
        {
            return;
        }

    Property::TEntryList entries;
    property->GenEntries(mLeaf.lock(), mClassList, entries);
    Refresh(entries);
}

void PropertyList::Refresh(const Property::TEntryList& entries)
{
    if (mCtrList == 0)
        {
            return;
        }

    mCtrList->Freeze();

    bool valRefresh =
        (mCtrList->GetItemCount() == static_cast<int>(entries.size()));

    if (! valRefresh)
        {
            mCtrList->DeleteAllItems();
        }

    int line = 0;

    for (
         Property::TEntryList::const_iterator iter = entries.begin();
         iter != entries.end();
         ++iter
         )
        {
            const Property::Entry& entry = (*iter);

            if (valRefresh)
                {
                    // just refresh values
                    mCtrList->SetItem(line, 1, entry.value);
                } else
                {
                    // complete refresh
                    int row = mCtrList->InsertItem(line, entry.name);
                    mCtrList->SetItem(row, 1, entry.value);
                }

            ++line;
        }

    mCtrList->Thaw();
}
