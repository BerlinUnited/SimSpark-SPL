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
#ifndef ROSELEMENTS_H
#define ROSELEMENTS_H

#include <map>
#include <string>

//! RoSim XML Attributes

// common
#define RA_NAME "name"

// AppearanceDefinition
#define RA_COLOR "Color"
#define RA_R "r"
#define RA_G "g"
#define RA_B "b"
#define RA_A "a"

// Vector
#define RA_X "x"
#define RA_Y "y"
#define RA_Z "z"

// Box
#define RA_LENGTH "length"
#define RA_WIDTH "width"
#define RA_HEIGHT "height"

// Sphere
#define RA_RADIUS "radius"

// Appearance
#define RA_REF "ref"

// GlobalPhysicalParameters
#define RA_GRAVITY "gravity"
#define RA_ERP "erp"
#define RA_CFM "cfm"

// PhysicalAttributes
#define RA_VALUE "value"
#define RA_CANCOLLIDE "canCollide"

// Use
#define RA_MACRONAME "macroName"
#define RA_INSTANCENAME "instanceName"

// ComplexShape
#define RA_VERTEXLIST "vertexList"

// Axis Deflection
#define RA_MIN "min"
#define RA_MAX "max"

class RosElements
{
public:
    enum ERosElement
        {
            RE_INVALID = 0,

            RE_ROSIINCLUDEFILE,
            RE_SIMULATION,
            RE_SCENE,
            RE_ELEMENTS,
            RE_MOVABLE,

            RE_TRANSLATION,
            RE_ROTATION,

            RE_BOX,
            RE_SPHERE,
            RE_CYLINDER,
            RE_CAPPEDCYLINDER,

            RE_COMPOUND,

            RE_COMPLEXSHAPE,
            RE_VERTEXLIST,
            RE_VERTEX,
            RE_GRAPHICALREPRESENTATION,
            RE_POLYGON,
            RE_TRIANGLESTRIP,
            RE_PHYSICALREPRESENTATION,
            RE_SIMPLEBOX,
            RE_SIMPLESPHERE,
            RE_SIMPLECYLINDER,
            RE_SIMPLECAPPEDCYLINDER,

            RE_MACRO,
            RE_USE,

            RE_HINGE,
            RE_UNIVERSAL,
            RE_SLIDER,
            RE_ANCHORPOINT,
            RE_AXIS,
            RE_AXIS1,
            RE_AXIS2,
            RE_DEFLECTION,

            RE_GLOBALPHYSICALPARAMETERS,
            RE_PHYSICALATTRIBUTES,
            RE_MASS,
            RE_CENTEROFMASS,

            RE_APPEARANCEDEFINITION,
            RE_APPEARANCE,
            RE_DEFAULTAPPEARANCE,
            RE_AMBIENTLIGHTCOLOR,
            RE_COLOR,

            RE_INTERACTIVEBUTTON
        };

    typedef std::map<std::string, ERosElement> TElementMap;
    typedef std::map<ERosElement, std::string> TReverseMap;

public:
    static RosElements& GetInstance();
    ~RosElements();

    ERosElement Lookup(const std::string& value) const;
    std::string Lookup(ERosElement element) const;

protected:
    void SetupMap();

private:
    RosElements();

protected:
    TElementMap mMap;
    TReverseMap mReverseMap;
};

#endif // ROSELEMENTS_H

