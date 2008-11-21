/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: xmlfunctions.h,v 1.1 2008/02/22 16:48:18 hedayat Exp $

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
#ifndef XMLFUNCTIONS_H
#define XMLFUNCTIONS_H

#include <string>

class TiXmlElement;
class TiXmlNode;

extern void SetXMLAttribute(TiXmlElement& element, const std::string& name, const std::string& value);
extern void SetXMLAttribute(TiXmlElement& element, const std::string& name, int value);
extern void SetXMLDoubleAttribute(TiXmlElement& element, const std::string& name, double value);

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const std::string& attrName, std::string& str);

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const std::string& attrName, int& i);

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const std::string& attrName, double& d);

extern bool GetXMLAttribute(const TiXmlElement* element,
                            const std::string& attrName, float& f);

extern std::string GetXMLValue(const TiXmlNode* node);

extern TiXmlElement* GetFirstChild(TiXmlNode* node, const std::string& type);
extern TiXmlElement* IterateChildren(TiXmlNode* node, const std::string& type);

extern std::string GetXMLPath(TiXmlNode* node);

#endif // XMLFUNCTIONS_H
