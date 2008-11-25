/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkedit.cpp,v 1.1 2008/02/24 16:20:24 rollmark Exp $

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
#include "sparkedit.h"
#include <wx/colour.h>

#include <res/xpm_play.xpm>
#include <res/xpm_spark.xpm>
#include <res/xpm_ruby.xpm>
#include <res/xpm_rsg.xpm>

static wxColour COL_COMMENT(0,128,0);
static wxColour COL_KEYWORD(0,0,255);


// --- SparkEdit::EditEntry

const wxFileName& SparkEdit::EditEntry::GetFileName() const
{
    return fn;
}


void SparkEdit::EditEntry::SetFileName(const wxString& fname)
{
    fn = wxFileName(fname);
    fn.Normalize();
}

// --- SparkEdit


SparkEdit::SparkEdit()
    : mStartScript(0)
{
}

SparkEdit::~SparkEdit()
{
}

SparkEdit& SparkEdit::GetInstance()
{
    static SparkEdit theInstance;
    return theInstance;
}

wxScintilla* SparkEdit::Create(wxWindow* parent)
{
    wxScintilla* edit = new wxScintilla(parent, wxID_ANY);

    // setup basic styles

    edit->SetMarginWidth(0, 24);
    edit->SetMarginWidth(1, 0);
    edit->SetMarginWidth(2, 0);

    edit->SetMarginType(0, wxSCI_MARGIN_NUMBER);
    edit->StyleSetVisible(wxSCI_STYLE_LINENUMBER, true);

    edit->SetCaretLineBackground(wxColour(255,255,0));
    edit->SetCaretLineVisible(true);

    return edit;
}

wxString SparkEdit::GetTitle(wxScintilla* edit)
{
    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            assert(false);
            return wxString();
        }

    const EditEntry& entry = (*iter).second;
    bool markModified = false;
    return GetTitle(edit, entry, markModified);
}

wxString SparkEdit::GetTitle(wxScintilla* edit, const EditEntry& entry, bool markModified)
{
    wxString fname = entry.GetFileName().GetFullName();
    wxString title(fname.IsEmpty() ? wxString(wxT("<unnamed>")) : fname);

    if (
        (markModified) &&
        (edit->GetModify())
        )
        {
            title += wxT("*");
        }

    return title;
}

wxScintilla* SparkEdit::GetEdit(const wxString& fname, wxFlatNotebook* notebook, bool select)
{
    if (notebook == 0)
        {
            assert(false);
            return 0;
        }

    EditEntry newEntry;
    if (! fname.IsEmpty())
        {
            newEntry.SetFileName(fname);

            for (
                 TEditMap::iterator iter = mEditMap.begin();
                 iter != mEditMap.end();
                 ++iter
                 )
                {
                    EditEntry& entry = (*iter).second;
                    if (newEntry.GetFileName() == entry.GetFileName())
                        {
                            wxScintilla* edit = (*iter).first;
                            if (select)
                                {
                                    SelectEdit(notebook, edit);
                                }

                            return edit;
                        }
                }
        }

    wxScintilla* edit = Create(notebook);

    if (
        (! fname.IsEmpty()) &&
        (! LoadFile(edit, newEntry))
        )
        {
            edit->Destroy();
            return 0;
        }

    bool markModified = true;
    wxString title(GetTitle(edit, newEntry, markModified));
    notebook->AddPage(edit,title,select);

    if (select)
    {
        edit->SetFocus();
    }

    mEditMap[edit] = newEntry;

    UpdateIcon(notebook, edit, newEntry);
    return edit;
}

bool SparkEdit::LoadFile(wxScintilla* edit, EditEntry& entry)
{
    if (edit == 0)
        {
            assert(false);
            return false;
        }

    if (
        (! entry.GetFileName().FileExists()) ||
        (! edit->LoadFile(entry.GetFileName().GetFullPath()))
        )
        {
            edit->ClearAll();
            return false;
        }

    PrepareEdit(edit, entry);
    return true;
}

void SparkEdit::PrepareEdit(wxScintilla* edit, EditEntry& entry)
{
    if (edit == 0)
        {
            assert(false);
            return;
        }


    edit->SetLabel(entry.GetFileName().GetFullPath());
    wxString ext(entry.GetFileName().GetExt().Lower());

    if (ext == wxT("rsg"))
        {
            entry.type = ET_RSG;
        } else if (ext == wxT("rb"))
        {
            entry.type = ET_RB;
        } else if (
                   (ext == wxT("c")) ||
                   (ext == wxT("cpp")) ||
                   (ext == wxT("cxx")) ||
                   (ext == wxT("h")) ||
                   (ext == wxT("hpp"))
                   )
        {
            entry.type = ET_C;
        } else
        {
            entry.type = ET_TEXT;
        }

    switch (entry.type)
        {
        default:
            assert(false);
            // fall through

        case ET_TEXT:
            {
                edit->SetLexer(wxSCI_LEX_NULL);
                break;
            }

        case ET_RSG:
            {
                edit->SetLexer(wxSCI_LEX_LISP);
                edit->SetKeyWords(0,wxT(
                                        "node select pwd template define attach RubyDeltaScene RubySceneGraph "
                                        "nd sel pwd templ def attach RDS RSG"
                                        )
                                  );


                edit->StyleSetForeground(wxSCI_LISP_KEYWORD, COL_KEYWORD);
                edit->StyleSetForeground(wxSCI_LISP_COMMENT, COL_COMMENT);
                edit->StyleSetForeground(wxSCI_LISP_MULTI_COMMENT, COL_COMMENT);
                break;
            }

        case ET_RB:
            {
                edit->SetLexer(wxSCI_LEX_RUBY);
                edit->StyleSetForeground(wxSCI_RB_COMMENTLINE, COL_COMMENT);
                // todo: set ruby color styles
                break;
            }

        case ET_C:
            {
                edit->SetLexer(wxSCI_LEX_CPP);
                // todo: set c color styles
                break;
            }
    }

    edit->Colourise(0,-1);
}

void SparkEdit::PutEdit(wxScintilla* edit)
{
    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            assert(false);
            return;
        }

    if (edit == mStartScript)
        {
            mStartScript = 0;
        }

    mEditMap.erase(iter);
}

int SparkEdit::GetImageIndex(const wxScintilla* edit, const EditEntry& entry)
{
    if (edit == mStartScript)
        {
            return II_STARTSCRIPT;
        }

    switch (entry.type)
        {
        default:
            return -1;

        case ET_RB:
            return II_RUBY;

        case ET_RSG:
            return II_RSG;
        }
}

void SparkEdit::PrepareImageList(wxFlatNotebook* notebook)
{
   if (notebook == 0)
        {
            assert(false);
            return;
        }

   mNotebookImgList.Clear();
   mNotebookImgList.Add(wxBitmap(xpm_spark)); // II_SIMSPARK
   mNotebookImgList.Add(wxBitmap(xpm_play)); // II_STARTSCRIPT
   mNotebookImgList.Add(wxBitmap(xpm_ruby)); // II_RUBY
   mNotebookImgList.Add(wxBitmap(xpm_rsg)); // II_RSG
   notebook->SetImageList(&mNotebookImgList);
}

void SparkEdit::UpdateIcon(wxFlatNotebook* notebook, wxScintilla* edit, const EditEntry& entry)
{
    if (
        (notebook == 0) ||
        (edit == 0)
        )
        {
            assert(false);
            return;
        }

    int page = notebook->GetPageIndex(edit);
    if (page < 0)
    {
        assert(false);
        return;
    }

    notebook->SetPageImageIndex(page, GetImageIndex(edit, entry));
}

void SparkEdit::UpdateIcons(wxFlatNotebook* notebook)
{
    if (notebook == 0)
        {
            assert(false);
            return;
        }

    for (
         TEditMap::iterator iter = mEditMap.begin();
         iter != mEditMap.end();
         ++iter
         )
        {
            EditEntry& entry = (*iter).second;
            wxScintilla* edit = (*iter).first;
            UpdateIcon(notebook, edit, entry);
        }
}

void SparkEdit::UpdateTitles(wxFlatNotebook* notebook)
{
    for (
         TEditMap::iterator iter = mEditMap.begin();
         iter != mEditMap.end();
         ++iter
         )
        {
            wxScintilla* edit = (*iter).first;
            UpdateTitle(notebook, edit);
        }
}

void SparkEdit::UpdateTitle(wxFlatNotebook* notebook, wxScintilla* edit)
{
    if (notebook == 0)
        {
            assert(false);
            return;
        }

    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return;
        }

    int page = notebook->GetPageIndex(edit);
    if (page < 0)
    {
        assert(false);
        return;
    }

    const EditEntry& entry = (*iter).second;
    bool markModified = true;
    notebook->SetPageText(page, GetTitle(edit, entry, markModified));
}

void SparkEdit::SaveModified()
{
    for (
         TEditMap::iterator iter = mEditMap.begin();
         iter != mEditMap.end();
         ++iter
         )
        {
            wxScintilla* edit = (*iter).first;
            if (! edit->GetModify())
                {
                    continue;
                }

            EditEntry& entry = (*iter).second;
            SaveFile(edit, entry);
        }
}

void SparkEdit::SaveFile(wxScintilla* edit, const EditEntry& entry)
{
    if (edit == 0)
        {
            assert(false);
            return;
        }

    edit->SaveFile(entry.GetFileName().GetFullPath());
}

void SparkEdit::SaveFile(wxScintilla* edit)
{
    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return;
        }

    const EditEntry& entry = (*iter).second;
    SaveFile(edit, entry);
}

void SparkEdit::SaveFileAs(wxFlatNotebook* notebook, wxScintilla* edit, const wxString& fname)
{
    if (notebook == 0)
    {
        assert(false);
        return;
    }

    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return;
        }

    EditEntry& entry = (*iter).second;
    entry.SetFileName(fname);

    edit->SaveFile(entry.GetFileName().GetFullPath());
    PrepareEdit(edit, entry);
    UpdateIcon(notebook, edit, entry);
}

void SparkEdit::SetStartScript(wxFlatNotebook* notebook, wxScintilla* edit)
{
    mStartScript = edit;
    UpdateIcons(notebook);
}

wxScintilla* SparkEdit::GetStartScript()
{
    return mStartScript;
}

wxScintilla* SparkEdit::GetCurrentPage(wxFlatNotebook* notebook)
{
    if (notebook == 0)
        {
            assert(false);
            return 0;
        }

    return dynamic_cast<wxScintilla*>(notebook->GetCurrentPage());
}

wxString SparkEdit::GetFile(wxScintilla* edit)
{
    if (edit == 0)
    {
        assert(false);
        return wxString();
    }

    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return wxString();
        }

    const EditEntry& entry = (*iter).second;
    return entry.GetFileName().GetFullPath();
}

void SparkEdit::SetFile(wxScintilla* edit, wxFlatNotebook* notebook, const wxString& fname)
{
    if (edit == 0)
    {
        assert(false);
        return;
    }

    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return;
        }

    EditEntry& entry = (*iter).second;
    entry.SetFileName(fname);

    PrepareEdit(edit, entry);
    UpdateIcon(notebook, edit, entry);
}

wxString SparkEdit::GetStartScriptFile()
{
    return GetFile(mStartScript);
}

SparkEdit::EEditType SparkEdit::GetFileType(wxScintilla* edit)
{
    TEditMap::iterator iter = mEditMap.find(edit);
    if (iter == mEditMap.end())
        {
            return ET_NONE;
        }

    const EditEntry& entry = (*iter).second;
    return entry.type;
}

void SparkEdit::SelectEdit(wxFlatNotebook* notebook, wxScintilla* edit)
{
    if (notebook == 0)
        {
            assert(false);
            return;
        }

    int page = notebook->GetPageIndex(edit);
    if (page < 0)
    {
        assert(false);
        return;
    }

    notebook->SetSelection(page);
    edit->SetFocus();
}
