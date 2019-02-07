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
#ifndef OXYGEN_STATICBODY_H
#define OXYGEN_STATICBODY_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/body.h>

namespace oxygen
{
class StaticBodyInt;
/**
    StaticBody is not yet implemented.
*/

class OXYGEN_API StaticBody : public Body
{
    //
    // Functions
    //
public:
    StaticBody();
    virtual ~StaticBody();
    
    virtual void OnLink();
    
    static boost::shared_ptr<StaticBodyInt> mStaticBodyImp;
};

DECLARE_CLASS(StaticBody);

} //namespace oxygen

#endif //OXYGEN_STATICBODY_H
