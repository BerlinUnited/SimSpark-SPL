/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef SPL_STATE_H
#define SPL_STATE_H

#include <gamestateaspect/gamestateaspect.h>

class SPLState : public GameStateAspect
{
public:
    SPLState();
    virtual ~SPLState();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** sets the current SPL state */
    void SetState(spl::TSPLState state);

    /** returns the current spl state */
    spl::TSPLState GetState() const { return mSPLState; }

    virtual void KickOff(TTeamIndex ti = TI_NONE);

    void UpdateKickOff();

    virtual std::string GetPlayModeStr() const;

    void UpdateTime(float deltaTime);

    TTime GetStateTime() const;

protected:
    /** state of SPL game */
    spl::TSPLState mSPLState;
};

DECLARE_CLASS(SPLState);

#endif // SPL_STATE_H
