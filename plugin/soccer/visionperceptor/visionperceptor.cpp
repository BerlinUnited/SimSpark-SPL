/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.cpp,v 1.5.2.2 2004/03/28 15:41:00 rollmark Exp $

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
#include <salt/random.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <soccer/soccerbase/soccerbase.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

VisionPerceptor::VisionPerceptor() : Perceptor(), mSenseMyPos(false),
                                     mAddNoise(true), mPredicateName("Vision")
{
    // set some default noise values
    SetNoiseParams(0.0965, 0.1225, 0.1480, 0.005);
}

VisionPerceptor::~VisionPerceptor()
{
}

void
VisionPerceptor::SetNoiseParams(float sigma_dist, float sigma_phi,
                                float sigma_theta, float cal_error_abs)
{
    mSigmaDist = sigma_dist;
    mSigmaPhi = sigma_phi;
    mSigmaTheta = sigma_theta;
    mCalErrorAbs = cal_error_abs;

    salt::UniformRNG<float> rng(-mCalErrorAbs,mCalErrorAbs);
    mError = salt::Vector3f(rng(),rng(),rng());
}

void
VisionPerceptor::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
    SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetActiveScene(*this,mActiveScene);
}

void VisionPerceptor::OnUnlink()
{
    mTransformParent.reset();
    mAgentState.reset();
    mActiveScene.reset();
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

    Predicate& predicate = predList->AddPredicate();
    predicate.name = mPredicateName;
    predicate.parameter.Clear();

    TTeamIndex  ti = mAgentState->GetTeamIndex();

    salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();

    TLeafList objectList;
    mActiveScene->ListChildrenSupportingClass<ObjectState>(objectList, true);

    ObjectData od;
    std::list<ObjectData> visibleObjects;

    for (TLeafList::iterator i = objectList.begin();
         i != objectList.end(); ++i)
    {
        od.mVisible = true;
        od.mObj = shared_static_cast<ObjectState>(*i);
        if (od.mObj.get() == 0)
        {
            GetLog()->Error() << "Error: (VisionPerceptor) skipped: "
                              << (*i)->GetName() << "\n";
            continue; // this should never happen
        }
        shared_ptr<Transform> j = od.mObj->GetTransformParent();
        if (j.get() == 0) continue; // this should never happen

        od.mRelPos = SoccerBase::FlipView(j->GetWorldTransform().Pos() - myPos, ti);
        if (mAddNoise) od.mRelPos += mError;

        od.mDist = od.mRelPos.Length();

        if (od.mDist > 0.1)
        {
            // theta is the angle in the X-Y (horizontal) plane
            od.mTheta = salt::gRadToDeg(salt::gArcTan2(od.mRelPos[1], od.mRelPos[0]));
            // latitude
            od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[2]/od.mDist));
            // make some noise
            if (mAddNoise)
            {
                od.mDist += salt::NormalRNG<>(0.0,mSigmaDist)();
                od.mTheta += salt::NormalRNG<>(0.0,mSigmaTheta)();
                od.mPhi += salt::NormalRNG<>(0.0,mSigmaPhi)();
            }
            visibleObjects.push_back(od);
        }
    }
    if (visibleObjects.empty()) return true;

    CheckOcclusion(myPos,visibleObjects);

    for (std::list<ObjectData>::iterator i = visibleObjects.begin();
         i != visibleObjects.end(); ++i)
    {
        ParameterList& element = predicate.parameter.AddList();
        element.AddValue(i->mObj->GetPerceptName());

        if (!i->mObj->GetID().empty())
        {
            ParameterList id;
            id.AddValue(std::string("id"));
            id.AddValue(i->mObj->GetID());
            element.AddValue(id);
        }

        ParameterList& position = element.AddList();
#define DEBUG_VISIBILITY_CHECK
#ifdef DEBUG_VISIBILITY_CHECK
        if (i->mVisible)
            position.AddValue(std::string("pol"));
        else
            position.AddValue(std::string("invisible"));
#else
        if (! i->mVisible) continue;
        position.AddValue(std::string("pol"));
#endif
        position.AddValue(i->mDist);
        position.AddValue(i->mTheta);
        position.AddValue(i->mPhi);
    }

    if (mSenseMyPos)
        {
            Vector3f sensedMyPos = myPos;
            SoccerBase::FlipView(sensedMyPos, ti);

            ParameterList& element = predicate.parameter.AddList();
            element.AddValue(std::string("mypos"));
            element.AddValue(sensedMyPos[0]);
            element.AddValue(sensedMyPos[1]);
            element.AddValue(sensedMyPos[2]);
        }

    return true;
}

void
VisionPerceptor::CheckOcclusion(const salt::Vector3f& my_pos,
                                std::list<ObjectData>& visible_objects) const
{
    return;

    if (mRay.get() == 0) return;

    // sort objects wrt their distance
    visible_objects.sort();
    // check visibility
    std::list<ObjectData>::iterator start = visible_objects.begin();
    ++start;

    // this is going to be expensive now: to check occlusion of an object,
    // we have to check all closer objects. For n objects, we have to
    // check at most n*(n-1)/2 objects.
    for (std::list<ObjectData>::iterator i = start;
         i != visible_objects.end(); ++i)
    {
        for (std::list<ObjectData>::iterator j = visible_objects.begin();
             j != i; ++j)
        {
            // cast ray from camera to object (j)
            mRay->SetParams(j->mRelPos,my_pos,j->mDist);

            dContactGeom contact;

            shared_ptr<Collider> collider = shared_static_cast<Collider>
                (i->mObj->GetChildSupportingClass("Collider"));

            if (mRay->Intersects(collider))
                {
                    j->mVisible = false;
                    j = i;
                }
        }
    }
}

void VisionPerceptor::SetSenseMyPos(bool sense)
{
    mSenseMyPos = sense;
}


