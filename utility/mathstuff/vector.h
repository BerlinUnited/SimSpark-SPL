/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _VECTOR_H_
#define _VECTOR_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

#include "mathstuff.h"

/*! Vector - Vector Math Classes
  $Id: vector.h,v 1.1 2002/08/14 13:05:07 fruit Exp $

    This header implements some useful vector math classes. Functions
    are inlined for efficiency. Operators are overloaded for usability.

    HISTORY:
    The console/variable subsystem was taken from a student project at
    the AI Research Group, Koblenz University. Original development by
    Alexander Fuchs <alexf@uni-koblenz.de>,
    Marco Koegler <koegler@uni-koblenz.de>, 
    Marcus Rollmann <rollmark@uni-koblenz.de>, et.al.
*/

//! enum type used to describe a dominant axis
enum EAxis
{
    eAxisX          = 0,
    eAxisY          = 1,
    eAxisZ          = 2,
    //! used to force the enum to be 32-bit wide
    eAxisForce32Bit = 0xffffffff
};

/*! \class Vector3

    A 3-dimensional vector with 'TFloat' precision (usually double or
    float) for each coordinate. All functions and members are public,
    since this is a pretty basic type.
*/
class Vector3
{
public:
    TFloat _v[3];

    inline const TFloat& x() const { return _v[0]; }
    inline const TFloat& y() const { return _v[1]; }
    inline const TFloat& z() const { return _v[2]; }

    inline Vector3() { }
    inline Vector3(TFloat vx, TFloat vy, TFloat vz) 
    { _v[0] = vx; _v[1] = vy; _v[2] = vz; }
    inline Vector3(TFloat va[3]) 
    { _v[0] = va[0]; _v[1] = va[1]; _v[2] = va[2];  }
    inline Vector3(const Vector3& v) 
    { _v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2];  }

    inline void set(TFloat vx, TFloat vy, TFloat vz) 
    {_v[0]=vx; _v[1]=vy; _v[2]=vz; }
    inline void set(const Vector3& v)  
    { _v[0]=v._v[0]; _v[1]=v._v[1]; _v[2]=v._v[2]; }
    inline EAxis getDominantAxis()                                            
    { return ((fabs(_v[0]) > fabs(_v[1])) ? 
          ((fabs(_v[0]) > fabs(_v[2])) ? eAxisX : eAxisZ) : 
          ((fabs(_v[1]) > fabs(_v[2])) ? eAxisY : eAxisZ)); }
    // math
    inline TFloat dot(const Vector3 &v) const
    { return (_v[0]*v._v[0] + _v[1]*v._v[1] + _v[2]*v._v[2]); }
    inline Vector3 cross(const Vector3 &v) const                            
    { return Vector3(_v[1]*v._v[2] - _v[2]*v._v[1], 
             _v[2]*v._v[0] - _v[0]*v._v[2], 
             _v[0]*v._v[1] - _v[1]*v._v[0]); }
    inline TFloat squareLength() const
    { return (_v[0]*_v[0] + _v[1]*_v[1] + _v[2]*_v[2]); }
    inline TFloat length() const
    { return (TFloat)sqrt(squareLength()); }
    inline Vector3& normalize()
    { TFloat l=length(); TFloat iLen = 1.0f/l;    
      _v[0] *= iLen; _v[1] *= iLen; _v[2] *= iLen; 
      return *this; }
    inline Vector3 normalized() const
    { TFloat l=length(); TFloat iLen = 1.0f/l;    
      return Vector3(_v[0]*iLen, _v[1]*iLen, _v[2]*iLen); }
    inline Vector3 reflect(const Vector3& n)
    { TFloat fac=2.0f*(n._v[0]*_v[0]+n._v[1]*_v[1]+n._v[2]*_v[2]);        
      return Vector3(fac*n._v[0]-_v[0],
             fac*n._v[1]-_v[1],
             fac*n._v[2]-_v[2]); }
    // operators
    inline Vector3 operator -() const
    { return Vector3(-_v[0], -_v[1], -_v[2]); }
    inline const Vector3& operator +=(const Vector3& v)
    { _v[0]+=v._v[0]; _v[1]+=v._v[1]; _v[2]+=v._v[2]; return *this; }
    inline const Vector3& operator -=(const Vector3& v)                            { _v[0]-=v._v[0]; _v[1]-=v._v[1]; _v[2]-=v._v[2]; return *this; }
    inline const Vector3& operator *=(TFloat m)
    { _v[0]*=m; _v[1]*=m; _v[2]*=m; return *this; }
    inline const Vector3& operator /=(TFloat d)   
    { TFloat iMul=1.0f/d; 
      _v[0]*=iMul; _v[1]*=iMul; _v[2]*=iMul; 
      return *this; }
    inline const Vector3& operator =(const Vector3& v)
    { _v[0]=v._v[0]; _v[1]=v._v[1]; _v[2]=v._v[2]; 
      return *this; }
    inline bool operator ==(const Vector3& in) const
    { return ((_v[0]==in._v[0]) && (_v[1]==in._v[1]) && (_v[2]==in._v[2])); }
    inline bool operator !=(const Vector3& in) const
    { return ((_v[0]!=in._v[0]) || (_v[1]!=in._v[1]) || (_v[2]!=in._v[2])); }
    inline TFloat& operator[](int row)
    { return _v[row]; }
    inline operator TFloat*()
    { return (TFloat*)_v; }
    inline operator const TFloat*() const
    { return (const TFloat*)_v; }
    void Dump()    
    { cout << _v[0] << " " << _v[1] << _v[2] << "\n"; }
};

// Vector3 operators
inline Vector3    
operator +(const Vector3& v, const Vector3& v2)                    
{ return Vector3(v._v[0]+v2._v[0], v._v[1]+v2._v[1], v._v[2]+v2._v[2]); }
inline Vector3    
operator -(const Vector3& v, const Vector3& v2)
{ return Vector3(v._v[0]-v2._v[0], v._v[1]-v2._v[1], v._v[2]-v2._v[2]); }
inline Vector3    
operator *(const Vector3& v, TFloat m)
{ return Vector3(v._v[0]*m, v._v[1]*m, v._v[2]*m); }
inline Vector3 
operator *(TFloat m, const Vector3& v)
{ return Vector3(m*v._v[0], m*v._v[1], m*v._v[2]); }
inline Vector3 
operator /(const Vector3& v, TFloat d)
{ TFloat iMul=1.0f/d; 
  return Vector3(v._v[0]*iMul, v._v[1]*iMul, v._v[2]*iMul); }
inline Vector3    
operator /(TFloat d, const Vector3& v)
{ return Vector3(d/v._v[0], d/v._v[1], d/v._v[2]); }
inline Vector3    
operator %(const Vector3& v, const Vector3& v2)
{ return Vector3(v._v[1]*v2._v[2] - v._v[2]*v2._v[1], 
         v._v[2]*v2._v[0] - v._v[0]*v2._v[2], 
         v._v[0]*v2._v[1] - v._v[1]*v2._v[0]); }

/*  \class Vector2

    A 2-dimensional vector with 'TFloat' precision (usually double or
    float) for each coordinate. All functions and members are public,
    since this is a pretty basic type.
*/
class Vector2
{
// Members
public:
    TFloat _v[2];

// Methods
public:
    // Initialization
    inline Vector2() { }

    inline const TFloat& x() const { return _v[0]; }
    inline const TFloat& y() const { return _v[1]; }

    inline Vector2(TFloat u, TFloat v) { _v[0] = u; _v[1] = v; }
    inline Vector2(const Vector2 &v) { _v[0] = v._v[0]; _v[1] = v._v[1]; }

    // misc functions
    inline void set(TFloat u, TFloat v)
    { _v[0]=u; _v[1]=v; }

    // math
    inline TFloat squareLength() const
    { return (_v[0]*_v[0] + _v[1]*_v[1]); }
    inline TFloat length() const
    { return sqrt(_v[0]*_v[0] + _v[1]*_v[1]); }
    inline TFloat dot(const Vector2& rhs) const
    { return (_v[0]*rhs._v[0] + _v[1]*rhs._v[1]); }
    inline Vector2& normalize()
    { TFloat l=length(); TFloat ooL=1.0/l;    
      _v[0] *= ooL; _v[1] *= ooL; 
      return *this; }
    inline Vector2 normalized() const 
    { TFloat l=length(); TFloat ooL=1.0/l;    
      return Vector2(_v[0]*ooL, _v[1]*ooL); }
    inline bool isNormalized() const
    { return (squareLenGth() == 1.0f); }

    // operators
    inline bool operator ==(const Vector2& vec)
    { return ((_v[0]==vec._v[0]) && (_v[1]==vec._v[1])); }
    inline bool operator !=(const Vector2& vec)
    { return ((_v[0]!=vec._v[0]) || (_v[1]!=vec._v[1])); }
    inline Vector2 operator -() const
    { return Vector2(-_v[0], -_v[1]); }
    inline const Vector2& operator +=(const Vector2& vec) 
    { _v[0]+=vec._v[0]; _v[1]+=vec._v[1]; 
      return *this; }
    inline const Vector2& operator -=(const Vector2& vec) 
    { _v[0]-=vec._v[0]; _v[1]-=vec._v[1]; 
    return *this; }    
    inline const Vector2& operator *=(TFloat s)
    { _v[0]*=s; _v[1]*=s; return *this; }
    inline const Vector2& operator /=(TFloat d)
    { TFloat ooD=1.0/d; _v[0]*=ooD; _v[1]*=ooD; 
      return *this; }
    inline const Vector2& operator =(const Vector2& v)
    { _v[0]=v._v[0]; _v[1]=v._v[1]; return *this; }
    inline const Vector2& operator*=(const Vector2& vec)   
    { _v[0]*=vec._v[0]; _v[1]*=vec._v[1]; 
      return *this; }
    inline TFloat& operator[](int row)
    { return _v[row]; }
    inline operator TFloat*()
    { return (TFloat*)_v; }
    inline operator const TFloat*() const
    { return (const TFloat*)_v; }
};

inline Vector2 operator +(const Vector2& v1, const Vector2& v2)
{ return Vector2(v1._v[0]+v2._v[0], v1._v[1]+v2._v[1]); }
inline Vector2 operator -(const Vector2& v1, const Vector2& v2)
{ return Vector2(v1._v[0]-v2._v[0], v1._v[1]-v2._v[1]); }
inline Vector2 operator *(const Vector2& v, TFloat m)
{ return Vector2(v._v[0]*m, v._v[1]*m); }
inline Vector2 operator /(const Vector2& v, TFloat d)
{ TFloat  ooD=1.0/d; return Vector2(v._v[0]*ooD, v._v[1]*ooD); }
inline Vector2 operator *(TFloat s, const Vector2& v)
{ return Vector2(s*v._v[0], s*v._v[1]); }
inline Vector2 operator /(TFloat d, const Vector2& v)
{ return Vector2(d/v._v[0], d/v._v[1]); }

#endif //_VECTOR_H_
