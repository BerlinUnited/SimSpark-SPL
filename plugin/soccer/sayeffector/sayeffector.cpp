/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sayeffector.cpp,v 1.1 2004/05/07 17:04:17 markelic Exp $

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
#include "sayaction.h"
#include "sayeffector.h"
#include <zeitgeist/logserver/logserver.h>


using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

SayEffector::SayEffector()
{
}

SayEffector::~SayEffector()
{
}

bool
SayEffector::Realize(boost::shared_ptr<ActionObject> action)
{
   
   

    shared_ptr<SayAction> sayAction =
        shared_dynamic_cast<SayAction>(action);

    if (sayAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    mText = sayAction->GetText();
    ifText=true;
    return true;
}

std::string
SayEffector::GetText()
{
    ifText=false;
    return mText;
}

bool
SayEffector::IfText()const
{
    return ifText;
}

shared_ptr<ActionObject>
SayEffector::GetActionObject(const Predicate& predicate)
{
  do
  {
      if (predicate.name != GetPredicate())
          {
              GetLog()->Error() << "ERROR: (SayEffector) invalid predicate"
                                << predicate.name << "\n";
              break;
          }

      Predicate::Iterator iter = predicate.begin();

      std::string text;
      if (! predicate.AdvanceValue(iter, text))
      {
          GetLog()->Error()
              << "ERROR: (SayEffector) said message expected\n";
          break;
      }

     

      // construct the SayAction object
      return shared_ptr<SayAction>(new SayAction(GetPredicate(),text));

  } while (0);

  // some error happened
  return shared_ptr<ActionObject>();
}

