/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: planecollider.h,v 1.2 2003/08/29 22:08:21 rollmark Exp $

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

   PlaneCollider
*/

#ifndef PLANECOLLIDER_H__
#define PLANECOLLIDER_H__

#include "collider.h"

namespace kerosin
{

class PlaneCollider : public Collider
{
	//
	// Functions
	//
public:
	PlaneCollider();

	void SetParams(float a, float b, float c, float d);

protected:
	virtual bool ConstructInternal();
};

DECLARE_CLASS(PlaneCollider);

} //namespace kerosin

#endif //PLANECOLLIDER_H__
