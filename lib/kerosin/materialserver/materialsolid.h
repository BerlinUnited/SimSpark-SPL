/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialsolid.h,v 1.2 2004/04/12 13:25:57 rollmark Exp $

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
#ifndef KEROSIN_MATERIALSOLID_H
#define KEROSIN_MATERIALSOLID_H

#include "material.h"
#include <kerosin/openglserver/glbase.h>
#include <salt/vector.h>

namespace kerosin
{
class MaterialSolid : public Material
{
    //
    // Functions
    //
public:
    MaterialSolid();
    virtual ~MaterialSolid();

    virtual void Bind();

    /** sets the ambient material reflectance */
    void SetAmbient(const RGBA& ambient);

    /** returns the ambient material reflectance */
    const RGBA& GetAmbient();

    /** sets the diffuse material reflectance */
    void SetDiffuse(const RGBA& diffuse);

    /** returns the diffuse material reflectancee */
    const RGBA& GetDiffuse();

    /** sets the specular material reflectance */
    void SetSpecular(const RGBA& specular);

    /** returns the specular material reflectance */
    const RGBA& GetSpecular();

    /** sets the light emission */
    void SetEmission(const RGBA& emission);

    /** returns the light emission */
    const RGBA& GetEmission();

    //
    // Members
    //
protected:
    /** the ambient material refeflectance */
    RGBA mAmbient;

    /** the diffuse material reflectance */
    RGBA mDiffuse;

    /** the specular material reflectance */
    RGBA mSpecular;

    /** the emitted light intensity of the material */
    RGBA mEmission;
};

DECLARE_CLASS(MaterialSolid);
};

#endif // KEROSIN_MATERIALSOLID_H
