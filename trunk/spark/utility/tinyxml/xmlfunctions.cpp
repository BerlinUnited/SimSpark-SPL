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
#include "xmlfunctions.h"
#include "tinyxml.h"

using namespace std;

extern void SetXMLAttribute(TiXmlElement& element, const string& name, const string& value)
{
    element.SetAttribute(name.c_str(), value.c_str());
}

extern void SetXMLAttribute(TiXmlElement& element, const string& name, int value)
{
    element.SetAttribute(name.c_str(), value);
}

extern void SetXMLDoubleAttribute(TiXmlElement& element, const string& name, double value)
{
    element.SetDoubleAttribute(name.c_str(),value);
}

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const string& attrName, string& str)
{
    const char* attr = element->Attribute(attrName.c_str());
    if (attr == 0)
        {
            return false;
        }

    str = attr;
    return true;
}

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const string& attrName, int& i)
{
    const char* attr = element->Attribute(attrName.c_str(),&i);
    if (attr == 0)
        {
            return false;
        }

    return true;
}

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const std::string& attrName, float& f)
{
    double d;
    bool ok = GetXMLAttribute(element, attrName, d);
    f = static_cast<float>(d);
    return ok;
}


extern bool GetXMLAttribute(const TiXmlElement* element,
                            const string& attrName, double& d)
{
    const char* attr = element->Attribute(attrName.c_str(),&d);
    if (attr == 0)
        {
            return false;
        }

    return true;
}

extern string GetXMLValue(const TiXmlNode* node)
{
    if (node == 0)
        {
            return "";
        }

    return node->Value();
}

extern TiXmlElement* GetFirstChild(TiXmlNode* node, const string& type)
{
    TiXmlNode* childNode
        = node->FirstChild(type.c_str());

    if (
        (childNode == 0) ||
        (childNode->Type() != TiXmlNode::ELEMENT)
        )
        {
            return 0;
        }

    return static_cast<TiXmlElement*>(childNode);
}

extern TiXmlElement* IterateChildren(TiXmlNode* node, const string& type)
{
    TiXmlNode* childNode
        = node->Parent()->IterateChildren(type.c_str(),node);

    if (
        (childNode == 0) ||
        (childNode->Type() != TiXmlNode::ELEMENT)
        )
        {
            return 0;
        }

    return static_cast<TiXmlElement*>(childNode);
}

extern string GetXMLPath(TiXmlNode* node)
{
    if (node == 0)
        {
            return "";
        }

    return GetXMLPath(node->Parent()) + "/" + node->Value();
}
