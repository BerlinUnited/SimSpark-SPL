/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkedit.h,v 1.1 2008/02/24 16:20:24 rollmark Exp $

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
#ifndef SPARKEDIT_H__
#define SPARKEDIT_H__

#include <wx/filename.h>
#include <wx/string.h>
#include <wx/wxscintilla.h>
#include <wx/wxflatnotebook/wxflatnotebook.h>

#include <map>

class SparkEdit
{
public:
    enum EEditType
        {
            ET_NONE,
            ET_RSG,
            ET_RB,
            ET_C,
            ET_TEXT
        };

    enum EImgIndex
        {
            II_NONE = -1,
            II_SIMSPARK = 0,
            II_STARTSCRIPT,
            II_RUBY,
            II_RSG
        };

    struct EditEntry
    {
    public:
        EEditType type;

    protected:
        wxFileName fn;
    public:
        EditEntry()
            : type(ET_RSG)
        {
        }

        const wxFileName& GetFileName() const;
        void SetFileName(const wxString& fname);
    };

    typedef std::map<wxScintilla*, EditEntry > TEditMap;

private:
    SparkEdit();

public:
    static SparkEdit& GetInstance();
    ~SparkEdit();

    wxScintilla* GetEdit(const wxString& fname, wxFlatNotebook* notebook, bool select);
    void PutEdit(wxScintilla* edit);
    void UpdateTitles(wxFlatNotebook* notebook);
    void UpdateTitle(wxFlatNotebook* notebook, wxScintilla* edit);
    void UpdateIcons(wxFlatNotebook* notebook);
    void SaveFile(wxScintilla* edit);
    void SaveFileAs(wxFlatNotebook* notebook, wxScintilla* edit, const wxString& fname);
    void SaveModified();
    void SetStartScript(wxFlatNotebook* notebook, wxScintilla* edit);
    wxScintilla* GetStartScript();
    EEditType GetFileType(wxScintilla* edit);
    wxString GetFile(wxScintilla* edit);
    void SetFile(wxScintilla* edit, wxFlatNotebook* notebook, const wxString& fname);
    wxString GetStartScriptFile();
    void PrepareImageList(wxFlatNotebook* notebook);
    void SelectEdit(wxFlatNotebook* notebook, wxScintilla* edit);
    wxString GetTitle(wxScintilla* edit);

    static wxScintilla* GetCurrentPage(wxFlatNotebook* notebook);

protected:
    wxScintilla* Create(wxWindow* parent);
    bool LoadFile(wxScintilla* edit, EditEntry& entry);
    void PrepareEdit(wxScintilla* edit, EditEntry& entry);
    wxString GetTitle(wxScintilla* edit, const EditEntry& entry, bool markModified);

    void UpdateIcon(wxFlatNotebook* notebook, wxScintilla* edit, const EditEntry& entry);
    int GetImageIndex(const wxScintilla* edit, const EditEntry& entry);

    void SaveFile(wxScintilla* edit, const EditEntry& entry);

protected:
    TEditMap mEditMap;
    wxFlatNotebookImageList mNotebookImgList;

    //! the editor holding the simulation start script
    wxScintilla* mStartScript;
};

#endif // SPARKEDIT_H__
