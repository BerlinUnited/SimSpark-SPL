/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: survivalagentaspect.cpp,v 1.2.2.1 2003/11/19 19:07:38 rollmark Exp $

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
#include "survivalagentaspect.h"
#include "survivalcontrolaspect.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/physicsserver/body.h>

using namespace boost;
using namespace oxygen;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;

SurvivalAgentAspect::SurvivalAgentAspect()
{
        mWanderingForce.Set(0,0,0);
        mIsSeeking = false;

        mWanderingStrength = 10.0f;
        mWanderingOffset   = 4.0f;
        mWanderingRate = 2.0f;
}

void SurvivalAgentAspect::Think(float deltaTime)
{
        Vector3f force(0,0,0);

        Perceptor::TDictionary dictionary;

        // try out vision perceptor
        Vector3f v = GetVelocity();
        v.y() = 0;
        mVisionPerceptor->SetViewDirection(v);
        if (mVisionPerceptor->Percept(dictionary))
        {
                TLeafList visible = any_cast<TLeafList>(dictionary["visibleObjects"]);
                TLeafList::iterator i;
                for (i = visible.begin(); i != visible.end(); ++i)
                {
                        shared_ptr<BaseNode> node = shared_static_cast<BaseNode>(*i);
                        //printf("node: %s\n", node->GetName().c_str());
                        if (node->GetName().compare("food") == 0)
                        {
                                //printf("%s: food\n", GetFullPath().c_str());
                                //mSeekPoint.Dump();
                                mIsSeeking = true;
                                mSeekPoint = node->GetWorldTransform().Pos();
                        }
                }
        }

        if (!mIsSeeking)
        {
                Vector3f avoidance(0,0,0);
                // sense environment using perceptors
                mLineSegmentPerceptor->SetLineSegment(GetWorldTransform().Pos(), Vector3f(7, 0, 0)+GetWorldTransform().Pos());
                mLineSegmentPerceptor->Percept(dictionary);
                if (any_cast<bool>(dictionary["hit"]) == true)
                {
                        avoidance+=CalcAvoidanceForce(any_cast<Vector3f>(dictionary["p"]));
                }
                mLineSegmentPerceptor->SetLineSegment(GetWorldTransform().Pos(), Vector3f(-7, 0, 0)+GetWorldTransform().Pos());
                mLineSegmentPerceptor->Percept(dictionary);
                if (any_cast<bool>(dictionary["hit"]) == true)
                {
                        avoidance+=CalcAvoidanceForce(any_cast<Vector3f>(dictionary["p"]));
                }
                mLineSegmentPerceptor->SetLineSegment(GetWorldTransform().Pos(), Vector3f(0, 0, 7)+GetWorldTransform().Pos());
                mLineSegmentPerceptor->Percept(dictionary);
                if (any_cast<bool>(dictionary["hit"]) == true)
                {
                        avoidance+=CalcAvoidanceForce(any_cast<Vector3f>(dictionary["p"]));
                }
                mLineSegmentPerceptor->SetLineSegment(GetWorldTransform().Pos(), Vector3f(0, 0, -7)+GetWorldTransform().Pos());
                mLineSegmentPerceptor->Percept(dictionary);
                if (any_cast<bool>(dictionary["hit"]) == true)
                {
                        avoidance+=CalcAvoidanceForce(any_cast<Vector3f>(dictionary["p"]));
                }

                force = 3.0f*avoidance + CalcWanderingForce();
        }
        else
        {
                force = CalcSeekForce(mSeekPoint);
        }

        if (mIsSeeking && (mSeekPoint-GetWorldTransform().Pos()).Length() < 1.5f)
        {
                mIsSeeking = false;
        }

        // use force effector to apply steering force
        mForceEffector->AddForce(force/deltaTime);
        shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(GetParent()));
        mForceEffector->Perform(parent, deltaTime);
}

void SurvivalAgentAspect::OnLink()
{
        // locate control aspect
        shared_ptr<SurvivalControlAspect> control = shared_static_cast<SurvivalControlAspect>(GetScene()->GetChildOfClass("SurvivalControlAspect"));

        if (!control)
        {
                GetLog()->Error() << "ERROR: Could not locate SurvivalControlAspect..." << endl;
                return;
        }

        // we now have the control aspect

        shared_ptr<AgentAspect> agent = shared_static_cast<AgentAspect>(make_shared(GetSelf()));
        // request some effectors
        mForceEffector = shared_static_cast<ForceEffector>(control->RequestEffector(agent, "kerosin/ForceEffector"));

        /*if (GetName().compare("fast")==0)
        {
                mForceEffector->SetMaxForce(0.1f);
        }*/

        // request some perceptors
        mVisionPerceptor                = shared_static_cast<VisionPerceptor>(control->RequestPerceptor(agent, "kerosin/VisionPerceptor"));
        mLineSegmentPerceptor   = shared_static_cast<LineSegmentPerceptor>(control->RequestPerceptor(agent, "LineSegmentPerceptor"));
}

salt::Vector3f SurvivalAgentAspect::CalcSeekForce(const salt::Vector3f& seek)
{
        Vector3f v = GetVelocity();
        Vector3f direction = seek - GetWorldTransform().Pos();
        Vector3f force = (direction - v);
        force.Normalize();
        return force * 100.0f;
}

salt::Vector3f SurvivalAgentAspect::CalcAvoidanceForce(const salt::Vector3f& avoid)
{
        Vector3f v = GetVelocity();
        Vector3f direction = avoid - GetWorldTransform().Pos();
        Vector3f force = (-direction - v);
        force.Normalize();
        return force * 100.0f;
}

salt::Vector3f SurvivalAgentAspect::CalcWanderingForce()
{
        Vector3f v = GetVelocity();

        v.y() = 0.0f;

        if (v.Length() < 0.01f)
                v.x() = 1.0f;

        // v == translation of wandering strength circle
        v.Normalize();
        v*=mWanderingOffset;

        Vector3f random;

        random.RandomUnitVector();
        random.y() = 0.0f;
        random.Normalize();

        mWanderingForce += mWanderingRate*random;
        // restrict to 2D
        mWanderingForce.y() = 0;

        // translate force, so that wandering strength circle is in the origin
        mWanderingForce -= v;

        //limit magnitude of force to wandering strength
        if (mWanderingForce.Length() > mWanderingStrength)
                mWanderingForce = mWanderingForce.Normalize()*mWanderingStrength;

        mWanderingForce+=v;

        return mWanderingForce;
}
