/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: survivalagentaspect.h,v 1.2.2.1 2003/11/19 19:07:39 rollmark Exp $

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
#ifndef SURVIVALAGENTASPECT_H__
#define SURVIVALAGENTASPECT_H__

#include <oxygen/agentaspect/agentaspect.h>
#include <kerosin/agentaspect/visionperceptor.h>
#include <oxygen/agentaspect/forceeffector.h>
#include "linesegmentperceptor.h"

class SurvivalAgentAspect : public oxygen::AgentAspect
{
        //
        // functions
        //
public:
        SurvivalAgentAspect();

        void Think(float deltaTime);

        // set parameters
        void SetWanderingStrength(float v)      {       mWanderingStrength = v; }
        void SetWanderingRate(float v)          {       mWanderingRate = v; }

        void SetIsSeeking(bool v)                       {       mIsSeeking = v; }


private:
        virtual void OnLink();

        salt::Vector3f  CalcSeekForce(const salt::Vector3f& seek);
        salt::Vector3f  CalcAvoidanceForce(const salt::Vector3f& avoid);
        salt::Vector3f  CalcWanderingForce();

        //
        // members
        //
private:
        // perceptors
        boost::shared_ptr<kerosin::VisionPerceptor>     mVisionPerceptor;
        boost::shared_ptr<LineSegmentPerceptor>         mLineSegmentPerceptor;

        // effectors
        boost::shared_ptr<oxygen::ForceEffector>       mForceEffector;

        // state of the agent
        bool                    mIsSeeking;
        salt::Vector3f  mSeekPoint;

        // wandering members

        //! radius of circle defining legal steering forces
        float           mWanderingStrength;
        //! offset of wandering strength
        float           mWanderingOffset;
        //! possible offset
        float           mWanderingRate;
        //! previous wandering force
        salt::Vector3f  mWanderingForce;
};

DECLARE_CLASS(SurvivalAgentAspect);

#endif //SURVIVALCONTROLASPECT_H__
