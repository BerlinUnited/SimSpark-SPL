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

#include "forceresistanceperceptor.h"
#include <../plugin/odeimps/odewrapper.h>
#include <oxygen/sceneserver/transform.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace oxygen;
using namespace zeitgeist;

ForceResistancePerceptor::ForceResistancePerceptor() :
    mLastForce(0, 0, 0), mLastCenter(0, 0, 0)
{
}

void ForceResistancePerceptor::OnLink()
{
    Perceptor::OnLink();

    mBody = static_pointer_cast<Transform>(FindParentSupportingClass<Transform>().lock());

    if (mBody.get() == 0)
        GetLog()->Error()
                << "(ForceResistancePerceptor) ERROR: no suitable parent node found!\n";
}

void ForceResistancePerceptor::OnUnlink()
{
    Perceptor::OnUnlink();
    mBody.reset();
}

GenericJointFeedback* ForceResistancePerceptor::AddTouchInfo(oxygen::GenericContact& contact)
{
    dContact& ODEContact = (dContact&) contact;
    mContactList.push_front(make_pair(ODEContact.geom, dJointFeedback()));
    return (GenericJointFeedback*) &mContactList.front().second;
}

bool ForceResistancePerceptor::Percept(
        boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mContactList.empty())
    {
        mLastForce.Zero();
        mLastCenter.Zero();
        return false;
    }

    Vector3f force(0,0,0);
    Vector3f center(0,0,0);
    float sumLength = 0;
    for (TContactList::const_iterator i = mContactList.begin();
         i!= mContactList.end();
         ++i)
        {
            Vector3f forceVec(i->second.f1[0], i->second.f1[1], i->second.f1[2]);
            force += forceVec;

            float forcevalue = forceVec.Length();
            center += Vector3f(i->first.pos[0], i->first.pos[1], i->first.pos[2])
                    * forcevalue;
            sumLength += forcevalue;
        }

    if (sumLength != 0) // sumLength will be zero when the body is disabled
        {
            Matrix invRot = mBody->GetLocalTransform();
            invRot.InvertRotationMatrix();
            mLastCenter = invRot * (center / sumLength);
            mLastForce = invRot.Rotate(force);
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "FRP";
    predicate.parameter.Clear();

    ParameterList& nameElement = predicate.parameter.AddList();
    nameElement.AddValue(std::string("n"));
    nameElement.AddValue(GetName());

    ParameterList& posElement = predicate.parameter.AddList();
    posElement.AddValue(std::string("c"));
    posElement.AddValue(mLastCenter.x());
    posElement.AddValue(mLastCenter.y());
    posElement.AddValue(mLastCenter.z());

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
