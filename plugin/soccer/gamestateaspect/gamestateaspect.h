/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.h,v 1.1.2.1 2004/01/08 12:43:36 rollmark Exp $

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
#ifndef GAMESTATEASPECT_H
#define GAMESTATEASPECT_H

#include <oxygen/controlaspect/controlaspect.h>

class GameStateAspect : public oxygen::ControlAspect
{
public:
    typedef enum _PlayMode
        {
            PM_BeforeKickOff,
            PM_TimeOver,
            PM_PlayOn,
            PM_KickOff_Left,
            PM_KickOff_Right,
            PM_FirstHalfOver
        } TPlayMode;

    typedef float TTime;

public:
    GameStateAspect();
    virtual ~GameStateAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** returns the current play mode */
    TPlayMode GetPlayMode() const;

    /** sets the current play mode */
    void SetPlayMode(TPlayMode mode);

    /** returns the current game time */
    TTime GetTime() const;

protected:
    /** the current play mode */
    TPlayMode mPlayMode;

    /** the current game time */
    TTime mTime;
};

DECLARE_CLASS(GameStateAspect);


#endif // GAMESTATEASPECT_H
