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
#ifndef KEROSIN_INTERNALSOCCERRENDER_H
#define KEROSIN_INTERNALSOCCERRENDER_H

#include <kerosin/renderserver/customrender.h>

namespace kerosin
{
    class Font;
    class FontServer;
    class TextureServer;
}

class GameStateAspect;

class InternalSoccerRender : public kerosin::CustomRender
{
    //
    // Functions
    //

public:
    InternalSoccerRender();
    virtual ~InternalSoccerRender();

    virtual void Render();

    virtual void OnLink();
    virtual void OnUnlink();

protected:

    void glEnable2D();
    void glDisable2D();

protected:
    
    /** cached reference to the font server */
    boost::shared_ptr<kerosin::FontServer> mFontServer;
    
    /** cached reference to the font we want to use */
    boost::shared_ptr<kerosin::Font> mFont;

    /** cached reference to the texture server */
    boost::shared_ptr<kerosin::TextureServer> mTextureServer;

    /** cached reference to the game state info */
    boost::shared_ptr<GameStateAspect> mGameState;    
};

DECLARE_CLASS(InternalSoccerRender);

#endif //KEROSIN_INTERNALSOCCERRENDER_H
