/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: collider.h,v 1.2 2003/08/29 22:08:21 rollmark Exp $

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

   Collider
*/


#ifndef COLLIDER_H__
#define COLLIDER_H__

#include "odeobject.h"

namespace kerosin
{

class Space;
class World;

/** Collider is the base class of all colliders.
*/
class Collider : public ODEObject
{
	//
	// Functions
	//
public:
	Collider();
	virtual ~Collider();

protected:
	virtual void OnLink();
	virtual void OnUnlink();

	//
	// Members
	//
protected:
	//! the world this collider is associated with
	boost::shared_ptr<World>	mWorld;
	//! the space this collider is associated with
	boost::shared_ptr<Space>	mSpace;
	//! the ode collision geometry
	dGeomID						mODEGeom;	
};

DECLARE_CLASS(Collider);

} //namespace kerosin

#endif //COLLIDER_H__
