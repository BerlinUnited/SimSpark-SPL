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
*/
#include "restrictedvisionperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <soccer/soccerbase/soccerbase.h>
#include <salt/gmath.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

RestrictedVisionPerceptor::RestrictedVisionPerceptor() : Perceptor(),
                                     mSenseMyPos(false),
                                     mAddNoise(true),
                                     mStaticSenseAxis(true)
{
    // set predicate name
    SetPredicateName("See");
    // set some default noise values
    SetNoiseParams(0.0965f, 0.1225f, 0.1480f, 0.005f);
    SetViewCones(90,90);
    SetPanRange(-90,90);
    SetTiltRange(-20,20);
    SetPanTilt(0,0);
}

RestrictedVisionPerceptor::~RestrictedVisionPerceptor()
{
    mDistRng.reset();
    mPhiRng.reset();
    mThetaRng.reset();
}

void
RestrictedVisionPerceptor::SetNoiseParams(float sigma_dist, float sigma_phi,
                                float sigma_theta, float cal_error_abs)
{
    mSigmaDist = sigma_dist;
    mSigmaPhi = sigma_phi;
    mSigmaTheta = sigma_theta;
    mCalErrorAbs = cal_error_abs;

    NormalRngPtr rng1(new salt::NormalRNG<>(0.0,sigma_dist));
    mDistRng = rng1;
    NormalRngPtr rng2(new salt::NormalRNG<>(0.0,sigma_phi));
    mPhiRng = rng2;
    NormalRngPtr rng3(new salt::NormalRNG<>(0.0,sigma_theta));
    mThetaRng = rng3;

    salt::UniformRNG<float> rng4(-mCalErrorAbs,mCalErrorAbs);
    mError = salt::Vector3f(rng4(),rng4(),rng4());
}

void
RestrictedVisionPerceptor::SetViewCones(unsigned int hAngle, unsigned int vAngle)
{
    mHViewCone = hAngle;
    mVViewCone = vAngle;
}

void
RestrictedVisionPerceptor::SetPanRange(int lower, int upper)
{
    /* The total pan range is 360 degrees, starting at -180 degrees
     * and ending at 180 degrees. For restricting this range:
     * - the normal case is: lower < upper, which means that
     *   the horizontal pan angles should be between lower and upper.
     * - the not so normal case is upper < lower, which means that
     *   the pan angles should be *either* smaller than 180 and > lower, *or*
     *   they should be greater than -180 and < upper.
     */
    mPanLower = static_cast<int>(gNormalizeDeg(lower));
    mPanUpper = static_cast<int>(gNormalizeDeg(upper));
}

void
RestrictedVisionPerceptor::SetTiltRange(int lower, int upper)
{
    mTiltLower = static_cast<int>(gNormalizeDeg(lower));
    mTiltUpper = static_cast<int>(gNormalizeDeg(upper));
}

// this should really go to gmath.h for the full release
template <class TYPE1, class TYPE2>
f_inline TYPE1 gClampAngleDeg(TYPE1 &val, TYPE2 min, TYPE2 max)
{
    val = gNormalizeDeg(val);
    if (min <= max)
    {
        if (val<min) val=min;
        if (val>max) val=max;
    } else {
        if (val>=min || val<=max) return val;
        if (val>=(min+max)/2.0) val = min; else val = max;
    }
    return val;
}



void
RestrictedVisionPerceptor::SetPanTilt(float pan, float tilt)
{
    pan = gNormalizeDeg(pan);
    mPan = gClampAngleDeg(pan,mPanLower,mPanUpper);
    tilt = gNormalizeDeg(tilt);
    mTilt = gClampAngleDeg(tilt,mTiltLower,mTiltUpper);
}

void
RestrictedVisionPerceptor::ChangePanTilt(float pan, float tilt)
{
    SetPanTilt(mPan + pan, mTilt + tilt);
}

float
RestrictedVisionPerceptor::GetPan() const
{
    return mPan;
}

float
RestrictedVisionPerceptor::GetTilt() const
{
    return mTilt;
}

void
RestrictedVisionPerceptor::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
//     SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetActiveScene(*this,mActiveScene);
    
    shared_ptr<AgentAspect> agent_aspect =
        FindParentSupportingClass<AgentAspect>().lock();
    if (agent_aspect == 0)
    {
        GetLog()->Error()
            << "Error: (RestrictedVisionPerceptor) cannot find AgentAspect.\n";
    }
    else
    {
        mAgentAspect = agent_aspect;
        agent_aspect = agent_aspect->FindParentSupportingClass<AgentAspect>().lock();
        if (agent_aspect != 0)
        {
            mAgentAspect = agent_aspect;
        }
        
        mAgentState = shared_static_cast<AgentState>
            (mAgentAspect->GetChildOfClass("AgentState",true));
        if (mAgentState == 0)
        {
            GetLog()->Error()
                << "Error: (RestrictedVisionPerceptor) cannot find AgentState.\n";
        }
    }
}

void
RestrictedVisionPerceptor::OnUnlink()
{
    mDistRng.reset();
    mPhiRng.reset();
    mThetaRng.reset();
    mTransformParent.reset();
    mAgentAspect.reset();
    mAgentState.reset();
    mActiveScene.reset();
}

void
RestrictedVisionPerceptor::AddNoise(bool add_noise)
{
    mAddNoise = add_noise;
}

void
RestrictedVisionPerceptor::SetStaticSenseAxis(bool static_axis)
{
    mStaticSenseAxis = static_axis;
}

bool
RestrictedVisionPerceptor::ConstructInternal()
{
    mRay = shared_static_cast<RayCollider>
        (GetCore()->New("oxygen/RayCollider"));

    if (mRay.get() == 0)
    {
        GetLog()->Error() << "Error: (RestrictedVisionPerceptor) cannot create Raycollider. "
                          << "occlusion check disabled\n";
    }

    return true;
}

void
RestrictedVisionPerceptor::SetupVisibleNodes(TNodeObjectsMap& visibleNodes)
{
    TLeafList objectList;
    //mActiveScene->ListChildrenSupportingClass<ObjectState>(objectList, true);
    mActiveScene->GetChildrenOfClass("ObjectState", objectList, true);

    salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();

    for (TLeafList::iterator i = objectList.begin();
        i != objectList.end(); ++i)
    {
        ObjectData od;

        od.mObj = shared_static_cast<ObjectState>(*i);

        shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>(mActiveScene);
        shared_ptr<AgentAspect> agent_aspect =
                od.mObj->FindParentSupportingClass<AgentAspect>().lock();
        if (agent_aspect != 0)
        {
            shared_ptr<AgentAspect> aspect =
                agent_aspect->FindParentSupportingClass<AgentAspect>().lock();
            if (aspect != 0)
            {
                agent_aspect = aspect;
            }

            // if (agent_aspect == mAgentAspect) // exclude node of self
            // {
            //     continue;
            // }

            // GetLog()->Normal()
            //    << "skipping agentAspect " << agent_aspect->GetFullPath() << std::endl;
            node = shared_dynamic_cast<BaseNode>(agent_aspect);
        }

        if (od.mObj.get() == 0)
        {
            GetLog()->Error() << "Error: (RestrictedVisionPerceptor) skipped: "
                << (*i)->GetName() << "\n";
            continue; // this should never happen
        }

        shared_ptr<Transform> j = od.mObj->GetTransformParent();
        
        if (j.get() == 0)
        {
            continue; // this should never happen
        }

        od.mRelPos = j->GetWorldTransform().Pos() - myPos;
        od.mDist   = od.mRelPos.Length();
        
        visibleNodes[node].push_back(od);
	}
}

void
RestrictedVisionPerceptor::AddSense(Predicate& predicate,
                                    shared_ptr<BaseNode> node,
                                    TObjectList& objectList) const
{
    if (objectList.empty())
    {
        return;
    }

    shared_ptr<AgentAspect> agent_aspect =
        shared_dynamic_cast<AgentAspect>(node);
    if (agent_aspect != 0)
    {        
        shared_ptr<AgentAspect> aspect =
            agent_aspect->FindParentSupportingClass<AgentAspect>().lock();
        if (aspect != 0)
        {
            agent_aspect = aspect;
        }
        
        shared_ptr<AgentState> agent_state = shared_static_cast<AgentState>
            (agent_aspect->GetChildOfClass("AgentState",true));
        if (agent_state.get() == 0 ||
            (agent_state->GetPerceptName(ObjectState::PT_Player).empty())
           )
        {
            return;
        }
        
        ParameterList& element = predicate.parameter.AddList();
        element.AddValue(std::string("P"));

        ParameterList player;
        player.AddValue(std::string("team"));
        player.AddValue
            (std::string
                (agent_state->GetPerceptName(ObjectState::PT_Player)
                )
            );
        element.AddValue(player);

        if (! agent_state->GetID().empty())
        {
            ParameterList id;
            id.AddValue(std::string("id"));
            id.AddValue(agent_state->GetID());
            element.AddValue(id);
        }

        for (TObjectList::iterator j = objectList.begin();
            j != objectList.end(); ++j)
        {
            ObjectData& od = (*j);

            if (!od.mObj->GetID().empty())
            {
                ParameterList id;
                id.AddValue(od.mObj->GetID());

                ParameterList position;
                position.AddValue(std::string("pol"));
                position.AddValue(od.mDist);
                position.AddValue(od.mTheta);
                position.AddValue(od.mPhi);
                id.AddValue(position);

                element.AddValue(id);
            }
        }
    }
    else
    {
        for (TObjectList::iterator j = objectList.begin();
            j != objectList.end(); ++j)
        {
            ObjectData& od = (*j);
            ParameterList& element = predicate.parameter.AddList();
            element.AddValue(od.mObj->GetPerceptName());

            ParameterList& position = element.AddList();
            position.AddValue(std::string("pol"));
            position.AddValue(od.mDist);
            position.AddValue(od.mTheta);
            position.AddValue(od.mPhi);
        }
    }
}

void
RestrictedVisionPerceptor::ApplyNoise(ObjectData& od) const
{
    if (mAddNoise)
    {
        od.mDist  += (*(mDistRng.get()))() * od.mDist / 100.0;
        od.mTheta += (*(mThetaRng.get()))();
        od.mPhi   += (*(mPhiRng.get()))();
    }
}

bool
RestrictedVisionPerceptor::StaticAxisPercept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name       = mPredicateName;
    predicate.parameter.Clear();

    TTeamIndex  ti       = mAgentState->GetTeamIndex();
    salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();

    TNodeObjectsMap visibleNodes;
    SetupVisibleNodes(visibleNodes);

    for (TNodeObjectsMap::iterator i = visibleNodes.begin();
        i != visibleNodes.end(); ++i)
    {
        shared_ptr<BaseNode> node   = (*i).first;
        TObjectList& visibleObjects = (*i).second;

        for (TObjectList::iterator j = visibleObjects.begin();
            j != visibleObjects.end();)
        {
            ObjectData& od = (*j);

            if (mAddNoise)
            {
                od.mRelPos += mError;
            }

            if (od.mRelPos.Length() <= 0.1 ||
                (CheckOcclusion(myPos,od))
               )
            {
                // object is too close
                j = visibleObjects.erase(j);
                continue;
            }

            // theta is the angle in the X-Y (horizontal) plane
            assert(gAbs(GetPan()) <= 360);
            od.mTheta = salt::gRadToDeg(salt::gArcTan2(od.mRelPos[1], od.mRelPos[0])) - GetPan();
            od.mTheta = gNormalizeDeg(od.mTheta);
            // latitude
            assert(gAbs(GetTilt()) <= 360);
            od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[2]/od.mDist)) - GetTilt();
            od.mPhi = gNormalizeDeg(od.mPhi);

            if (gAbs(od.mTheta) > mHViewCone ||
                gAbs(od.mPhi) > mVViewCone
               )
            {
                j = visibleObjects.erase(j);
                continue;
            }

            // make some noise
            ApplyNoise(od);

            ++j;
        }

        // generate a sense entry
        AddSense(predicate, node, visibleObjects);
    }

    if (mSenseMyPos)
    {
        Vector3f sensedMyPos = SoccerBase::FlipView(myPos, ti);

        ParameterList& element = predicate.parameter.AddList();
        element.AddValue(std::string("mypos"));
        element.AddValue(sensedMyPos[0]);
        element.AddValue(sensedMyPos[1]);
        element.AddValue(sensedMyPos[2]);
    }

    return true;
}

bool
RestrictedVisionPerceptor::DynamicAxisPercept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name       = mPredicateName;
    predicate.parameter.Clear();

    const int hAngle_2 = mHViewCone >> 1;
    const int vAngle_2 = mVViewCone >> 1;

    // get the transformation matrix describing the current orientation
    const Matrix& mat = mTransformParent->GetWorldTransform();

    TNodeObjectsMap visibleNodes;
    SetupVisibleNodes(visibleNodes);

    for (TNodeObjectsMap::iterator i = visibleNodes.begin();
        i != visibleNodes.end(); ++i)
    {
        shared_ptr<BaseNode> node   = (*i).first;
        TObjectList& visibleObjects = (*i).second;

        for (TObjectList::iterator j = visibleObjects.begin();
            j != visibleObjects.end();)
        {
            ObjectData& od = (*j);

            if (mAddNoise)
            {
                od.mRelPos += mError;
            }

            if (od.mRelPos.Length() <= 0.1)
            {
                // object is too close
                j = visibleObjects.erase(j);
                continue;
            }

            // determine position relative to the local reference frame
            Vector3f localRelPos = mat.InverseRotate(od.mRelPos);

            // theta is the angle in horizontal plane, with fwAngle as 0 degree
            od.mTheta = gNormalizeDeg (gRadToDeg(gNormalizeRad(
                gArcTan2(localRelPos[1],localRelPos[0])
                )) -90 );

            if (gAbs(od.mTheta) > hAngle_2)
            {
                // object is out of view range
                // GetLog()->Debug() << "(RestrictedVisionPerceptor) Omitting "
                //     << od.mObj->GetPerceptName() << od.mObj->GetID()
                //     << ": h-angle = " << od.mTheta << ".\n" ;
                j = visibleObjects.erase(j);
                continue;
            }

            // latitude with fwPhi as 0 degreee
            od.mPhi = gRadToDeg(gNormalizeRad(
                            gArcTan2(localRelPos[2],
                                     Vector2f(localRelPos[0], localRelPos[1]).Length()
                                    )
                            )
                      );

            if (gAbs(od.mPhi) > vAngle_2)
            {
                j = visibleObjects.erase(j);
                continue;
            }

            // make some noise
            ApplyNoise(od);

            ++j;
        }

        // generate a sense entry
        AddSense(predicate, node, visibleObjects);
    }

    if (mSenseMyPos)
    {
        TTeamIndex  ti       = mAgentState->GetTeamIndex();
        salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();
        Vector3f sensedMyPos = SoccerBase::FlipView(myPos, ti);

        ParameterList& element = predicate.parameter.AddList();
        element.AddValue(std::string("mypos"));
        element.AddValue(sensedMyPos[0]);
        element.AddValue(sensedMyPos[1]);
        element.AddValue(sensedMyPos[2]);
    }

    return true;
}

bool
RestrictedVisionPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if (
        (mTransformParent.get() == 0) ||
        (mActiveScene.get() == 0) ||
        (mAgentAspect.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return false;
    }

    return mStaticSenseAxis ?
        StaticAxisPercept(predList) :
        DynamicAxisPercept(predList);
}

bool RestrictedVisionPerceptor::CheckOcclusion(const Vector3f& my_pos, const ObjectData& od) const
{
    // (occlusion test disabled for now, every object is visible)
    return false;

//     if (mRay.get() == 0) return;

//     // sort objects wrt their distance
//     visible_objects.sort();
//     // check visibility
//     std::list<ObjectData>::iterator start = visible_objects.begin();
//     ++start;

//     // this is going to be expensive now: to check occlusion of an object,
//     // we have to check all closer objects. For n objects, we have to
//     // check at most n*(n-1)/2 objects.
//     for (std::list<ObjectData>::iterator i = start;
//          i != visible_objects.end(); ++i)
//     {
//         for (std::list<ObjectData>::iterator j = visible_objects.begin();
//              j != i; ++j)
//         {
//             // cast ray from camera to object (j)
//             mRay->SetParams(j->mRelPos,my_pos,j->mDist);

//             dContactGeom contact;

//             shared_ptr<Collider> collider = shared_static_cast<Collider>
//                 (i->mObj->GetChildSupportingClass("Collider"));

//             if (mRay->Intersects(collider))
//                 {
//                     j->mVisible = false;
//                     j = i;
//                 }
//         }
//     }
}

void
RestrictedVisionPerceptor::SetSenseMyPos(bool sense)
{
    mSenseMyPos = sense;
}
