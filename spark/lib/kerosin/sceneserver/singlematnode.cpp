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
#include "singlematnode.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/materialserver/materialserver.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

SingleMatNode::SingleMatNode() : StaticMesh ()
{
}

SingleMatNode::~SingleMatNode()
{
}

bool SingleMatNode::SetMaterial(const std::string& name)
{
    mMaterials.clear();

    boost::shared_ptr<MaterialServer> materialServer = shared_dynamic_cast<MaterialServer>
        (GetCore()->Get("/sys/server/material"));

    if (materialServer.get() == 0)
        {
            GetLog()->Error()
              << "(SingleMatNode) ERROR: Cannot find MaterialServer\n";
            return false;
        }

    boost::shared_ptr<Material> material = materialServer->GetMaterial(name);

    if (material.get() == 0)
        {
            GetLog()->Error()
                << "(SingleMatNode) ERROR: Cannot find Material "<<name<<"\n";
            return false;
        }

    mMaterials.push_back(material);

    return true;
}

boost::shared_ptr<Material> SingleMatNode::GetMaterial()
{
    if (mMaterials.empty())
        {
            return boost::shared_ptr<Material>();
        }

    return mMaterials.front();
}

