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
#include "main.h"

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include <wx/splash.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

#include "mainframe.h"
#include "sparkglcanvas.h"
#include "simspark.h"
#include "property.h"

#include <res/xpm_sparklogo.xpm>
#include <res/xpm_spark.xpm>

IMPLEMENT_APP(RsgEditApp)

using namespace boost;

bool RsgEditApp::OnInit()
{
    wxSplashScreen* splash = new wxSplashScreen
        (
         wxBitmap(xpm_sparklogo),
         wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT,
         0, NULL, -1, wxDefaultPosition, wxDefaultSize,
         wxSIMPLE_BORDER|wxSTAY_ON_TOP
         );
    wxYield();

    mSpark = shared_ptr<SimSpark>(new SimSpark());

    if (! mSpark->Init(argc, reinterpret_cast<char**>(argv)))
        {
            return false;
        }

    mProperty = shared_ptr<Property>(new Property);
    mainframe* frame = new mainframe(NULL, wxID_ANY, _T("RsgEdit"));

#ifdef __WXMSW__
    frame->SetIcon(wxIcon("Spark", wxBITMAP_TYPE_ICO_RESOURCE));
    ::wxHandleFatalExceptions(true);
#else
    wxIcon icon;
    icon.CopyFromBitmap(wxBitmap(xpm_spark));
    frame->SetIcon(icon);
#endif

    // Show the frame
    frame->Show(true);
    SetTopWindow(frame);

    splash->Destroy();

#ifdef __WXMSW__
    frame->Raise();
#endif
    return true;
}

shared_ptr<SimSpark> RsgEditApp::GetSpark()
{
    return mSpark;
}

shared_ptr<Property> RsgEditApp::GetProperty()
{
    return mProperty;
}

void RsgEditApp::OnFatalException()
{
}
