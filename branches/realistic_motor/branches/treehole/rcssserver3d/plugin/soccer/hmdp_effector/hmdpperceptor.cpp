/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

 this file is part of rcssserver3D

 Copyright (C) 2002,2003 Koblenz University
 Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
 Copyright (C) 2008 N. Michael Mayer, email: nmmayer@gmail.com


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
#include "hmdpperceptor.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <salt/vector.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;

extern HMDPPerceptor *hmdpPerceptorHandle; //! for HMDPEffector

HMDPPerceptor::HMDPPerceptor() :
    oxygen::Perceptor()
{
    hmdpPerceptorHandle = this; //! for HMDPEffector

}

HMDPPerceptor::~HMDPPerceptor()
{
}

void HMDPPerceptor::OnLink()
{

    messageOut = "";
    shared_ptr<Transform> transformParent = shared_static_cast<Transform> (
        FindParentSupportingClass<Transform> ().lock());

    mBody = shared_static_cast<RigidBody> (transformParent->GetChildOfClass("RigidBody"));
}

void HMDPPerceptor::OnUnlink()
{
    mBody.reset();
}

void HMDPPerceptor::sendMessage(std::string out) //! called by effector object
{
    messageOut = messageOut + out + ";";
}

bool HMDPPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{

    while (messageOut != "")
    {
        int i1 = messageOut.find(";");
        if (i1 < 0)
            i1 = messageOut.size();
        std::string out = messageOut.substr(0, i1);
        if ((i1 + 1) < messageOut.size())
            messageOut = messageOut.substr(i1 + 1, messageOut.size());
        else
            messageOut = "";

        Predicate &predicate = predList->AddPredicate();
        predicate.name = "hmdp " + out;
        predicate.parameter.Clear();

    }
    return true;
}
