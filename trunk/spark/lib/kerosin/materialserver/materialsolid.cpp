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
#include "materialsolid.h"
#include <kerosin/openglserver/openglwrapper.h>
#include <kerosin/openglserver/openglserver.h>

using namespace kerosin;
using namespace zeitgeist;
using namespace salt;
using namespace boost;
using namespace std;

MaterialSolid::MaterialSolid()
    : Material(),
      mAmbient(0.2f,0.2f,0.2f,1.0f),
      mDiffuse(1.0f,1.0f,1.0f,1.0f),
      mSpecular(0.0f,0.0f,0.0f,1.0f),
      mEmission(0.0f,0.0f,0.0f,1.0f),
      mDepthCheck(true),
      mShininess(128.0f)
{
}

MaterialSolid::~MaterialSolid()
{
}

void MaterialSolid::SetupMaterial()
{
    // set ambient material reflectance
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mAmbient);

    // set diffuse material reflectance
    glColor3fv(mDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mDiffuse);

    // set specular material reflectance
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mSpecular);

    // set light emission
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mEmission);
}

void MaterialSolid::Bind()
{
    SetupMaterial();
    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void MaterialSolid::SetAmbient(const RGBA& ambient)
{
    mAmbient = ambient;
}

const RGBA& MaterialSolid::GetAmbient() const
{
    return mAmbient;
}

void MaterialSolid::SetDiffuse(const RGBA& diffuse)
{
    mDiffuse = diffuse;
}

const RGBA& MaterialSolid::GetDiffuse() const
{
    return mDiffuse;
}

void MaterialSolid::SetSpecular(const RGBA& specular)
{
    mSpecular = specular;
}

const RGBA& MaterialSolid::GetSpecular() const
{
    return mSpecular;
}

void MaterialSolid::SetEmission(const RGBA& emission)
{
    mEmission = emission;
}

const RGBA&
MaterialSolid::GetEmission() const
{
    return mEmission;
}


bool
MaterialSolid::DepthCheck() const
{
    return mDepthCheck;
}

void
MaterialSolid::SetDepthCheck(bool depthCheck)
{
    mDepthCheck = depthCheck;
}

void
MaterialSolid::SetShininess(float val)
{
    mShininess = val;
}

float
MaterialSolid::GetShininess() const
{
    return mShininess;
}
