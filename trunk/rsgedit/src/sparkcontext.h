/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkcontext.h,v 1.1 2008/02/24 16:20:23 rollmark Exp $

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
#ifndef SPARKCONTEXT_H__
#define SPARKCONTEXT_H__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace zeitgeist
{
    class Leaf;
}

class wxEvtHandler;
class wxMenu;
class wxScintilla;

class SparkContext
{
public:
    static SparkContext& GetInstance();
    ~SparkContext();

    void SetSelection(boost::weak_ptr<zeitgeist::Leaf> node);
    boost::weak_ptr<zeitgeist::Leaf> GetSelection() const;
    void ResetSelection();
    bool HasSelection();

    void SetEventHandler(wxEvtHandler* handler, int id);
    wxEvtHandler* GetEventHandler();
    int GetEventId();

    wxMenu* GetContextMenu(boost::weak_ptr<zeitgeist::Leaf> leaf);
    boost::weak_ptr<zeitgeist::Leaf> GetContextNode() const;

    wxMenu* GetContextMenu(wxScintilla* edit);

private:
    SparkContext();

protected:
    //! the current selection
    boost::weak_ptr<zeitgeist::Leaf> mSelectedNode;

    //! the node that context menu commands refer to
    boost::weak_ptr<zeitgeist::Leaf> mContextNode;

    //! assigned event notification target
    wxEvtHandler* mEventHandler;
    int mEventId;

};

#endif // SPARKCONTEXT_H__
