/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.cpp,v 1.26 2008/06/06 07:56:16 sgvandijk Exp $

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
#include "visionperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <soccer/soccerbase/soccerbase.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

VisionPerceptor::VisionPerceptor() : Perceptor(),
                                     mSenseMyPos(false),
                                     mAddNoise(true),
                                     mUseRandomNoise(true),
                                     mStaticSenseAxis(true)
{
    // set predicate name
    SetPredicateName("See");
    // set some default noise values
    SetNoiseParams(0.0965f, 0.1225f, 0.1480f, 0.005f);
}

VisionPerceptor::~VisionPerceptor()
{
    mDistRng.reset();
    mPhiRng.reset();
    mThetaRng.reset();
}

void
VisionPerceptor::SetNoiseParams(float sigma_dist, float sigma_phi,
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
VisionPerceptor::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
//     SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetActiveScene(*this,mActiveScene);
    
    shared_ptr<AgentAspect> agent_aspect =
        make_shared(FindParentSupportingClass<AgentAspect>());
    if (agent_aspect == 0)
    {
        GetLog()->Error()
            << "Error: (RestrictedVisionPerceptor) cannot find AgentAspect.\n";
    }
    else
    {
        mAgentAspect = agent_aspect;
        agent_aspect = make_shared(agent_aspect->FindParentSupportingClass<AgentAspect>());
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
VisionPerceptor::OnUnlink()
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
VisionPerceptor::AddNoise(bool add_noise)
{
    mAddNoise = add_noise;
}

void
VisionPerceptor::UseRandomNoise(bool random_noise)
{
    mUseRandomNoise = random_noise;
}

void
VisionPerceptor::SetStaticSenseAxis(bool static_axis)
{
    mStaticSenseAxis = static_axis;
}

bool
VisionPerceptor::ConstructInternal()
{
    mRay = shared_static_cast<oxygen::RayCollider>
        (GetCore()->New("oxygen/RayCollider"));

    if (mRay.get() == 0)
    {
        GetLog()->Error() << "Error: (VisionPerceptor) cannot create Raycollider. "
                          << "occlusion check disabled\n";
    }

    return true;
}

void
VisionPerceptor::SetupVisibleObjects(TObjectList& visibleObjects)
{
    TLeafList objectList;
    mActiveScene->ListChildrenSupportingClass<ObjectState>(objectList, true);

    salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();

    for (TLeafList::iterator i = objectList.begin();
         i != objectList.end(); ++i)
        {
            ObjectData od;
            od.mObj = shared_static_cast<ObjectState>(*i);

            if (od.mObj.get() == 0)
                {
                    GetLog()->Error() << "Error: (VisionPerceptor) skipped: "
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

            visibleObjects.push_back(od);
        }
}

void VisionPerceptor::AddSense(oxygen::Predicate& predicate, ObjectData& od) const
{
    ParameterList& element = predicate.parameter.AddList();
    element.AddValue(od.mObj->GetPerceptName());

//    if(od.mObj->GetPerceptName() == "Player")
    if(od.mObj->GetPerceptName() == "P")
        {
            ParameterList player;
            player.AddValue(std::string("team"));
            player.AddValue
                (std::string
                 (od.mObj->GetPerceptName(ObjectState::PT_Player)
                  )
                 );
            element.AddValue(player);
        }

    if (!od.mObj->GetID().empty())
        {
            ParameterList id;
            id.AddValue(std::string("id"));
            id.AddValue(od.mObj->GetID());
            element.AddValue(id);
        }

    ParameterList& position = element.AddList();
    position.AddValue(std::string("pol"));
    position.AddValue(od.mDist);
    position.AddValue(od.mTheta);
    position.AddValue(od.mPhi);
}

void
VisionPerceptor::ApplyNoise(ObjectData& od) const
{
    if (mAddNoise)
        {
            if (mUseRandomNoise)
                {
                    od.mDist  += (*(mDistRng.get()))() * od.mDist / 100.0;
                    od.mTheta += (*(mThetaRng.get()))();
                    od.mPhi   += (*(mPhiRng.get()))();
                } else
                {
                    /* This gives a constant random error throughout the whole
                     * match. This behavior was not intended and is a bug and
                     * not an intended feature.
                     * It was kept in the simulator because I discovered this
                     * bug only shortly before the competition. *sigh* oliver
                     */
                    od.mDist  += salt::NormalRNG<>(0.0,mSigmaDist)();
                    od.mTheta += salt::NormalRNG<>(0.0,mSigmaTheta)();
                    od.mPhi  += salt::NormalRNG<>(0.0,mSigmaPhi)();
                }
        }
}


bool
VisionPerceptor::StaticAxisPercept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name       = mPredicateName;
    predicate.parameter.Clear();

    TTeamIndex  ti       = mAgentState->GetTeamIndex();
    salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();

    TObjectList visibleObjects;
    SetupVisibleObjects(visibleObjects);

    for (std::list<ObjectData>::iterator i = visibleObjects.begin();
         i != visibleObjects.end(); ++i)
    {
        ObjectData& od = (*i);

        od.mRelPos = SoccerBase::FlipView(od.mRelPos, ti);
        if (mAddNoise)
            {
                od.mRelPos += mError;
            }

        if (
            (od.mRelPos.Length() <= 0.1) ||
            (CheckOcclusion(myPos,od))
            )
        {
            // object is occluded or too close
            continue;
        }

        // theta is the angle in the X-Y (horizontal) plane
        od.mTheta = salt::gRadToDeg(salt::gArcTan2(od.mRelPos[1], od.mRelPos[0]));

        // latitude
        od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[2]/od.mDist));

        // make some noise
        ApplyNoise(od);

        // generate a sense entry
        AddSense(predicate,od);
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
VisionPerceptor::DynamicAxisPercept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name       = mPredicateName;
    predicate.parameter.Clear();

	// get the transformation matrix describing the current orientation
    const Matrix& mat = mTransformParent->GetWorldTransform();

    TObjectList visibleObjects;
    SetupVisibleObjects(visibleObjects);

    for (std::list<ObjectData>::iterator i = visibleObjects.begin();
         i != visibleObjects.end(); ++i)
    {
        ObjectData& od = (*i);

        if (mAddNoise)
            {
                od.mRelPos += mError;
            }

        if (od.mRelPos.Length() <= 0.1)
        {
            // object is too close
            continue;
        }

		// determine position relative to the local reference frame
        Vector3f localRelPos = mat.InverseRotate(od.mRelPos);
        
        
        // theta is the angle in horizontal plane, with fwAngle as 0 degree
        od.mTheta = gNormalizeDeg (gRadToDeg(gNormalizeRad(
                              gArcTan2(localRelPos[1],localRelPos[0])
                              )) -90 );

        // latitude with fwPhi as 0 degreee
        od.mPhi = gRadToDeg(gNormalizeRad(
							gArcTan2( localRelPos[2], Vector2f(localRelPos[0], localRelPos[1]).Length())
                            ));

        // make some noise
        ApplyNoise(od);

        // generate a sense entry
        AddSense(predicate,od);
    }

    if (mSenseMyPos)
    {
        TTeamIndex  ti          = mAgentState->GetTeamIndex();
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
VisionPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
    if (
        (mTransformParent.get() == 0) ||
        (mActiveScene.get() == 0) ||
        (mAgentState.get() == 0)
        )
    {
        return false;
    }

    return mStaticSenseAxis ?
        StaticAxisPercept(predList) :
        DynamicAxisPercept(predList);
}

bool VisionPerceptor::CheckOcclusion(const Vector3f& my_pos, const ObjectData& od) const
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
VisionPerceptor::SetSenseMyPos(bool sense)
{
    mSenseMyPos = sense;
}
