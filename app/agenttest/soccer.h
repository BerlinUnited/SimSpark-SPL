/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: soccer.h,v 1.1.2.3 2004/02/09 14:19:54 fruit Exp $

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
#ifndef SOCCER_H
#define SOCCER_H

#include "behavior.h"

class Soccer : public Behavior
{
public:
    Soccer();
    virtual ~Soccer();

    virtual void Think();
    virtual void Behave() = 0;

    void SetTeamName(std::string name) { mTeamName = name; }

protected:
    void CreateAgent();
    void InitAgent();
    void Kick(const float angle, const float power);
    void Drive(const salt::Vector3f& driveVec);

protected:
    /** the team name the agent uses with the init effector */
    std::string mTeamName;

    /** the number of received sensations */
    int mSensationCount;
};

DECLARE_ABSTRACTCLASS(Soccer)

#endif // SOCCER_H
