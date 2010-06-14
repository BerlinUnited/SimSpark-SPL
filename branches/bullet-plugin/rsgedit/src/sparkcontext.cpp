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
#include "sparkcontext.h"
#include "sparkcontextevent.h"
#include "constants.h"
#include <wx/menu.h>
#include <wx/filename.h>
#include <zeitgeist/leaf.h>
#include <oxygen/sceneserver/scenedict.h>
#include <src/sparkedit.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;

SparkContext::SparkContext()
    : mEventHandler(0), mEventId(1)
{
}

SparkContext::~SparkContext()
{
}

SparkContext& SparkContext::GetInstance()
{
    static SparkContext theInstance;
    return theInstance;
}

void SparkContext::SetSelection(weak_ptr<Leaf> node)
{
    mSelectedNode = node;

    if (mEventHandler == 0)
        {
            return;
        }

    SparkContextEvent event(mEventId,SparkContextEvent::T_SELECTION_CHANGED);
    wxPostEvent(mEventHandler,event);
}

boost::weak_ptr<Leaf> SparkContext::GetSelection() const
{
    return mSelectedNode;
}

void SparkContext::ResetSelection()
{
    SetSelection(weak_ptr<Leaf>());
}

bool SparkContext::HasSelection()
{
    return (! mSelectedNode.expired());
}

void SparkContext::SetEventHandler(wxEvtHandler* handler, int id)
{
    mEventHandler = handler;
    mEventId = id;
}

wxEvtHandler* SparkContext::GetEventHandler()
{
    return mEventHandler;
}

int SparkContext::GetEventId()
{
    return mEventId;
}

wxMenu* SparkContext::GetContextMenu(wxScintilla* edit)
{
    if (edit == 0)
    {
        assert(false);
        return 0;
    }

    wxString path(SparkEdit::GetInstance().GetFile(edit));
    wxString fname(wxFileName(path).GetFullName());

    wxMenu* menu(new wxMenu(fname, 9));

    menu->Append(ID_FILE_SAVE, wxT("&Save " + fname));
    menu->Append(ID_TAB_CLOSE, wxT("&Close tab"));
    menu->Append(ID_TAB_CLOSE_OTHER, wxT("Close other tabs"));

    if (edit != SparkEdit::GetInstance().GetStartScript())
    {
        menu->AppendSeparator();
        menu->Append(ID_TAB_STARTSCRIPT, wxT("Set ") + fname + wxT(" as Startscript"));
    }

    return menu;
}

wxMenu* SparkContext::GetContextMenu(weak_ptr<Leaf> leaf)
{
    if (leaf.expired())
        {
            assert(false);
            return 0;
        }

    mContextNode = leaf;
    wxMenu* menu(new wxMenu(wxString::FromAscii(leaf.lock()->GetFullPath().c_str()), 0));

    menu->Append(ID_CONTEXT_COPY_PATH, wxT("&Copy node path"));

    const SceneDict::FileRef* ref = SceneDict::GetInstance().Lookup(leaf);
    if (ref != 0)
    {
        menu->AppendSeparator();
        menu->Append(ID_CONTEXT_COPY_FILENAME, wxT("C&opy filename"));
        menu->Append(ID_CONTEXT_EDIT_SOURCE, wxT("&Edit source file"));
    }

    menu->AppendSeparator();
    menu->Append(ID_CONTEXT_PROPERTIES, wxT("&Properties"));
    menu->Append(ID_CONTEXT_KINEMATIC, wxT("&Kinematic"));


    return menu;
}

weak_ptr<Leaf> SparkContext::GetContextNode() const
{
    return mContextNode;
}
