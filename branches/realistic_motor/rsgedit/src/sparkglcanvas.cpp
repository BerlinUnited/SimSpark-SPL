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
#include "sparkglcanvas.h"
#include <wx/dcclient.h>
#include "main.h"

#include "simspark.h"
#include "sparkcontext.h"

//! wxWidgets and zeitgeist both use a 'DECLARE_CLASS' macro
#undef DECLARE_CLASS

#include <zeitgeist/logserver/logserver.h>
#include <kerosin/inputserver/inputserver.h>
#include <kerosin/inputserver/inputsystem.h>
#include <kerosin/renderserver/renderserver.h>
#include <kerosin/renderserver/rendernode.h>

BEGIN_EVENT_TABLE(SparkGLCanvas, wxGLCanvas)
    EVT_SIZE(SparkGLCanvas::OnSize)
    EVT_PAINT(SparkGLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(SparkGLCanvas::OnEraseBackground)
    EVT_ENTER_WINDOW(SparkGLCanvas::OnEnterWindow)
    EVT_KEY_DOWN(SparkGLCanvas::OnKeyDown)
    EVT_KEY_UP(SparkGLCanvas::OnKeyUp)
    EVT_LEFT_DOWN(SparkGLCanvas::OnLeftDown)
    EVT_LEFT_UP(SparkGLCanvas::OnLeftUp)
    EVT_LEFT_DCLICK(SparkGLCanvas::OnLeftDClick)
    EVT_MIDDLE_DOWN(SparkGLCanvas::OnMiddleDown)
    EVT_MIDDLE_UP(SparkGLCanvas::OnMiddleUp)
    EVT_RIGHT_DOWN(SparkGLCanvas::OnRightDown)
    EVT_RIGHT_UP(SparkGLCanvas::OnRightUp)
    EVT_MOTION(SparkGLCanvas::OnMouseMove)
    EVT_MOUSE_CAPTURE_LOST(SparkGLCanvas::OnMouseCaptureLost)
END_EVENT_TABLE()

using namespace boost;
using namespace zeitgeist;
using namespace kerosin;
using namespace salt;

SparkGLCanvas::SparkGLCanvas(wxWindow *parent, wxWindowID id,
                             const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxGLCanvas(parent, (wxGLCanvas*) NULL, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name )
{
    mMouseCaptured = false;
    mInit = false;
}

SparkGLCanvas::SparkGLCanvas(wxWindow *parent, const SparkGLCanvas *other,
                             wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
                             const wxString& name )
    : wxGLCanvas(parent, other->GetContext(), id, pos, size, style|wxFULL_REPAINT_ON_RESIZE , name)
{
    mMouseCaptured = false;
    mInit = false;
}

SparkGLCanvas::~SparkGLCanvas()
{
}

void SparkGLCanvas::Reset()
{
    mInit = false;
}

void SparkGLCanvas::Render(wxDC& /*dc*/, bool swapBuffers)
{
#ifndef __WXMOTIF__
    if (!GetContext()) return;
#endif

    SetCurrent();

    // Init once, but after SetCurrent
    if (!mInit)
        {
            Init();
            mInit = true;
        }

    int w, h;
    GetClientSize(&w, &h);
    mRender.Render(w,h);

    glFlush();

    if (swapBuffers)
    {
        SwapBuffers();
    }
}

weak_ptr<RenderNode> SparkGLCanvas::PickNode()
{
    shared_ptr<SimSpark> spark = wxGetApp().GetSpark();
    if (
        (spark.get() == 0) ||
        (mRenderServer.get() == 0)
        )
    {
        assert(false);
        return weak_ptr<RenderNode>();
    }

    wxClientDC dc(this);

    wxPoint ptMouse = wxGetMousePosition();
    ScreenToClient(&ptMouse.x, &ptMouse.y);

    double pickRange = 5.0;
    mRenderServer->EnablePicking(true, Vector2f(ptMouse.x, ptMouse.y), pickRange);

    {
        bool swapBuffers = false;
        Render(dc, swapBuffers);
    }

    mRenderServer->DisablePicking();

    weak_ptr<RenderNode> pickedNode = mRenderServer->GetPickedNode();
    if (! pickedNode.expired())
        {
            spark->GetLog()->Normal()
                << "(SparkGLCanvas) SparkGLCanvas::PickNode picked " << pickedNode.lock()->GetFullPath() << std::endl;
        }

    {
        bool swapBuffers = true;
        Render(dc, swapBuffers);
    }

    return pickedNode;
}

void SparkGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    wxPaintDC dc(this);

    bool swapBuffers = true;
    Render(dc, swapBuffers);
}

void SparkGLCanvas::OnSize(wxSizeEvent& event)
{
    // this is also necessary to update the context on some platforms
    wxGLCanvas::OnSize(event);
}

void SparkGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing.
}

void SparkGLCanvas::OnEnterWindow( wxMouseEvent& WXUNUSED(event) )
{
    SetFocus();
}

bool SparkGLCanvas::Init()
{
    shared_ptr<SimSpark> spark = wxGetApp().GetSpark();
    if (spark.get() == 0)
        {
            return false;
        }

    shared_ptr<InputServer> inputServer = spark->GetInputServer();
    if (inputServer.get() != 0)
        {
            mInputSystem = inputServer->GetInputSystem();
        } else
        {
            spark->GetLog()->Error()
                << "(SparkGLCanvas) ERROR: InputSystem not found" << std::endl;
        }

    mRenderServer = shared_dynamic_cast<RenderServer>
        (spark->GetCore()->Get("/sys/server/render"));

    if (mRenderServer.get() == 0)
        {
            spark->GetLog()->Error()
                << "(SparkGLCanvas) ERROR: RenderServer not found\n";
        }

    mRender.Init(spark);
    return true;
}

void SparkGLCanvas::OnKeyDown(wxKeyEvent& event)
{
    event.Skip();
    AddInput(wxInput(event,true));
}

void SparkGLCanvas::OnKeyUp(wxKeyEvent& event)
{
    event.Skip();

    // handle rsgedit specific keys
    if (
        (event.GetModifiers() == 0) &&
        (event.GetKeyCode() == WXK_ESCAPE)
        )
    {
        wxClientDC dc(this);
        SparkContext::GetInstance().ResetSelection();
    }

    AddInput(wxInput(event,false));
}

void SparkGLCanvas::OnLeftDown(wxMouseEvent& event)
{
    event.Skip();

    Input input(Input::eButton, Input::IC_MOUSE_LEFT, 1);
    input.mData.l = 1;

    AddInput(input);
}

void SparkGLCanvas::OnLeftDClick(wxMouseEvent& event)
{
    SparkContext::GetInstance().SetSelection(PickNode());
    event.Skip();
}

void SparkGLCanvas::OnLeftUp(wxMouseEvent& event)
{
    event.Skip();

    Input input(Input::eButton, Input::IC_MOUSE_LEFT);
    input.mData.l = 0;

    AddInput(input);
}

void SparkGLCanvas::OnMiddleDown(wxMouseEvent& event)
{
    event.Skip();
    Input input(Input::eButton, Input::IC_MOUSE_MIDDLE);
    input.mData.l = 1;

    AddInput(input);
}

void SparkGLCanvas::OnMiddleUp(wxMouseEvent& event)
{
    event.Skip();

    Input input(Input::eButton, Input::IC_MOUSE_MIDDLE);
    input.mData.l = 0;

    AddInput(input);
}

void SparkGLCanvas::OnRightDown(wxMouseEvent& event)
{
    event.Skip();

    if (SparkContext::GetInstance().HasSelection())
        {
            weak_ptr<RenderNode> node = PickNode();
            shared_ptr<Leaf> selection =
                SparkContext::GetInstance().GetSelection().lock();

            if (
                (! node.expired()) &&
                (node.lock() == selection)
                )
                {
                    wxMenu* menu =
                        SparkContext::GetInstance().GetContextMenu(selection);

                    if (menu != 0)
                        {
                            PopupMenu(menu);
                            return;
                        }
                }
        }

    Input input(Input::eButton, Input::IC_MOUSE_RIGHT);
    input.mData.l = 1;

    AddInput(input);
}

void SparkGLCanvas::OnRightUp(wxMouseEvent& event)
{
    event.Skip();

    Input input(Input::eButton, Input::IC_MOUSE_RIGHT, 0);
    input.mData.l = 0;

    AddInput(input);
}

void SparkGLCanvas::OnMouseMove(wxMouseEvent& event)
{
    event.Skip();

    static bool first = true;
    static long lastX = 0;
    static long lastY = 0;

    const double fkt = 0.5;

    if (first)
        {
            first = false;
            lastX = event.GetX();
            lastY = event.GetY();
            return;
        }

    {
        Input input(Input::eAxis, Input::IC_AXISX);
        input.mData.l = static_cast<long>((event.GetX() - lastX) * fkt);
        AddInput(input);
    }

    {
        Input input(Input::eAxis, Input::IC_AXISY);
        input.mData.l = static_cast<long>((event.GetY() - lastY) * fkt);
        AddInput(input);
    }

    lastX = event.GetX();
    lastY = event.GetY();

    if (
        (event.LeftIsDown()) ||
        (event.MiddleIsDown()) ||
        (event.RightIsDown())
        )
    {
        if (! mMouseCaptured)
        {
            CaptureMouse();
            mMouseCaptured = true;
        }
    } else
    {
        if (mMouseCaptured)
        {
            ReleaseMouse();
            mMouseCaptured = false;
        }
    }
}

void SparkGLCanvas::AddInput(const kerosin::Input& input)
{
    if (mInputSystem.get() == 0)
        {
            return;
        }

    mInputSystem->AddInput(input);
}

void SparkGLCanvas::OnMouseCaptureLost(wxMouseCaptureLostEvent& /*event*/)
{
    mMouseCaptured = false;
}
