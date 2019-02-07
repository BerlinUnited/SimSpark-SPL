/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri Oct 20 2006
   Copyright (C) 2006 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef KEROSIN_CYLINDER_H
#define KEROSIN_CYLINDER_H

#include <kerosin/kerosin_defines.h>
#include "singlematnode.h"

namespace kerosin
{
class Material;

/** Cylinder is a SingleMatNode that creates and renders a flat
    cylinder mesh with the given length and radius
 */
class KEROSIN_API Cylinder : public SingleMatNode
{
    //
    // Function
    //
public:
    Cylinder();
    virtual ~Cylinder();

    /** sets the parameters of the cylinder.

       \param radius is the radius of the caps, and of the cylinder
       itself

       \param length is the height of the cylinder, not counting the
       caps
    */
    void SetParams(float radius, float length);

    /** returns the parameters of the cylinder */
    void GetParams(float& radius, float& length) const;

    /** returns the radius of the cylinder */
    float GetRadius();

    /** returns the length of the cylinder */
    float GetLength();

    //
    // Members
    //
protected:
    /** the radius of the cylinder */
    float mRadius;

    /** the height of the clinder */
    float mLength;
};

DECLARE_CLASS(Cylinder);

} //namespace kerosin

#endif //KEROSIN_CYLINDER_H
