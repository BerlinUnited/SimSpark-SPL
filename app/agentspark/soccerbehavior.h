/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: soccerbehavior.h,v 1.3 2004/12/18 14:32:13 rollmark Exp $

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
#ifndef SOCCERBEHAVIOR_H
#define SOCCERBEHAVIOR_H

#include "behavior.h"
#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <zeitgeist/zeitgeist.h>

class SoccerBehavior : public Behavior
{
public:
    struct VisionSense
    {
        /** distance perceptor to object */
        float distance;

        /** theta is the angle in the X-Y (horizontal) plane */
        float theta;

        /** phi is the latitude angle */
        float phi;

        VisionSense() : distance(0), theta(0), phi(0) {};
    };

    enum VisionObject
    {
        VO_BALL   = 0,
    };

public:
    SoccerBehavior();
    virtual ~SoccerBehavior();

    virtual std::string Init();
    virtual std::string Think(const std::string& message);

    const VisionSense& GetVisionSense(VisionObject obj) const;

protected:
    void ParseObjectVision(const oxygen::Predicate& predicate);
    void ParseVision(const oxygen::Predicate& predicate);
    void SetupVisionObjectMap();

    std::string TurnLeft() const;
    std::string TurnRight() const;
    std::string Forward() const;
    std::string Kick() const;
    std::string SeekBall() const;

    salt::Vector3f GetPosition(const VisionSense& sense) const;
    salt::Vector3f GetPosition(const VisionObject& obj) const;
    salt::Vector3f GetDriveVec(const VisionSense& vision) const;

protected:
    zeitgeist::Zeitgeist mZG;
    boost::shared_ptr<oxygen::BaseParser> mParser;

    // mapping from vision object to VisionSense
    typedef std::map<VisionObject, VisionSense> TVisionMap;
    TVisionMap mVisionMap;

    // mapping from <ObjectName>+<ID> to VisionObject
    typedef std::map<std::string, VisionObject> TVisionObjectMap;
    TVisionObjectMap mVisionObjectMap;

    // the players position
    salt::Vector3f mMyPos;
};

#endif // SOCCERBEHAVIOR_H
