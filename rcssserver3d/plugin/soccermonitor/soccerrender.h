/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

   SoccerRender
*/
#ifndef KEROSIN_SOCCERRENDER_H
#define KEROSIN_SOCCERRENDER_H

#include <kerosin/renderserver/customrender.h>

namespace kerosin
{
    class Font;
    class FontServer;
}

class SoccerMonitor;
class SoccerInput;

class SoccerRender : public kerosin::CustomRender
{
    //
    // Functions
    //
public:
    SoccerRender();
    virtual ~SoccerRender();

    virtual void Render();

    virtual void OnLink();
    virtual void OnUnlink();

protected:
    boost::shared_ptr<SoccerMonitor> mMonitor;
    boost::shared_ptr<kerosin::FontServer> mFontServer;
    boost::shared_ptr<kerosin::Font> mFont;
    boost::shared_ptr<SoccerInput> mInput;
};

DECLARE_CLASS(SoccerRender);

#endif //KEROSIN_SOCCERRENDER_H
