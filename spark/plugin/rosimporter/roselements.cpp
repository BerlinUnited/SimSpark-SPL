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
#include "roselements.h"

using namespace std;

RosElements::RosElements()
{
    SetupMap();
}

RosElements::~RosElements()
{
}

RosElements& RosElements::GetInstance()
{
    static RosElements theInstance;
    return theInstance;
}

#define ROS_DEFINE_ELEMENT(_elem, _str)\
    mMap[_str] = _elem;\
    mReverseMap[_elem] = _str;


void RosElements::SetupMap()
{
    mMap.clear();

    ROS_DEFINE_ELEMENT(RE_ROSIINCLUDEFILE,"RoSiIncludeFile");
    ROS_DEFINE_ELEMENT(RE_SIMULATION,"Simulation");
    ROS_DEFINE_ELEMENT(RE_SCENE,"Scene");
    ROS_DEFINE_ELEMENT(RE_ELEMENTS,"Elements");
    ROS_DEFINE_ELEMENT(RE_MOVABLE,"Movable");
    ROS_DEFINE_ELEMENT(RE_COMPOUND,"Compound");

    ROS_DEFINE_ELEMENT(RE_TRANSLATION, "Translation");
    ROS_DEFINE_ELEMENT(RE_ROTATION, "Rotation");

    ROS_DEFINE_ELEMENT(RE_BOX,"Box");
    ROS_DEFINE_ELEMENT(RE_SPHERE,"Sphere");
    ROS_DEFINE_ELEMENT(RE_CYLINDER,"Cylinder");
    ROS_DEFINE_ELEMENT(RE_CAPSULE,"Capsule");

    ROS_DEFINE_ELEMENT(RE_COMPLEXSHAPE,"ComplexShape");
    ROS_DEFINE_ELEMENT(RE_VERTEXLIST,"VertexList");
    ROS_DEFINE_ELEMENT(RE_VERTEX,"Vertex");
    ROS_DEFINE_ELEMENT(RE_GRAPHICALREPRESENTATION, "GraphicalRepresentation");
    ROS_DEFINE_ELEMENT(RE_PHYSICALREPRESENTATION,"PhysicalRepresentation");
    ROS_DEFINE_ELEMENT(RE_POLYGON,"Polygon");
    ROS_DEFINE_ELEMENT(RE_TRIANGLESTRIP,"TriangleStrip");

    ROS_DEFINE_ELEMENT(RE_SIMPLEBOX,"SimpleBox");
    ROS_DEFINE_ELEMENT(RE_SIMPLESPHERE,"SimpleSphere");
    ROS_DEFINE_ELEMENT(RE_SIMPLECYLINDER,"SimpleCylinder");
    ROS_DEFINE_ELEMENT(RE_SIMPLECAPSULE,"SimpleCapsule");

    ROS_DEFINE_ELEMENT(RE_MACRO, "Macro");
    ROS_DEFINE_ELEMENT(RE_USE, "Use");

    ROS_DEFINE_ELEMENT(RE_HINGE,"Hinge");
    ROS_DEFINE_ELEMENT(RE_SLIDER,"Slider");
    ROS_DEFINE_ELEMENT(RE_UNIVERSAL,"UniversalJoint");
    ROS_DEFINE_ELEMENT(RE_ANCHORPOINT,"AnchorPoint");
    ROS_DEFINE_ELEMENT(RE_AXIS,"Axis");
    ROS_DEFINE_ELEMENT(RE_AXIS1,"Axis1");
    ROS_DEFINE_ELEMENT(RE_AXIS2,"Axis2");
    ROS_DEFINE_ELEMENT(RE_DEFLECTION,"Deflection");

    ROS_DEFINE_ELEMENT(RE_GLOBALPHYSICALPARAMETERS,"GlobalPhysicalParameters");
    ROS_DEFINE_ELEMENT(RE_PHYSICALATTRIBUTES,"PhysicalAttributes");
    ROS_DEFINE_ELEMENT(RE_MASS,"Mass");
    ROS_DEFINE_ELEMENT(RE_CENTEROFMASS, "CenterOfMass");

    ROS_DEFINE_ELEMENT(RE_APPEARANCEDEFINITION, "AppearanceDefinition");
    ROS_DEFINE_ELEMENT(RE_APPEARANCE, "Appearance");
    ROS_DEFINE_ELEMENT(RE_DEFAULTAPPEARANCE, "DefaultAppearance");
    ROS_DEFINE_ELEMENT(RE_AMBIENTLIGHTCOLOR,"AmbientLightColor");

    ROS_DEFINE_ELEMENT(RE_COLOR, "Color");
    ROS_DEFINE_ELEMENT(RE_INTERACTIVEBUTTON,"InteractiveButton");
}

string RosElements::Lookup(ERosElement element) const
{
    TReverseMap::const_iterator iter = mReverseMap.find(element);
    if (iter == mReverseMap.end())
        {
            return "";
        }

    return (*iter).second;
}

RosElements::ERosElement RosElements::Lookup(const std::string& value) const
{
    TElementMap::const_iterator iter = mMap.find(value);

    if (iter == mMap.end())
        {
            return RE_INVALID;
        }

    return (*iter).second;
}

