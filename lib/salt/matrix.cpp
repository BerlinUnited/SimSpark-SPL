/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: matrix.cpp,v 1.4 2003/09/08 08:58:53 rollmark Exp $

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
#include "matrix.h"

#include <cstdio>

using namespace salt;

float Matrix::mIdentity[16]=
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

void Matrix::Dump() const
{
	printf("%f %f %f %f\n", m[0], m[4], m[8], m[12]);
	printf("%f %f %f %f\n", m[1], m[5], m[9], m[13]);
	printf("%f %f %f %f\n", m[2], m[6], m[10], m[14]);
	printf("%f %f %f %f\n", m[3], m[7], m[11], m[15]);
}


void Matrix::LookAt(const Vector3f & inEye, const Vector3f & inDirection, const Vector3f & inUp)
{
	Vector3f forward	= inDirection.Normalized();
	Vector3f up			= inUp.Normalized();
	Vector3f right		= forward.Cross(up);
	right.Normalize();

	up = right.Cross(forward);

	// Make inverse rotation matrix using right, forward, up vectors
	Set(	   right.x(),  right.y(),	  right.z(),	0.0f,
				  up.x(),	 up.y(),		 up.z(),	0.0f,
			 forward.x(), forward.y(),  forward.z(),	0.0f,
				  0.0f,		  0.0f,		  0.0f, 1.0f);
	
	Pos() = Rotate(-inEye);
}

/*!	Calculate an attenuation matrix.
	pos = world space position of light
	radius = radius of light (used for distance attenuation)
*/
void Matrix::CalcAttenuationNoRotation(const Vector3f &pos, float radius)
{
	Matrix tmp1, tmp2;
	
	// translate to 'light space' ... no rotation needed
	tmp1.Translation(-pos);
	
	// create proper scaling matrix
	float invrad = 0.5f / radius;

	tmp2.Identity();
	tmp2(0,0) = invrad;
	tmp2(1,1) = invrad;
	tmp2(2,2) = invrad;
	
	Identity ();
	Translation (Vector3f(0.5f, 0.5f, 0.5f));

	*this *= tmp2 * tmp1;
}

void Matrix::CalcAttenuationWithRotation(const Matrix &lightWorldMatrix, float radius)
{
	Matrix tmp1, tmp2;
	
	// translate to 'light space' ... this time with rotation
	tmp1 = lightWorldMatrix;
	tmp1.InvertRotationMatrix();
	
	// create proper scaling matrix
	float invrad = 0.5f / radius;

	tmp2.Identity();
	tmp2(0,0) = invrad;
	tmp2(1,1) = invrad;
	tmp2(2,2) = invrad;
	
	Identity ();
	Translation (Vector3f(0.5f, 0.5f, 0.5f));

	*this *= tmp2 * tmp1;
}

void Matrix::CalcInfiniteProjection(float width, float height, float fov, float zNear)
{
	const float halfWidth	= zNear * (float)tan(gDegToRad(fov*0.5f));
	const float halfHeight	= halfWidth * (height/width);

	CalcInfiniteFrustum(-halfWidth, halfWidth, -halfHeight, halfHeight, zNear);
}

void Matrix::CalcInfiniteFrustum(float left, float right, float bottom, float top, float zNear)
{
	Identity();
	
	El(0,0) = (2*zNear) / (right - left);
	El(0,2) = (right + left) / (right - left);
	
	El(1,1) = (2*zNear) / (top - bottom);
	El(1,2) = (top + bottom) / (top - bottom);
	
    // nudge infinity in just slightly for lsb slop
    const float nudge = 1 - 1.0 / (1<<23);

	El(2,2) = -1  * nudge;
	El(2,3) = -2*zNear * nudge;
	
	El(3,2) = -1;
	El(3,3) = 0;
}

void Matrix::CalcSpotLight(const Matrix &lightWorldTransform, float fov, float width, float height, float zNear)
{
	Matrix scale, proj, space;
	Vector3f halfVector(0.5f, 0.5f, 0.5f);

	Identity ();
	Translation (halfVector);

	scale.Identity ();
	scale.Scale (halfVector);

	// create projection matrix
	proj.CalcInfiniteProjection(width, height, fov, zNear);

	space = lightWorldTransform;
	space.InvertRotationMatrix();

	// so, we transform first into light space, then project, then scale and
	// translate (this)
	*this *= scale * proj * space;
}

bool Matrix::IsEqual(const Matrix& matrix) const
{
	for (int i=0; i<16; ++i)
		if (matrix.m[i] != m[i]) return false;

	return true;
}
