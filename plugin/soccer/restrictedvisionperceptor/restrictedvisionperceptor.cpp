/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: restrictedvisionperceptor.cpp,v 1.2 2006/03/01 18:32:23 fruit Exp $

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
#include <iostream>
using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

RestrictedVisionPerceptor::RestrictedVisionPerceptor() : Perceptor(),
                                     mSenseMyPos(false),
                                     mAddNoise(true),
                                     mUseRandomNoise(true),
                                     mStaticSenseAxis(true)
{
    // set predicate name
    SetPredicateName("Vision");
    // set some default noise values
    SetNoiseParams(0.0965, 0.1225, 0.1480, 0.005);
    SetVisionAngles(90,90);
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
RestrictedVisionPerceptor::SetVisionAngles(int hAngle, int vAngle)
{
    mHAngle = hAngle;
    mVAngle = vAngle;

//     std::cerr << "setting vision angles to (" << mHAngle
//               << ", " << mVAngle << ").\n";
    
}


void
RestrictedVisionPerceptor::OnLink()
{
    SoccerBase::GetTransformParent(*this,mTransformParent);
    SoccerBase::GetAgentState(*this, mAgentState);
    SoccerBase::GetActiveScene(*this,mActiveScene);
}

void
RestrictedVisionPerceptor::OnUnlink()
{
    mDistRng.reset();
    mPhiRng.reset();
    mThetaRng.reset();
    mTransformParent.reset();
    mAgentState.reset();
    mActiveScene.reset();
}

void
RestrictedVisionPerceptor::AddNoise(bool add_noise)
{
    mAddNoise = add_noise;
}

void
RestrictedVisionPerceptor::UseRandomNoise(bool random_noise)
{
    mUseRandomNoise = random_noise;
}

void
RestrictedVisionPerceptor::SetStaticSenseAxis(bool static_axis)
{
    mStaticSenseAxis = static_axis;
}

bool
RestrictedVisionPerceptor::ConstructInternal()
{
    mRay = shared_static_cast<oxygen::RayCollider>
        (GetCore()->New("oxygen/RayCollider"));

    if (mRay.get() == 0)
    {
        GetLog()->Error() << "Error: (RestrictedVisionPerceptor) cannot create Raycollider. "
                          << "occlusion check disabled\n";
    }

    return true;
}

void
RestrictedVisionPerceptor::SetupVisibleObjects(TObjectList& visibleObjects)
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

            visibleObjects.push_back(od);
        }
}

void RestrictedVisionPerceptor::AddSense(oxygen::Predicate& predicate, ObjectData& od) const
{
    ParameterList& element = predicate.parameter.AddList();
    element.AddValue(od.mObj->GetPerceptName());

    if(od.mObj->GetPerceptName() == "Player")
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

    const int hAngle_2 = mHAngle>>1;
    const int vAngle_2 = mVAngle>>1;
    
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
        {
            ParameterList& element = predicate.parameter.AddList();
            element.AddValue(std::string("debug"));
            element.AddValue(od.mTheta);
            element.AddValue(mHAngle/2);
        }

        if ((gAbs(od.mTheta) > mHAngle/2) && (od.mObj->GetPerceptName() != "Flag"))
        {
            // object is out of view range
            GetLog()->Debug() << "(RestrictedVisionPerceptor) Omitting "
                              << od.mObj->GetPerceptName() << od.mObj->GetID()
                              << ": h-angle = " << od.mTheta << ".\n" ;
                continue;       
            }   

        // latitude
        od.mPhi = 90.0 - salt::gRadToDeg(salt::gArcCos(od.mRelPos[2]/od.mDist));

        // make some noise
        ApplyNoise(od);

        // generate a sense entry
        AddSense(predicate,od);
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

bool
RestrictedVisionPerceptor::DynamicAxisPercept(boost::shared_ptr<PredicateList> predList)
{
    Predicate& predicate = predList->AddPredicate();
    predicate.name       = mPredicateName;
    predicate.parameter.Clear();
    
    const int hAngle_2 = mHAngle>>1;
    const int vAngle_2 = mVAngle>>1;
    
    //std::cout << "mHAngle = " << mHAngle << " / hAngle_2 = " << hAngle_2 << std::endl;
    
    TTeamIndex  ti = mAgentState->GetTeamIndex();
    
    const Vector3f& up = mTransformParent->GetWorldTransform().Up();
    
    // calc the percptors angle in the horizontal plane
    double fwTheta = gNormalizeRad(Vector2f(up[0],up[1]).GetAngleRad());
    
    // calc the perceptors angle in the vertical plane
    // for this the vector has to rotated, i.e. you cannot just use x and z component
    //double fwPhi = gNormalizeRad(Vector2f(Vector2f(up[0],up[1]).Length(),up[2]).GetAngleRad());
    
    // assume that perceptor is always horizontal.
    // FIXME: this is magic
    double fwPhi = 0.0;
    
    
    TObjectList visibleObjects;
    SetupVisibleObjects(visibleObjects);
    
    // log for 7th agent of the first team
    if ((mAgentState->GetTeamIndex() == 1) && (mAgentState->GetUniformNumber() ==7))
        GetLog()->Debug() << "percept: " << visibleObjects.size() << " objects. :::"
                          << up << " theta " << gRadToDeg(fwTheta)
                          << " phi " << gRadToDeg(fwPhi) << "\n";      

    for (std::list<ObjectData>::iterator i = visibleObjects.begin();
         i != visibleObjects.end(); ++i)
    {   
        ObjectData& od = (*i);


        od.mRelPos = SoccerBase::FlipView(od.mRelPos, ti);
        if ((mAgentState->GetTeamIndex() == 1) && (mAgentState->GetUniformNumber() ==7))
            GetLog()->Debug() << "object " << od.mObj->GetPerceptName()
                              << " at : " << od.mRelPos << "\n";

        if (mAddNoise)
        {
            od.mRelPos += mError;
        }
        
        if (od.mRelPos.Length() <= 0.1)
        {
            // object is too close
            continue;
        }       

        // theta is the angle in horizontal plane, with fwAngle as 0 degree
        od.mTheta = gRadToDeg(gNormalizeRad(
                                  Vector2f(od.mRelPos[0],od.mRelPos[1]).GetAngleRad() -
                                  fwTheta
                                  ));
        // flags are always visible
        if ((gAbs(od.mTheta) > hAngle_2) && (od.mObj->GetPerceptName() != "Flag"))
        {
            // object is out of view range
//                                 GetLog()->Debug() << "(RestrictedVisionPerceptor) Omitting "
//                               << od.mObj->GetPerceptName() << od.mObj->GetID()
//                               << ": h-angle = " << od.mTheta << ".\n" ;
            continue;
        }
        
        
        // latitude with fwPhi as 0 degreee
        od.mPhi = gRadToDeg(gNormalizeRad(
                                Vector2f(
                                    Vector2f(od.mRelPos[0],od.mRelPos[1]).Length(),
                                    od.mRelPos[2]).GetAngleRad() - fwPhi
                                ));

        
        if ((gAbs(od.mPhi) > vAngle_2) && (od.mObj->GetPerceptName() != "Flag"))
            continue;
            
        // log for 7th agent of the first team
        if ((mAgentState->GetTeamIndex() == 1) && (mAgentState->GetUniformNumber() ==7))
            GetLog()->Debug() << "percept: " << "adding object: "
                              << gAbs(od.mPhi) << ":" << vAngle_2 << "\n";

        // make some noise
        ApplyNoise(od);

        // generate a sense entry
        AddSense(predicate,od);

    }
 
    if (mSenseMyPos)
    {
        salt::Vector3f myPos = mTransformParent->GetWorldTransform().Pos();
            
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


bool
RestrictedVisionPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
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
