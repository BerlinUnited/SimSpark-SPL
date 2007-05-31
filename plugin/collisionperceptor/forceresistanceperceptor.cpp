/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceresistanceperceptor.cpp,v 1.1 2007/05/31 17:39:54 hedayat Exp $

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

using namespace std;
using namespace boost;
using namespace oxygen;
using namespace zeitgeist;

void ForceResistancePerceptor::AddTouchInfo(dContact &contact, dJointID contactJointID)
{
	mContactList.push_front(make_pair(contact, contactJointID));
}

bool ForceResistancePerceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
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

     for (
          ContactListType::const_iterator i = mContactList.begin();
          i != mContactList.end();
          ++i
          )
       {
         ParameterList &collElement = predicate.parameter.AddList();
         collElement.AddValue(string("pt"));
         
         ParameterList &posElement = collElement.AddList();
         posElement.AddValue(string("pos"));
         posElement.AddValue(static_cast<float>(i->first.geom.pos[0]));
         posElement.AddValue(static_cast<float>(i->first.geom.pos[1]));
         posElement.AddValue(static_cast<float>(i->first.geom.pos[2]));
         
         dJointFeedback *feedback = dJointGetFeedback(i->second);
         if (feedback)
         {
             ParameterList &forceElement = collElement.AddList();
             forceElement.AddValue(string("f"));
             forceElement.AddValue(static_cast<float>(feedback->f1[0]));
             forceElement.AddValue(static_cast<float>(feedback->f1[1]));
             forceElement.AddValue(static_cast<float>(feedback->f1[2]));
             // Sometimes the Percept function is called more than once in a 
             // time step. Double freeing is avoided using this line!
             dJointSetFeedback(i->second, 0);
             delete feedback;
         }
       }

    return true;
}

void ForceResistancePerceptor::PrePhysicsUpdateInternal(float deltaTime)
{
	mContactList.clear();
}
