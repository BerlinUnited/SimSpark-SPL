/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: body.h 106 2009-11-19 10:10:50Z a-held $

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
#include <oxygen/physicsserver/concavecollider.h>
#include <oxygen/physicsserver/int/concavecolliderint.h>
#include <iostream>

using namespace oxygen;
using namespace boost;

boost::shared_ptr<ConcaveColliderInt> ConcaveCollider::mConcaveColliderImp;

ConcaveCollider::ConcaveCollider() : Collider()
{

}

ConcaveCollider::~ConcaveCollider()
{

}

bool ConcaveCollider::ConstructInternal()
{
    if (mConcaveColliderImp.get() == 0)
        mConcaveColliderImp = shared_dynamic_cast<ConcaveColliderInt>
            (GetCore()->New("ConcaveColliderImp"));
            
    if (mConcaveColliderImp.get() == 0)
        {
            //we can't use the logserver here
            std::cerr << "(ConcaveCollider) ERROR: No implementation found at '/classes/ConcaveColliderImp'";
            return false;
        }
    
    if (!Collider::ConstructInternal()) return false;
    
    //we can't use the logserver here
    std::cerr << "(ConcaveCollider) ERROR: ConcaveCollider is not implemented yet. Did nothing";
    
    return true;
}
