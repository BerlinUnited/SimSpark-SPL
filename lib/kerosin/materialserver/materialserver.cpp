/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialserver.cpp,v 1.7 2007/06/20 00:25:22 fruit Exp $

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
#include "materialserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include "material.h"
#include "materialsolid.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

MaterialServer::MaterialServer() : Node()
{
}

MaterialServer::~MaterialServer()
{
}

shared_ptr<Material> MaterialServer::GetMaterial(const std::string& name)
{
    shared_ptr<Material> material =
        shared_dynamic_cast<Material>(GetChild(name));

    if (material.get() == 0)
    {
        GetLog()->Error() << "(MaterialServer) ERROR: Unknown material '"
                          << name << "'\n";
    }

    return material;
}

void 
MaterialServer::OnLink()
{
    // create the default material
    shared_ptr<MaterialSolid> defMat = shared_dynamic_cast<MaterialSolid>
        (GetCore()->New("kerosin/MaterialSolid"));
    defMat->SetName("default");
    AddChildReference(defMat);
}
