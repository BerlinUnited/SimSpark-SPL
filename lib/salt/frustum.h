/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: frustum.h,v 1.2 2003/05/19 21:32:40 fruit Exp $

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
#ifndef FRUSTUM_H__
#define FRUSTUM_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "defines.h"
#include "plane.h"
#include "bounds.h"

/*	$Id: frustum.h,v 1.2 2003/05/19 21:32:40 fruit Exp $

	Frustum

	This class implements a frustum, which is an arbitrary volume in space (possibly infinite)
	defined by a set of planes. This class also provides some 'special' frustra construction
	methods. 

	NOTE:

	HISTORY:
		11.06.01 - MK
			- Initial version
		23.03.02 - MK
			- Intersection/Inclusion-tests


	TODO:
		- Create frustum from a (view)point and a polygon
		- Create a view frustum given field-of-views

	TOFIX:
*/

namespace salt
{

class Frustum
{
// Members
public:
	enum ePlaneID
	{
		PI_NEAR		= 0,
		PI_LEFT		= 1,
		PI_RIGHT	= 2,
		PI_FAR		= 3,
		PI_BOTTOM	= 4,
		PI_TOP		= 5
	};
	enum eFrustumSide
	{
		FS_INSIDE  = 0,
		FS_OUTSIDE = 1,
		FS_SPLIT   = 2
	};

	
	Vector3f	mBasePos;			// Position from where the frustum is cast
	Plane		mPlanes[6];			// 6 Plane equations

// Methods
public:
	void Dump() const;
	void Set(const Matrix& worldTransform, float fov=60.0f, float zNear=0.1f, float zFar=2000.0f, float aspect=0.75f);
	eFrustumSide Intersects(const AABB3& bb) const;
};

} // namespace salt

#endif //FRUSTUM_H__
