/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerrender.cpp,v 1.2 2004/12/30 15:57:40 rollmark Exp $

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
        shared_static_cast<FontServer>(GetCore()->Get("/sys/server/font"));
    if (mFontServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SoccerRender) Unable to get FontServer\n";
        } else
        {
            string font = "VeraMono.ttf";
            int fontSize = 24;
            mFont = mFontServer->GetFont(font, fontSize);

            if (mFont.get() == 0)
                {
                    GetLog()->Error() << "(SoccerRender) Unable to get font "
                                      << font << " " << fontSize << "\n";
                }
        }

    // get the SoccerMonitor
    mMonitor = shared_static_cast<SoccerMonitor>
        (GetCore()->Get("/sys/server/simulation/SparkMonitorClient/SoccerMonitor"));

    if (mMonitor.get() == 0)
        {
            GetLog()->Error() << "ERROR: (SoccerRender) Unable to get SoccerMonitor\n";
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

    stringstream ss;
    ss << "(" << mMonitor->GetGameHalfString() << ") ";
    ss << mMonitor->GetPlayModeString();
    ss << " t=" << mMonitor->GetTime();

    mFontServer->Begin();
    mFont->Bind();
    glColor3f(1,1,1);
    mFont->DrawString(0, 0, ss.str().c_str());
    mFontServer->End();
}
