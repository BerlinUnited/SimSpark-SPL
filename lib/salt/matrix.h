/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: matrix.h,v 1.2 2003/05/19 21:32:40 fruit Exp $

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
#ifndef MATRIX_H__
#define MATRIX_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "defines.h"
#include "vector.h"

#include <memory.h>

namespace salt
{
#if 0
}
#endif

class Matrix
{
public:
    float m[16];			// 16 float values
    f_inline Matrix(){}		// do nothing constructor
    f_inline Matrix(float *newMatrix)	{	memcpy(m, newMatrix,	sizeof(float)*16);	}
    f_inline Matrix(float m00, float m01, float m02, float m03,
                    float m10, float m11, float m12, float m13,
                    float m20, float m21, float m22, float m23,
                    float m30, float m31, float m32, float m33);
    f_inline Matrix(const Matrix &newMatrix)	{	memcpy(m, newMatrix.m,	sizeof(float)*16);	}
    f_inline Matrix(Matrix *newMatrix)			{	memcpy(m, newMatrix->m, sizeof(float)*16);	}

    // matrix initializations
    f_inline void Identity()			{	memcpy(m, mIdentity,	sizeof(float)*16);	}
    f_inline void RotationX(float inAngle);				// Create rotation matrix
    f_inline void RotationY(float inAngle);				// Create rotation matrix
    f_inline void RotationZ(float inAngle);				// Create rotation matrix
    f_inline void Translation(const Vector3f &inVector);		// Create translation matrix
    f_inline void Scale(const Vector3f & inVector);			// Create scale matrix
    void LookAt(const Vector3f & inEye, const Vector3f & inDirection, const Vector3f & inUp);

    // misc functions (subject to change)
    void	SetModelView();
    void	GetModelView();
    void	Dump() const;
    f_inline void	Set(float m00, float m01, float m02, float m03,
                            float m10, float m11, float m12, float m13,
                            float m20, float m21, float m22, float m23,
                            float m30, float m31, float m32, float m33);
	
    f_inline const Matrix &	RotateX(float inAngle);			// Rotate the matrix around the axis X with <inAngle> radians
    f_inline const Matrix &	RotateY(float inAngle);			// Rotate the matrix around the axis Y with <inAngle> radians
    f_inline const Matrix &	RotateZ(float inAngle);			// Rotate the matrix around the axis Z with <inAngle> radians
    f_inline const Matrix &	Translate(const Vector3f & inVector);	// Translate the matrix by vector <inVector>

    f_inline const Vector3f &	Right() const		{ return *(const Vector3f*) &El(0, 0); }
    f_inline Vector3f &	Right()				{ return *(Vector3f*)  &El(0, 0); }
    f_inline const Vector3f &	Up() const			{ return *(const Vector3f*) &El(0, 1); }
    f_inline Vector3f &	Up()				{ return *(Vector3f*)  &El(0, 1); }
    f_inline const Vector3f &	Forward() const		{ return *(const Vector3f*) &El(0, 2); }
    f_inline Vector3f &	Forward()			{ return *(Vector3f*)  &El(0, 2); }
    f_inline const Vector3f &	Pos() const			{ return *(const Vector3f*) &El(0, 3); }
    f_inline Vector3f &	Pos()				{ return *(Vector3f*)  &El(0, 3); }

    bool	IsEqual(const Matrix& matrix) const;
    // matrix inversion (rotation matrix only)
    f_inline void		InvertRotationMatrix();

    // matrix*vector (including some special case 'hacks')
    f_inline Vector3f	Transform(const Vector3f & inVector);
    f_inline Vector3f	Rotate(const Vector3f & inVector);
    f_inline Vector3f	InverseRotate(const Vector3f & inVector);

    // special lighting matrices
    void CalcAttenuationNoRotation(const Vector3f &pos, float radius);
    void CalcAttenuationWithRotation(const Matrix &lightWorldMatrix, float radius);
    void CalcInfiniteProjection(float width, float height, float fov, float zNear);
    void CalcInfiniteFrustum(float left, float right, float bottom, float top, float zNear);
    void CalcSpotLight(const Matrix &lightWorldTransform, float fov, float width, float height, float zNear);

    // Operators
    f_inline const Matrix	operator*(const Matrix &inRHS) const;	// Multiply this Matrix with another Matrix
    f_inline const Vector3f	operator*(const Vector3f &inRHS) const;	// Multiply a Vector3f with this Matrix
    f_inline const Vector3f	operator*(const Vector2f &inRHS) const;	// Multiply a Vector2 with this Matrix
    f_inline const Matrix &	operator*=(const Matrix &inRHS);		// Multiply this Matrix with another Matrix


    // Element access operators
    f_inline float& operator() (int inRow, int inColumn) {	return El(inRow, inColumn);	}
    f_inline const float& operator() (int inRow, int inColumn) const {	return El(inRow, inColumn);	}
protected:
    f_inline float& El(int inRow, int inColumn)	{	return m[inColumn*4 + inRow];	}
    f_inline const float& El(int inRow, int inColumn) const {	return m[inColumn*4 + inRow];	}

private:
    static float mIdentity[16];
};

f_inline Matrix::Matrix(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33)
{
    m[0] = m00;	m[4] = m01;	m[8] = m02;	m[12] = m03;
    m[1] = m10;	m[5] = m11;	m[9] = m12;	m[13] = m13;
    m[2] = m20;	m[6] = m21;	m[10]= m22;	m[14] = m23;
    m[3] = m30;	m[7] = m31;	m[11]= m32;	m[15] = m33;
}

f_inline void Matrix::RotationX(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);
    
    // Create X-Rotation matrix
    Identity();
    El(1, 1) = c;
    El(2, 1) = s;
    El(1, 2) = -s;
    El(2, 2) = c;
}

f_inline void Matrix::RotationY(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);
    
    // Create Y-Rotation matrix
    Identity();
    El(0, 0) = c;
    El(2, 0) = -s;
    El(0, 2) = s;
    El(2, 2) = c;
}

f_inline void Matrix::RotationZ(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);
    
    // Create Z-Rotation matrix
    Identity();
    El(0, 0) = c;
    El(1, 0) = s;
    El(0, 1) = -s;
    El(1, 1) = c;
}

f_inline void Matrix::Translation(const Vector3f & inVector)
{
    // Create translation matrix
    Identity();
    El(0, 3) = inVector.x();
    El(1, 3) = inVector.y();
    El(2, 3) = inVector.z();
}

f_inline void Matrix::Scale(const Vector3f & inVector)
{
    // Create translation matrix
    Identity();
    El(0, 0) = inVector.x();
    El(1, 1) = inVector.y();
    El(2, 2) = inVector.z();
}

f_inline void Matrix::Set(float m00, float m01, float m02, float m03,
                          float m10, float m11, float m12, float m13,
                          float m20, float m21, float m22, float m23,
                          float m30, float m31, float m32, float m33)
{
    m[0] = m00;	m[4] = m01;	m[8] = m02;	m[12] = m03;
    m[1] = m10;	m[5] = m11;	m[9] = m12;	m[13] = m13;
    m[2] = m20;	m[6] = m21;	m[10]= m22;	m[14] = m23;
    m[3] = m30;	m[7] = m31;	m[11]= m32;	m[15] = m33;
}


// 'multiplies' the current matrix by a x-rotation matrix
f_inline const Matrix & Matrix::RotateX(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);

    // calculate the changed values
    float m01 = c*El(0,1) + s*El(0,2);
    float m11 = c*El(1,1) + s*El(1,2);
    float m21 = c*El(2,1) + s*El(2,2);
    float m31 = c*El(3,1) + s*El(3,2);
    float m02 =-s*El(0,1) + c*El(0,2);
    float m12 =-s*El(1,1) + c*El(1,2);
    float m22 =-s*El(2,1) + c*El(2,2);
    float m32 =-s*El(3,1) + c*El(3,2);

    // transfer them to the matrix
    El(0,1) = m01;
    El(1,1) = m11;
    El(2,1) = m21;
    El(3,1) = m31;
    El(0,2) = m02;
    El(1,2) = m12;
    El(2,2) = m22;
    El(3,2) = m32;

    return (*this);
}

// 'multiplies' the current matrix by a y-rotation matrix
f_inline const Matrix & Matrix::RotateY(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);

    // calculate the changed values
    float m00 = c*El(0,0) - s*El(0,2);
    float m10 = c*El(1,0) - s*El(1,2);
    float m20 = c*El(2,0) - s*El(2,2);
    float m30 = c*El(3,0) - s*El(3,2);
    float m02 = s*El(0,0) + c*El(0,2);
    float m12 = s*El(1,0) + c*El(1,2);
    float m22 = s*El(2,0) + c*El(2,2);
    float m32 = s*El(3,0) + c*El(3,2);

    // transfer them to the matrix
    El(0,0) = m00;
    El(1,0) = m10;
    El(2,0) = m20;
    El(3,0) = m30;
    El(0,2) = m02;
    El(1,2) = m12;
    El(2,2) = m22;
    El(3,2) = m32;

    return (*this);
}

// 'multiplies' the current matrix by a z-rotation matrix
f_inline const Matrix & Matrix::RotateZ(float inAngle)
{
    float c=gCos(inAngle), s=gSin(inAngle);

    // calculate the changed values
    float m00 = c*El(0,0) + s*El(0,1);
    float m10 = c*El(1,0) + s*El(1,1);
    float m20 = c*El(2,0) + s*El(2,1);
    float m30 = c*El(3,0) + s*El(3,1);
    float m01 =-s*El(0,0) + c*El(0,1);
    float m11 =-s*El(1,0) + c*El(1,1);
    float m21 =-s*El(2,0) + c*El(2,1);
    float m31 =-s*El(3,0) + c*El(3,1);

    // transfer them to the matrix
    El(0,0) = m00;
    El(1,0) = m10;
    El(2,0) = m20;
    El(3,0) = m30;
    El(0,1) = m01;
    El(1,1) = m11;
    El(2,1) = m21;
    El(3,1) = m31;

    return (*this);
}

// 'multiplies' the current matrix by a translation matrix (last row is interpreted as 0 0 0 1)
f_inline const Matrix & Matrix::Translate(const Vector3f & inVector)
{
    float x = inVector.x(), y = inVector.y(), z = inVector.z();

    // transfer them to the matrix
    El(0,3) = x*m[0] + y*m[4] + z*m[8] + m[12];
    El(1,3) = x*m[1] + y*m[5] + z*m[9] + m[13];
    El(2,3) = x*m[2] + y*m[6] + z*m[10]+ m[14];

    return (*this);
}

f_inline void Matrix::InvertRotationMatrix()
{
    // transpose the rotation part
    gSwap(El(0, 1), El(1, 0));
    gSwap(El(0, 2), El(2, 0));
    gSwap(El(1, 2), El(2, 1));

    // Set new position
    Pos() = -Rotate(Pos());
}

// full matrix application (last row is interpreted as 0 0 0 1)
f_inline Vector3f Matrix::Transform(const Vector3f & inVector)
{
    float x = inVector.x(), y = inVector.y(), z = inVector.z();

    return Vector3f(	x*m[0] + y*m[4] + z*m[8] + m[12],
                        x*m[1] + y*m[5] + z*m[9] + m[13],
                        x*m[2] + y*m[6] + z*m[10]+ m[14]);
}

// this only applies the rotation part of the matrix
f_inline Vector3f Matrix::Rotate(const Vector3f & inVector)
{
    float x = inVector.x(), y = inVector.y(), z = inVector.z();
    return Vector3f(	x*m[0] + y*m[4] + z*m[8],
                        x*m[1] + y*m[5] + z*m[9],
                        x*m[2] + y*m[6] + z*m[10]);
}

// this applies the transpose of the rotation part of the matrix (inverse rotation)
f_inline Vector3f Matrix::InverseRotate(const Vector3f & inVector)
{
    float x = inVector.x(), y = inVector.y(), z = inVector.z();
    return Vector3f(x*m[0] + y*m[1] + z*m[2],
                    x*m[4] + y*m[5] + z*m[6],
                    x*m[8] + y*m[9] + z*m[10]);
}

// Multiply this Matrix with another Matrix
f_inline const Matrix Matrix::operator*(const Matrix & inRHS) const
{
    Matrix r;

    for (int i=0; i<4; i++)
        for (int j=0; j<4; j++) 
            r(i,j) = El(i,0)*inRHS(0,j)+El(i,1)*inRHS(1,j)+El(i,2)*inRHS(2,j)+El(i,3)*inRHS(3,j);

    return r;
}

// Multiply a Vector3f with this Matrix
f_inline const Vector3f Matrix::operator*(const Vector3f &inRHS) const
{
    float x = inRHS.x(), y = inRHS.y(), z = inRHS.z();

    return Vector3f(x*m[0] + y*m[4] + z*m[8] + m[12],
                    x*m[1] + y*m[5] + z*m[9] + m[13],
                    x*m[2] + y*m[6] + z*m[10]+ m[14]);
}

// Multiply a Vector2f with this Matrix	
f_inline const Vector3f Matrix::operator*(const Vector2f &inRHS) const
{
    float x = inRHS.x(), y = inRHS.y();

    return Vector3f(x*m[0] + y*m[4] + m[12],
                    x*m[1] + y*m[5] + m[13],
                    x*m[2] + y*m[6] + m[14]);
}

f_inline const Matrix & Matrix::operator*=(const Matrix &inRHS)
{
    *this = *this * inRHS;
    return *this;
}

} //namespace salt

#endif // __MATRIX_H__
