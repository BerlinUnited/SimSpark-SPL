/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor.cpp,v 1.1.2.2 2004/01/26 15:19:29 fruit Exp $

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
#include <oxygen/physicsserver/raycollider.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>

using namespace oxygen;
using namespace boost;

VisionPerceptor::VisionPerceptor() : oxygen::Perceptor(), mAddNoise(true)
{
    salt::UniformRNG<> rng(-0.005,0.005);
    mError = salt::Vector3f(rng(),rng(),rng());

    // we use small sigmas for generating small measurement errors
    // normally distributed around 0.0.
    // the sigmas should be set by script vars

    // dist error values approx. between -0.5 and 0.5
    mDistErrorRNG =
        std::auto_ptr<salt::NormalRNG<> >(new salt::NormalRNG<>(0.0, 0.0965));
    // angle error in X-Y plane, approx. between -0.65 and 0.64
    mThetaErrorRNG =
        std::auto_ptr<salt::NormalRNG<> >(new salt::NormalRNG<>(0.0, 0.1225));
    // latitudal angle error, approx. between -0.8 and 0.8
    mPhiErrorRNG =
        std::auto_ptr<salt::NormalRNG<> >(new salt::NormalRNG<>(0.0, 0.1480));
}

VisionPerceptor::~VisionPerceptor()
{
}

bool
VisionPerceptor::Percept(Predicate& predicate)
{
    predicate.name = "Vision";
    predicate.parameter.clear();

    if (mSceneServer.get() == 0)
    {
        mSceneServer = shared_static_cast<SceneServer>
            (GetCore()->Get("/sys/server/scene"));
    }

    if (mSceneServer.get() == 0)
    {
        return false;
    }

    shared_ptr<Scene> activeScene = mSceneServer->GetActiveScene();
    if (activeScene.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (VisionPerceptor) SceneServer reports no active scene\n";
        return false;
    }

    // we want positions relative to the closest parent transform node
    shared_ptr<Transform> parent = shared_dynamic_cast<Transform>
        (make_shared(GetParentSupportingClass("Transform")));

    salt::Vector3f myPos(0,0,0);
    if (parent.get() == 0)
    {
        GetLog()->Warning()
            << "WARNING: (VisionPerceptor) parent node is not derived from TransformNode\n";
    } else {
        myPos = parent->GetWorldTransform().Pos();
    }

    TLeafList transformList;
    activeScene->GetChildrenSupportingClass("Transform", transformList, true);

    ObjectData od;
    std::list<ObjectData> visibleObjects;

    for (TLeafList::iterator i = transformList.begin();
         i != transformList.end(); ++i)
    {
        od.mVisible = true;
        shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
        od.mRelPos = j->GetWorldTransform().Pos() - myPos;
        if (mAddNoise) od.mRelPos += mError;

        od.mDist = od.mRelPos.Length();

        if (od.mDist > 0.1)
        {
            od.mObj = i;
            // theta is the angle in the X-Y (horizontal) plane
            od.mTheta = salt::gRadToDeg(salt::gArcTan2(od.mRelPos[2], od.mRelPos[0]));
            // latitude
            od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[1]/od.mDist));
            // make some noise
            if (mAddNoise)
            {
                od.mDist += (*mDistErrorRNG)();
                od.mTheta += (*mThetaErrorRNG)();
                od.mPhi += (*mPhiErrorRNG)();
            }
            visibleObjects.push_back(od);
        }
    }
    if (visibleObjects.empty()) return true;

#if 0
    // sort objects wrt their distance
    visibleObjects.sort();
    // check visibility
    std::list<ObjectData>::iterator start = visibleObjects.begin();
    ++start;

    // this is going to be expensive now: to check occlusion of an object,
    // we have to check all closer objects. For n objects, we have to
    // check at most n*(n-1)/2 objects.
    shared_ptr<oxygen::RayCollider> ray =
        shared_static_cast<oxygen::RayCollider>(GetCore()->New("kerosin/RayCollider"));


    for (std::list<ObjectData>::iterator i = start;
         i != visibleObjects.end(); ++i)
    {
        for (std::list<ObjectData>::iterator j = visibleObjects.begin();
             j != i; ++j)
        {
            // cast ray from camera to object (j)
            ray->SetParams(myPos,j->mRelPos,j->mDist);

            dContactGeom contact;

            shared_ptr<Collider> collider =
                shared_static_cast<Collider>((*(i->mObj))->GetChild("geometry"));

            if (collider.get() != 0)
            {
                if (0 < dCollide(ray->GetODEGeom(),
                                 collider->GetODEGeom(),
                                 1, /* ask for at most one collision point */
                                 &contact, sizeof(contact)))
                {
                    j->mVisible = false;
                    j = i;
                }
            }
        }
    }
#endif

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

        element.push_back((*(i->mObj))->GetName());
        element.push_back(position);
        predicate.parameter.push_back(element);
    }

    return true;
}
