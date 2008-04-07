/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

 this file is part of rcssserver3D
 Fri May 9 2003
 Copyright (C) 2002,2003 Koblenz University
 Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
 $Id: forceresistanceperceptor.cpp,v 1.7 2008/04/07 21:37:44 hedayat Exp $

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
                << "(ForceResistancePerceptor) ERROR: no suitable parent node found!\n";
}

void ForceResistancePerceptor::OnUnlink()
{
    Perceptor::OnUnlink();
    mBody.reset();
}

dJointFeedback *ForceResistancePerceptor::AddTouchInfo(dContact &contact)
{
    mContactList.push_front(make_pair(contact.geom, dJointFeedback()));
    return &mContactList.front().second;
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

    if (sumLength == 0)
        return false;

    Matrix invRot = mBody->GetLocalTransform();
    invRot.InvertRotationMatrix();
    center = invRot * (center / sumLength);
    force = invRot.Rotate(force);

    ParameterList& posElement = predicate.parameter.AddList();
    posElement.AddValue(std::string("c"));
    posElement.AddValue(center.x());
    posElement.AddValue(center.y());
    posElement.AddValue(center.z());

    ParameterList& forceElement = predicate.parameter.AddList();
    forceElement.AddValue(std::string("f"));
    forceElement.AddValue(force.x());
    forceElement.AddValue(force.y());
    forceElement.AddValue(force.z());

    return true;
}

void ForceResistancePerceptor::PrePhysicsUpdateInternal(float deltaTime)
{
    mContactList.clear();
}
