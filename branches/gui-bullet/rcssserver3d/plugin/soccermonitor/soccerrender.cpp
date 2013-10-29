/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerrender.cpp 222 2010-09-28 16:18:10Z sgvandijk $

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

   SoccerRender
*/
#include <sstream>
#include "soccerrender.h"
#include "soccermonitor.h"
#include "soccerinput.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/fontserver/fontserver.h>
#include <kerosin/fontserver/font.h>

using namespace kerosin;
using namespace boost;
using namespace std;

SoccerRender::SoccerRender() : kerosin::CustomRender()
{
}

SoccerRender::~SoccerRender()
{
}

void SoccerRender::OnLink()
{
    // get the FontServer
    mFontServer =
        static_pointer_cast<FontServer>(GetCore()->Get("/sys/server/font"));
    if (mFontServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SoccerRender) Unable to get FontServer\n";
        } else
        {
            string font = "fonts/VeraMono.ttf";
            int fontSize = 16;
            mFont = mFontServer->GetFont(font, fontSize);

            if (mFont.get() == 0)
                {
                    GetLog()->Error() << "(SoccerRender) Unable to get font "
                                      << font << " " << fontSize << "\n";
                }
        }

    // get the SoccerMonitor
    mMonitor = static_pointer_cast<SoccerMonitor>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient/SoccerMonitor"));

    if (mMonitor.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SoccerRender) Unable to get SoccerMonitor\n";
        }
    mInput = static_pointer_cast<SoccerInput>
        (GetCore()->Get("/sys/server/simulation/InputControl/SoccerInput"));

    if (mInput.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SoccerRender) Unable to get SoccerInput\n";
        }
}

void SoccerRender::OnUnlink()
{
    mFont.reset();
    mFontServer.reset();
    mMonitor.reset();
}


void SoccerRender::Render()
{
    if (
        (mFontServer.get() == 0) ||
        (mFont.get() == 0) ||
        (mMonitor.get() == 0)
        )
        {
            return;
        }

    stringstream ss_l, ss_c, ss_r, ss_m;
    
    ss_c.setf(ios_base::fixed,ios_base::floatfield);
    ss_c.precision(2);
    
    ss_l << mMonitor->GetScoreLeft() << " ";

    string nameleft = mMonitor->GetTeamNameLeft();

    if ( nameleft == "" )
    {
        nameleft = "<Left>";
    }
   
    ss_l << nameleft;

    ss_c << " (" << mMonitor->GetGameHalfString() << ") ";    
    ss_c << mMonitor->GetPlayModeString();
    ss_c << " t=" << mMonitor->GetTime() << " ";

    string nameright = mMonitor->GetTeamNameRight();

    if ( nameright == "" )
    {
        nameright = "<Right>";
    }
   
    ss_r << nameright;    
    ss_r << " " << mMonitor->GetScoreRight();

    int xPos;
    
    mFontServer->Begin();
    mFont->Bind();

    glDisable(GL_TEXTURE_2D);
    glColor4f(1,1,1,0.3);
    glBegin(GL_QUADS);
    glVertex2f(0,0);
    glVertex2f(1024, 0);
    glVertex2f(1024, 40);
    glVertex2f(0, 40);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    
    // draw left team info left justified
    glColor3f(0,0,0.8);
    mFont->DrawString(10, 0, ss_l.str().c_str());

    // draw right team info right justified
    // Window Width is mapped to 1024
    // use 1014 as base to keep a small right margin
    // FIXME: remove the magic numbers.
    glColor3f(0.8,0,0);
    xPos = int(1014-(mFont->GetStringWidth(ss_r.str().c_str())));
    mFont->DrawString(xPos, 0, ss_r.str().c_str());

    // draw game state info centered
    // Window Width is mapped to 1024
    // FIXME: remove the magic number.
    glColor3f(1,1,1);
    xPos = int((1024-(mFont->GetStringWidth(ss_c.str().c_str())))/2);
    mFont->DrawString(xPos, 0, ss_c.str().c_str());
    
    //ss_m << mInput->GetCmdMode();
    //mFont->DrawString(10, 30, ss_m.str().c_str());
    
    mFontServer->End();
}
