/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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

   InternalSoccerRender
*/
#include <sstream>
#include "internalsoccerrender.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/textureserver/textureserver.h>
#include <kerosin/textureserver/texture2d.h>
#include <kerosin/fontserver/fontserver.h>
#include <kerosin/fontserver/font.h>
#include <soccer/gamestateaspect/gamestateaspect.h>
#include <soccer/soccerbase/soccerbase.h>

using namespace kerosin;
using namespace boost;
using namespace std;

InternalSoccerRender::InternalSoccerRender() : kerosin::CustomRender()
{
}

InternalSoccerRender::~InternalSoccerRender()
{
}

void InternalSoccerRender::OnLink()
{
    // get the FontServer
    mFontServer =
        shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));

    if (mFontServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerRender) Unable to get FontServer\n";
        } else
        {
            string font = "fonts/VeraMono.ttf";
            int fontSize = 16;
            mFont = mFontServer->GetFont(font, fontSize);

            if (mFont.get() == 0)
                {
                    GetLog()->Error() << "(InternalSoccerRender) Unable to get font "
                                      << font << " " << fontSize << "\n";
                }
        }

    // get the TextureServer
    mTextureServer = shared_static_cast<TextureServer>(GetCore()->Get("/sys/server/texture"));

    if (mTextureServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerRender) Unable to get TextureServer\n";
        }

    // get the GameStateAspect
    mGameState = shared_dynamic_cast<GameStateAspect>
        (SoccerBase::GetControlAspect(*this, "GameStateAspect"));

    if (mGameState.get() == 0)
        {
            GetLog()->Error() << "ERROR: (InternalSoccerRender) Unable to get GameStateAspect\n";
        }
}

void InternalSoccerRender::OnUnlink()
{
    mFont.reset();
    mFontServer.reset();
    mTextureServer.reset();
    mGameState.reset();
}

void InternalSoccerRender::glEnable2D()
{
    GLint vPort[4];

   glGetIntegerv(GL_VIEWPORT, vPort);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   glOrtho(vPort[0], vPort[0]+vPort[2], vPort[1], vPort[1]+vPort[3], -1, 1);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   //glTranslatef(0.375, 0.375, 0.);

   glPushAttrib(GL_DEPTH_BUFFER_BIT);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void InternalSoccerRender::glDisable2D()
{
   glDisable(GL_BLEND);
   glPopAttrib();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

void InternalSoccerRender::Render()
{
    if (
        (mFontServer.get() == 0) ||
        (mFont.get() == 0) ||
        (mGameState.get() == 0)
        )
        {
            return;
        }

    stringstream ss_l, ss_c, ss_r;

    ss_c.setf(ios_base::fixed,ios_base::floatfield);
    ss_c.precision(2);

    ss_l << mGameState->GetScore(TI_LEFT) << " ";

    string nameleft = mGameState->GetTeamName(TI_LEFT);

    if ( nameleft == "" )
    {
        nameleft = "<Left>";
    }

    ss_l << nameleft;

    if (static_cast<int>(mGameState->GetGameHalf()) == 1)
    {
        ss_c << " (1st half) ";
    }
    else
    {
        ss_c << " (2nd half) ";
    }

    ss_c << SoccerBase::PlayMode2Str(mGameState->GetPlayMode());
    ss_c << " t=" << mGameState->GetTime() << " ";

    string nameright = mGameState->GetTeamName(TI_RIGHT);

    if ( nameright == "" )
    {
        nameright = "<Right>";
    }

    ss_r << nameright;
    ss_r << " " << mGameState->GetScore(TI_RIGHT);

    int xPos;

    mFontServer->Begin();
    mFont->Bind();

    // draw left team info left justified
    mFont->DrawString(10, 0, ss_l.str().c_str());

    // draw right team info right justified
    // Window Width is mapped to 1024
    // use 1014 as base to keep a small right margin
    // FIXME: remove the magic numbers.
    xPos = int(1014-(mFont->GetStringWidth(ss_r.str().c_str())));
    mFont->DrawString(xPos, 0, ss_r.str().c_str());

    // draw game state info centered
    // Window Width is mapped to 1024
    // FIXME: remove the magic number.
    xPos = int((1024-(mFont->GetStringWidth(ss_c.str().c_str())))/2);
    mFont->DrawString(xPos, 0, ss_c.str().c_str());

    mFontServer->End();

#if 0
    // get texture -- has to be square and power of 2 in size!!!
    shared_ptr<Texture2D> tex = shared_static_cast<Texture2D>
                                    (mTextureServer->GetTexture("test.tga"));

    glEnable2D();

    // draw some 2D elements
    tex->Enable();
    tex->Bind();

    glColor4f(1.0f,1.0f,1.0f,0.8f);
    glTranslatef(720.0, 16.0, 0.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(64.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(64.0f, 64.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f,  64.0f);
    glEnd();

    tex->Disable();

    glDisable2D();
#endif
}
