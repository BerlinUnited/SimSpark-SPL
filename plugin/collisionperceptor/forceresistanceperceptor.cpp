/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

 this file is part of rcssserver3D
 Fri May 9 2003
 Copyright (C) 2002,2003 Koblenz University
 Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
 $Id: forceresistanceperceptor.cpp,v 1.2 2007/06/03 22:12:58 hedayat Exp $

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

#include "forceresistanceperceptor.h"
#include <oxygen/physicsserver/odewrapper.h>
#include <oxygen/sceneserver/transform.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace oxygen;
using namespace zeitgeist;

void ForceResistancePerceptor::OnLink()
{
    Perceptor::OnLink();

    mBody = shared_static_cast<Transform>(FindParentSupportingClass<Transform>().lock());

    if (mBody.get() == 0)
        GetLog()->Error()
                << "ForceResistancePerceptor: no suitable parent node found!\n";
}

void ForceResistancePerceptor::OnUnlink()
{
    Perceptor::OnUnlink();
    mBody.reset();
}

void ForceResistancePerceptor::AddTouchInfo(dContact &contact,
        dJointID contactJointID)
{
    mContactList.push_front(make_pair(contact.geom, contactJointID));
}

bool ForceResistancePerceptor::Percept(
        boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mContactList.empty())
    {
        return false;
    }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "FRP";
    predicate.parameter.Clear();

    ParameterList& nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("n"));
    nameElement.AddValue(GetName());

    Vector3f force(0,0,0);
    Vector3f pos(0,0,0);
    float sumLength = 0;
    for (ContactListType::const_iterator i = mContactList.begin();
         i!= mContactList.end();
         ++i)
    {
        dJointFeedback *feedback = dJointGetFeedback(i->second);
        if (feedback)
        {
            Vector3f forceVec(feedback->f1[0], feedback->f1[1], feedback->f1[2]);
            force += forceVec;
            pos += Vector3f(i->first.pos[0], i->first.pos[1], i->first.pos[2])
                    * forceVec.Length();
            sumLength += forceVec.Length();
            
            // Sometimes the Percept function is called more than once in a 
            // time step. Double freeing is avoided using this line!
            dJointSetFeedback(i->second, 0);
            delete feedback;
        }
    }
    
    // It should be always true, except when feedback == NULL
    if (sumLength > 0.001)
    {
        mLastPoint = (pos / sumLength ) - mBody->GetLocalTransform().Pos();
        mLastForce = force;
    }
    
    ParameterList& posElement = predicate.parameter.AddList();
    posElement.AddValue(std::string("c"));
    posElement.AddValue(mLastPoint.x());
    posElement.AddValue(mLastPoint.y());
    posElement.AddValue(mLastPoint.z());

    ParameterList& forceElement = predicate.parameter.AddList();
    forceElement.AddValue(std::string("f"));
    forceElement.AddValue(mLastForce.x());
    forceElement.AddValue(mLastForce.y());
    forceElement.AddValue(mLastForce.z());

    return true;
}

void ForceResistancePerceptor::PrePhysicsUpdateInternal(float deltaTime)
{
    mContactList.clear();
}
