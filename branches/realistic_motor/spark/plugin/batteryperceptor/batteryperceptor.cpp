/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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

#include "batteryperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace zeitgeist;
using namespace salt;
using namespace std;

BatteryPerceptor::BatteryPerceptor() : oxygen::Perceptor()
{
}

BatteryPerceptor::~BatteryPerceptor()
{
}

void BatteryPerceptor::OnLink()
{ 
  mBattery = shared_static_cast<Battery>(FindParentSupportingClass<Battery>().lock());
  if (0 == mBattery.get())
  {
    GetLog()->Error()<< "(BatteryPerceptor) ERROR: can not link to battery\n";
  }
}

void BatteryPerceptor::OnUnLink()
{
  mBattery.reset();
}

bool BatteryPerceptor::Percept(boost::shared_ptr<PredicateList> predList)
{
  Predicate &predicate = predList->AddPredicate();
  predicate.name = GetName();
  predicate.parameter.Clear();

  float v = 0;
  if(0 != mBattery.get())
  {
    v = mBattery->State();
  }

  predicate.parameter.AddValue(v);

  return true;
}
