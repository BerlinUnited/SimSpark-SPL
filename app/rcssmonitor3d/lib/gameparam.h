/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: gameparam.h,v 1.3 2004/04/09 20:15:33 fruit Exp $

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

Main of the rcssmonitor3D application

This is a very simple 3D monitor example which will hopefully be
useful for the first steps of 3D SoccerServer
*/
#ifndef GAMEPARAM_H
#define GAMEPARAM_H

class GameParam
{
public:
    GameParam();
    virtual ~GameParam();

    const float& GetFieldLength() const;
    void SetFieldLength(float f);

    const float& GetFieldWidth() const;
    void SetFieldWidth(float f);

    const float& GetFieldHeight() const;
    void SetFieldHeight(float f);

    const float& GetBorderSize() const;
    void SetBorderSize(float f);

    const float& GetLineWidth() const;
    void SetLineWidth(float f);

    const float& GetGoalWidth() const;
    void SetGoalWidth(float f);

    const float& GetGoalDepth() const;
    void SetGoalDepth(float f);

    const float& GetGoalHeight() const;
    void SetGoalHeight(float f);

    const float& GetAgentMass() const;
    void SetAgentMass(float f);

    const float& GetAgentRadius() const;
    void SetAgentRadius(float f);

    const float& GetAgentMaxSpeed() const;
    void SetAgentMaxSpeed(float f);

    const float& GetBallRadius() const;
    void SetBallRadius(float f);

    const float& GetBallMass() const;
    void SetBallMass(float f);

protected:
    // field
    float mFieldLength;
    float mFieldWidth;
    float mFieldHeight;
    float mBorderSize;
    float mLineWidth;
    float mGoalWidth;
    float mGoalDepth;
    float mGoalHeight;

    // agent
    float mAgentMass;
    float mAgentRadius;
    float mAgentMaxSpeed;

    // ball
    float mBallRadius;
    float mBallMass;
};

#endif // GAMEPARAM_H
