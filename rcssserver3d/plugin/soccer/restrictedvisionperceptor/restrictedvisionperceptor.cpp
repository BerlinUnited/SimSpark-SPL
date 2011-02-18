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
#include <soccerbase/soccerbase.h>
#include <salt/gmath.h>
#include <list>
#include <math.h>
#include <salt/linesegment2.h>
#include <limits>
#include <iostream>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;

RestrictedVisionPerceptor::RestrictedVisionPerceptor() : Perceptor(),
                                     mSenseMyPos(false),
                                     mAddNoise(true),
                                     mStaticSenseAxis(true),
                                     mSenseLine(false)
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
    
    boost::shared_ptr<AgentAspect> agent_aspect =
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

        boost::shared_ptr<BaseNode> node = shared_dynamic_cast<BaseNode>(mActiveScene);
        boost::shared_ptr<AgentAspect> agent_aspect =
                od.mObj->FindParentSupportingClass<AgentAspect>().lock();
        if (agent_aspect != 0)
        {
            boost::shared_ptr<AgentAspect> aspect =
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

        boost::shared_ptr<Transform> j = od.mObj->GetTransformParent();
        
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
                                    boost::shared_ptr<BaseNode> node,
                                    TObjectList& objectList) const
{
    if (objectList.empty())
    {
        return;
    }

    boost::shared_ptr<AgentAspect> agent_aspect =
        shared_dynamic_cast<AgentAspect>(node);
    if (agent_aspect != 0)
    {        
        boost::shared_ptr<AgentAspect> aspect =
            agent_aspect->FindParentSupportingClass<AgentAspect>().lock();
        if (aspect != 0)
        {
            agent_aspect = aspect;
        }
        
        boost::shared_ptr<AgentState> agent_state = shared_static_cast<AgentState>
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
        boost::shared_ptr<BaseNode> node   = (*i).first;
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

    if (mSenseLine)
    {
        SenseLine(predicate);
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
        boost::shared_ptr<BaseNode> node   = (*i).first;
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

    if (mSenseLine)
    {
      SenseLine(predicate);
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

//             boost::shared_ptr<Collider> collider = shared_static_cast<Collider>
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

bool RestrictedVisionPerceptor::CheckVisuable(RestrictedVisionPerceptor::ObjectData& od) const
{
  // theta is the angle in horizontal plane, with fwAngle as 0 degree
  od.mTheta = gNormalizeDeg(gRadToDeg(gNormalizeRad(
    gArcTan2(od.mRelPos[1], od.mRelPos[0])
    )) - 90);

  // latitude with fwPhi as 0 degreee
  od.mPhi = gRadToDeg(gNormalizeRad(
    gArcTan2(od.mRelPos[2],
    Vector2f(od.mRelPos[0], od.mRelPos[1]).Length()
    )
    )
    );

  od.mDist = od.mRelPos.Length();
  if (od.mDist <= 0.1)
  {
    // object is too close
    return false;
  }
  
  const int hAngle_2 = mHViewCone >> 1;
  if (gAbs(od.mTheta) > hAngle_2)
  {
    // object is out of view range
    return false;
  }

  const int vAngle_2 = mVViewCone >> 1;
  if (gAbs(od.mPhi) > vAngle_2)
  {
    return false;
  }

  return true;
}

void RestrictedVisionPerceptor::SenseLine(Predicate& predicate)
{
  const float focalLength = 0.1f;
  TLineList visibleLines;
  SetupLines(visibleLines);

  // TODO: precalculate it
  // visual range
  Vector2f ru(tan(gDegToRad<double>(mHViewCone*0.5))*focalLength, tan(gDegToRad<double>(mVViewCone*0.5))*focalLength);
  Vector2f rb(ru.x(), -ru.y());
  Vector2f lu(-ru.x(), ru.y());
  Vector2f lb(-ru.x(), -ru.y());
  LineSegment2f viewRangeLine[4];
  viewRangeLine[0] = LineSegment2f(lu, ru);
  viewRangeLine[1] = LineSegment2f(lb, rb);
  viewRangeLine[2] = LineSegment2f(lu, lb);
  viewRangeLine[3] = LineSegment2f(ru, rb);

  for (TLineList::iterator i = visibleLines.begin(); i != visibleLines.end(); )
  {
    LineData& ld = (*i);

    bool seeBeginPoint = CheckVisuable(ld.mBeginPoint);
    bool seeEndPoint = CheckVisuable(ld.mEndPoint);

    if (!(seeBeginPoint && seeEndPoint))
    {
      Vector3f bp3 = ld.mBeginPoint.mRelPos;
      Vector3f ep3 = ld.mEndPoint.mRelPos;
      
      if ( bp3.y() < focalLength && ep3.y() >= focalLength )
      {
        float t = ( focalLength - ep3.y() ) / ( bp3.y() - ep3.y() );
        bp3 = ep3 + ( bp3 - ep3 ) * t;
      }
      else if ( ep3.y() < focalLength && bp3.y() >= focalLength )
      {
        float t = ( focalLength - bp3.y() ) / ( ep3.y() - bp3.y() );
        ep3 = bp3 + ( ep3 - bp3 ) * t;
      }

      if ( bp3.y() > 0 && ep3.y() > 0 )
      {
        Vector2f bp = Vector2f(bp3.x(), bp3.z()) * focalLength / bp3.y();
        Vector2f ep = Vector2f(ep3.x(), ep3.z()) * focalLength / ep3.y();
        LineSegment2f l(bp, ep);

        float CB = bp3.Length();
        float CE = ep3.Length();
        float BE = (bp3 - ep3).Length();
        float CEB = acos((BE * BE + CE * CE - CB * CB) / (2 * BE * CE));
        Vector3f Ef(ep.x(), focalLength, ep.y());
        float CEf = Ef.Length();

        Vector3f X[2];
        int inum = 0;
        for ( int i=0; i<4 && inum < 2; i++)
        {
          Vector2f ip;
          if ( l.Intersection(viewRangeLine[i], ip) )
          {
            Vector3f Xf(ip.x(), focalLength, ip.y());
            float CXf = Xf.Length();
            float XfEf = (Xf - Ef).Length();
            float XfCEf = acos((CXf * CXf + CEf * CEf - XfEf * XfEf) / (2 * CXf * CEf));
            float CXE = gPI - CEB - XfCEf;
            float EX = CE / sin(CXE) * sin(XfCEf);
            X[inum] = ep3 + (bp3 - ep3) * (EX / BE);
            inum++;
          }
        }

        if ( inum == 2 )
        {
          ld.mBeginPoint.mRelPos = X[0];
          CheckVisuable(ld.mBeginPoint);
          seeBeginPoint = true;

          ld.mEndPoint.mRelPos = X[1];
          CheckVisuable(ld.mEndPoint);
          seeEndPoint = true;
        }
        else if ( inum == 1 )
        {
          if ( !seeBeginPoint )
          {
            ld.mBeginPoint.mRelPos = X[0];
            CheckVisuable(ld.mBeginPoint);
            seeBeginPoint = true;
          }
          else
          {
            ld.mEndPoint.mRelPos = X[0];
            CheckVisuable(ld.mEndPoint);
            seeEndPoint = true;
          }
        }
      }
    }
    
    if (seeBeginPoint && seeEndPoint)
    {
      // make some noise
      ApplyNoise(ld.mBeginPoint);
      ApplyNoise(ld.mEndPoint);

      ++i;
    }
    else
    {
      i = visibleLines.erase(i);
    }
  }

  // generate a sense entry
  AddSense(predicate, visibleLines);
}

void
RestrictedVisionPerceptor::SetupLines(TLineList& visibleLines)
{
  TLeafList lineList;
  mActiveScene->GetChildrenOfClass("Line", lineList, true);

  // determine position relative to the local reference frame
  const Matrix& mat = mTransformParent->GetWorldTransform();

  // get the transformation matrix describing the current orientation
  Vector3f myPos = mat.Pos();
  if (mAddNoise)
  {
    myPos -= mError;
  }


  for (TLeafList::iterator i = lineList.begin(); i != lineList.end(); ++i)
  {
    LineData ld;

    ld.mLine = shared_static_cast<Line > (*i);

    if (ld.mLine.get() == 0)
    {
      GetLog()->Error() << "Error: (RestrictedVisionPerceptor) skipped line: "
        << (*i)->GetName() << "\n";
      continue; // this should never happen
    }

    boost::shared_ptr<Transform> j = ld.mLine->GetTransformParent();

    if (j.get() == 0)
    {
      GetLog()->Error() << "Error: (RestrictedVisionPerceptor) skipped line (2): "
        << (*i)->GetName() << "\n";
      continue; // this should never happen
    }

    const salt::Matrix& t = j->GetWorldTransform();
    ld.mBeginPoint.mRelPos =   mat.InverseRotate( t * ld.mLine->BeginPoint() - myPos );
    ld.mEndPoint.mRelPos = mat.InverseRotate( t * ld.mLine->EndPoint() - myPos );

    visibleLines.push_back(ld);
  }
}

void RestrictedVisionPerceptor::AddSense(Predicate& predicate, const TLineList& lineList) const
{
  for (TLineList::const_iterator i = lineList.begin(); i != lineList.end(); ++i)
  {
    const LineData& ld = (*i);
    ParameterList& element = predicate.parameter.AddList();
    element.AddValue(std::string("L"));

    ParameterList& begPos = element.AddList();
    begPos.AddValue(std::string("pol"));
    begPos.AddValue(ld.mBeginPoint.mDist);
    begPos.AddValue(ld.mBeginPoint.mTheta);
    begPos.AddValue(ld.mBeginPoint.mPhi);

    ParameterList& endPos = element.AddList();
    endPos.AddValue(std::string("pol"));
    endPos.AddValue(ld.mEndPoint.mDist);
    endPos.AddValue(ld.mEndPoint.mTheta);
    endPos.AddValue(ld.mEndPoint.mPhi);
  }
}

void RestrictedVisionPerceptor::SetSenseLine(bool sense)
{
  mSenseLine = sense;
}
