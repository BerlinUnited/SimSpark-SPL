/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.cpp,v 1.1.2.5 2004/02/02 18:29:42 fruit Exp $

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

using namespace oxygen;
using namespace boost;

VisionPerceptor::VisionPerceptor() : oxygen::Perceptor(),
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

bool VisionPerceptor::ConstructInternal()
{
    mRay = shared_static_cast<oxygen::RayCollider>
        (GetCore()->New("kerosin/RayCollider"));

    if (mRay.get() == 0)
    {
        GetLog()->Error() << "Error: (VisionPerceptor) cannot create Raycollider. "
                          << "occlusion check disabled\n";
    }

    return true;
}

bool
VisionPerceptor::Percept(Predicate& predicate)
{
    predicate.name = mPredicateName;
    predicate.parameter.clear();

    shared_ptr<Scene> activeScene;
    if (!GetActiveScene(activeScene))
    {
        GetLog()->Error() << "ERROR: (VisionPerceptor) no active scene\n";
        return false;
    }

    // we want positions relative to the closest parent transform node
    shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
        (make_shared(GetParentSupportingClass("Transform")));

    if (parent.get() == 0)
    {
        GetLog()->Error()
            << "Error: (VisionPerceptor) "
            << "parent node is not derived from TransformNode\n";
        return false;
    }

    if (mAgentState.get() == 0)
    {
        mAgentState =
            shared_static_cast<AgentState>(parent->GetChild("AgentState"));
    }
    TTeamIndex ti = TI_NONE;
    if (mAgentState.get() != 0)
    {
        ti = mAgentState->GetTeamIndex();
    }
#define DEBUG_SIDE
#ifdef DEBUG_SIDE
    if (ti == TI_LEFT) predicate.parameter.push_back(std::string("(debug_message left)"));
    else if (ti == TI_RIGHT) predicate.parameter.push_back(std::string("(debug_message right)"));
#endif
    salt::Vector3f myPos = parent->GetWorldTransform().Pos();

    TLeafList transformList;
    activeScene->GetChildrenSupportingClass("Transform", transformList, true);

    ObjectData od;
    std::list<ObjectData> visibleObjects;

    for (TLeafList::iterator i = transformList.begin();
         i != transformList.end(); ++i)
    {
        od.mVisible = true;
        shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
        od.mRelPos = FlipView(j->GetWorldTransform().Pos() - myPos, ti);
        if (mAddNoise) od.mRelPos += mError;

        od.mDist = od.mRelPos.Length();

        if (od.mDist > 0.1)
        {
            od.mObj = (*i);
            // theta is the angle in the X-Y (horizontal) plane
            od.mTheta = salt::gRadToDeg(salt::gArcTan2(od.mRelPos[2], od.mRelPos[0]));
            // latitude
            od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[1]/od.mDist));
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
        Predicate::TParameterList position;
#define DEBUG_VISIBILITY_CHECK
#ifdef DEBUG_VISIBILITY_CHECK
        if (i->mVisible)
            position.push_back(std::string("pol"));
        else
            position.push_back(std::string("invisible"));
#else
        if (! i->mVisible) continue;
        position.push_back(std::string("pol"));
#endif

        position.push_back(i->mDist);
        position.push_back(i->mTheta);
        position.push_back(i->mPhi);

        Predicate::TParameterList element;

        element.push_back(i->mObj->GetName());
        element.push_back(position);
        predicate.parameter.push_back(element);
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

bool
VisionPerceptor::GetActiveScene(boost::shared_ptr<oxygen::Scene>& active_scene)
{
    if (mSceneServer.get() == 0)
    {
        mSceneServer = shared_static_cast<SceneServer>
            (GetCore()->Get("/sys/server/scene"));
    }

    if (mSceneServer.get() == 0)
    {
        return false;
    }

    active_scene = mSceneServer->GetActiveScene();
    if (active_scene.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (VisionPerceptor) SceneServer reports no active scene\n";
        return false;
    }
    return true;
}

salt::Vector3f
VisionPerceptor::FlipView(const salt::Vector3f& pos, TTeamIndex ti)
{
    salt::Vector3f newPos;
    switch (ti) {
    case TI_NONE:
        newPos[0] = 0.0;
        newPos[1] = 0.0;
        newPos[2] = 0.0;
        break;
    case TI_LEFT:
        newPos[0] = -pos[0];
        newPos[1] = pos[1];
        newPos[2] = -pos[2];
        break;
    case TI_RIGHT:
        newPos = pos;
        break;
    }
    return newPos;
}
