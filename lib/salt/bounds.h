/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: bounds.h,v 1.2 2003/05/19 21:37:49 fruit Exp $

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
#ifndef BOUNDS_H__
#define BOUNDS_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "defines.h"
#include "matrix.h"
#include "vector.h"

#include <cfloat>

namespace salt
{

class AABB3
{
public:
	// constructors
	f_inline AABB3()															{ Init(); }
	f_inline AABB3(const Vector3f &mn, const Vector3f &mx)						{ Init(); Encapsulate(mn); Encapsulate(mx); }


	// inline functions
	f_inline void Init()													{ minVec.Set(FLT_MAX, FLT_MAX, FLT_MAX); maxVec.Set(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	f_inline void Encapsulate(const Vector3f &v)								{ minVec.x() = gMin(minVec.x(), v.x()); minVec.y() = gMin(minVec.y(), v.y()); minVec.z() = gMin(minVec.z(), v.z()); maxVec.x() = gMax(maxVec.x(), v.x()); maxVec.y() = gMax(maxVec.y(), v.y()); maxVec.z() = gMax(maxVec.z(), v.z()); }
	f_inline void Encapsulate(const float x, const float y, const float z)	{ minVec.x() = gMin(minVec.x(), x); minVec.y() = gMin(minVec.y(), y); minVec.z() = gMin(minVec.z(), z); maxVec.x() = gMax(maxVec.x(), x); maxVec.y() = gMax(maxVec.y(), y); maxVec.z() = gMax(maxVec.z(), z); }
	f_inline void Encapsulate(const AABB3 &box)								{ Encapsulate(box.minVec); Encapsulate(box.maxVec);}
	f_inline void Widen(float delta)										{ minVec.x()-=delta; minVec.y()-=delta; minVec.z()-=delta; maxVec.x()+=delta; maxVec.y()+=delta; maxVec.z()+=delta; }
	f_inline void Translate(const Vector3f &v)								{ minVec+=v; maxVec+=v; }

	f_inline bool Contains(const Vector3f &v) const							{ return (gInRange(v.x(), minVec.x(), maxVec.x()) && gInRange(v.z(), minVec.z(), maxVec.z()) && gInRange(v.y(), minVec.y(), maxVec.y())); }
	f_inline bool Contains(const AABB3 &b) const							{ return (Contains(b.minVec) && Contains(b.maxVec)); }
	f_inline bool Intersects(const AABB3 &b) const							{ return !(minVec.x() > b.maxVec.x() || maxVec.x() < b.minVec.x() || minVec.y() > b.maxVec.y() || maxVec.y() < b.minVec.y() || minVec.z() > b.maxVec.z() || maxVec.z() < b.minVec.z()); }

	f_inline float		GetWidth() const									{ return gAbs(minVec.x()-maxVec.x()); }			// get width of bounding box
	f_inline float		GetHeight() const									{ return gAbs(minVec.y()-maxVec.y()); }			// get height of bounding box
	f_inline float		GetDepth() const									{ return gAbs(minVec.z()-maxVec.z()); }			// get depth of bounding box
	f_inline Vector3f	GetMiddle() const									{ return Vector3f((minVec.x()+maxVec.x())*0.5f, (minVec.y()+maxVec.y())*0.5f, (minVec.z()+maxVec.z())*0.5f); }
	f_inline float		GetRadius() const									{ return ((maxVec-minVec)*0.5).Length(); }	// get distance from middle of bounding box to one of the corners (i.e. radius of bounding sphere through Middle()).

	void	TransformBy(Matrix& matrix);

	// attributes
	Vector3f		minVec;
	Vector3f		maxVec;
};

class AABB2
{
public:
	// constructors
	f_inline AABB2()															{ Init(); }
	f_inline AABB2(const Vector2f &mn, const Vector2f &mx)						{ Init(); Encapsulate(mn); Encapsulate(mx); }


	// inline functions
	f_inline void Init()													{ minVec.Set(FLT_MAX, FLT_MAX); maxVec.Set(FLT_MIN, FLT_MIN); }
	f_inline void Encapsulate(const Vector2f &v)								{ minVec.x() = gMin(minVec.x(), v.x()); minVec.y() = gMin(minVec.y(), v.y()); maxVec.x() = gMax(maxVec.x(), v.x()); maxVec.y() = gMax(maxVec.y(), v.y()); }
	f_inline void Encapsulate(const AABB2 &box)								{ Encapsulate(box.minVec); Encapsulate(box.maxVec);}
	f_inline void Widen(float delta)										{ minVec.x()-=delta; minVec.y()-=delta; maxVec.x()+=delta; maxVec.y()+=delta; }
	f_inline void Translate(const Vector2f &v)								{ minVec+=v; maxVec+=v; }

	f_inline bool Contains(const Vector2f &v) const							{ return (gInRange(v.x(), minVec.x(), maxVec.x()) && gInRange(v.y(), minVec.y(), maxVec.y())); }
	f_inline bool Contains(const AABB2 &b) const							{ return (Contains(b.minVec) && Contains(b.maxVec)); }
	f_inline bool Intersects(const AABB2 &b) const							{ return !(minVec.x() > b.maxVec.x() || maxVec.x() < b.minVec.x() || minVec.y() > b.maxVec.y() || maxVec.y() < b.minVec.y()); }

	f_inline float		GetWidth() const									{ return gAbs(minVec.x()-maxVec.x()); }			// get width of bounding box
	f_inline float		GetHeight() const									{ return gAbs(minVec.y()-maxVec.y()); }			// get height of bounding box
	f_inline Vector2f	GetMiddle() const									{ return Vector2f((minVec.x()+maxVec.x())*0.5f, (minVec.y()+maxVec.y())*0.5f); }
	f_inline float		GetRadius() const									{ return ((maxVec-minVec)*0.5).Length(); }	// get distance from middle of bounding box to one of the corners (i.e. radius of bounding sphere through Middle()).

	// attributes
	Vector2f		minVec;
	Vector2f		maxVec;
};


// bounding sphere
class BoundingSphere
{
public:
	// constructors
	f_inline BoundingSphere()												: center(Vector3f(0,0,0)), radius(0.0f), radiusSq(0.0f) {}
	f_inline BoundingSphere(const Vector3f &pos, float rad)					: center(pos), radius(rad), radiusSq(rad*rad) {}
	f_inline BoundingSphere(const Vector3f &pos, float rad, float radSq)		: center(pos), radius(rad), radiusSq(radSq)	{}


	// inline functions
	f_inline void EncapsulateFast(const Vector3f &v)						{ Vector3f diff=(center - v);	float dist=diff.Dot(diff); if (dist>radiusSq) {	radiusSq=dist; radius=gSqrt(dist); }}	// not accurate
	f_inline bool Contains(const Vector3f &v)								{ return ((center - v).SquareLength() < radiusSq); }
	f_inline bool Contains(const BoundingSphere &s) const					{ return ((center - s.center).SquareLength() < (radiusSq - s.radiusSq)); }
	f_inline bool Intersects(const BoundingSphere &s) const					{ return ((center - s.center).SquareLength() < (radiusSq + s.radiusSq)); }

	// non-inline functions
	void Encapsulate(const Vector3f &v);		// accurate encapsulation
	bool Contains(const AABB3 &b) const;
	bool Intersects(const AABB3 &b) const;


	// attributes
	Vector3f	center;
	float		radius;
	float		radiusSq;
};

} // namespace salt

#endif // BOUNDS_H__
