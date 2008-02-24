/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkglcanvas.h,v 1.1 2008/02/24 16:20:24 rollmark Exp $

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
#ifndef SPARKGLCANVAS_H__
#define SPARKGLCANVAS_H__

#ifdef WIN32
#include <winsock2.h>
#endif

#include <wx/glcanvas.h>
#include "sparkglrender.h"
#include "inputwx.h"

namespace kerosin
{
    class InputSystem;
    class RenderServer;
    class RenderNode;
}

class SparkGLCanvas: public wxGLCanvas
{
public:
    SparkGLCanvas( wxWindow *parent, wxWindowID id = wxID_ANY,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = 0, const wxString& name = _T("SparkGLCanvas") );

    SparkGLCanvas( wxWindow *parent, const SparkGLCanvas *other,
              wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize, long style = 0,
              const wxString& name = _T("SparkGLCanvas") );

    virtual ~SparkGLCanvas();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnEnterWindow(wxMouseEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftDClick(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMiddleDown(wxMouseEvent& event);
    void OnMiddleUp(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);
    void OnRightUp(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

    void Render(wxDC& dc, bool swapBuffers);
    void Reset();

    boost::weak_ptr<kerosin::RenderNode> PickNode();

protected:
    bool Init();
    void AddInput(const kerosin::Input& input);

protected:
    bool   mInit;
    SparkGLRender mRender;
    boost::shared_ptr<kerosin::InputSystem> mInputSystem;
    boost::shared_ptr<kerosin::RenderServer> mRenderServer;
    bool mMouseCaptured;

private:
    DECLARE_EVENT_TABLE()
};

#endif // SPARKGLCANVAS_H__
