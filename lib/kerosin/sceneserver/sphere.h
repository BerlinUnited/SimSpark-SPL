/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sphere.h,v 1.3 2004/03/20 12:57:56 rollmark Exp $

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
#ifndef KEROSIN_SPHERE_H
#define KEROSIN_SPHERE_H

#include <oxygen/sceneserver/basenode.h>

namespace kerosin
{
class Material;

class Sphere : public oxygen::BaseNode
{
    //
    // Function
    //
public:
    Sphere();

    // set/get properties
    float GetRadius() const;
    void SetRadius(float radius);

    void SetMaterial(const std::string& name);

private:
    virtual void RenderInternal();

    //
    // Members
    //
private:
    float   mRadius;
    boost::shared_ptr<Material> mMaterial;
};

DECLARE_CLASS(Sphere);

} //namespace kerosin

#endif //KEROSIN_SPHERE_H
