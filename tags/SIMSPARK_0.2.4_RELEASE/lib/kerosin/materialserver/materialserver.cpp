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
#include "materialserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include "material.h"
#include "materialsolid.h"
#include "materialexporter.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

MaterialServer::MaterialServer() : Node()
{
}

MaterialServer::~MaterialServer()
{
}

void MaterialServer::RegisterMaterial(boost::shared_ptr<Material> material)
{
    if (material.get() == 0)
    {
        return;
    }

    // remove any previous material with the same name
    boost::shared_ptr<Material> previous =
        dynamic_pointer_cast<Material>(GetChild(material->GetName()));

    if (previous.get() != 0)
    {
        GetLog()->Debug() << "(MaterialServer) removing material "
                          << material->GetName() << "\n";
        RemoveChildReference(previous);
    }

    // register new material
    AddChildReference(material);

    GetLog()->Debug() << "(MaterialServer) registered material "
                      << material->GetName() << "\n";

}

boost::shared_ptr<Material> MaterialServer::GetMaterial(const std::string& name)
{
    boost::shared_ptr<Material> material =
        dynamic_pointer_cast<Material>(GetChild(name));

    if (material.get() == 0)
    {
        GetLog()->Error() << "(MaterialServer) ERROR: Unknown material '"
                          << name << "'\n";
    }

    return material;
}

void
MaterialServer::ResetMaterials()
{
    // drop all previous materials
    UnlinkChildren();

    // (re)create the default material
    boost::shared_ptr<MaterialSolid> defMat = dynamic_pointer_cast<MaterialSolid>
        (GetCore()->New("kerosin/MaterialSolid"));

    defMat->SetName("default");
    AddChildReference(defMat);
}

void
MaterialServer::OnLink()
{
    Node::OnLink();
    ResetMaterials();
}

bool
MaterialServer::InitMaterialExporter(const std::string& name)
{
    boost::shared_ptr<MaterialExporter> exporter
        = dynamic_pointer_cast<MaterialExporter>(GetCore()->New(name));

    if (exporter.get() == 0)
    {
        GetLog()->Error() << "(MaterialServer) ERROR: "
                          << "unable to create MaterialExporter '" << name << "'\n";
        return false;
    }

    exporter->SetName(name);
    AddChildReference(exporter);

    GetLog()->Normal() << "(MaterialServer) MaterialExporter '" << name << "' registered\n";

    return true;
}

void
MaterialServer::ExportAllMaterial()
{
    GetLog()->Debug() << "(MaterialServer) ExportAllMaterial\n";
    TLeafList materials;
    ListChildrenSupportingClass<Material>(materials);

    for (TLeafList::const_iterator mi = materials.begin(); mi != materials.end(); ++mi)
    {
        boost::shared_ptr<Material> m = static_pointer_cast<Material>(*mi);
        ExportMaterial(m);
    }
    GetLog()->Debug() << "(MaterialServer) ExportAllMaterial done\n";
}

void
MaterialServer::ExportMaterial(boost::shared_ptr<Material> material)
{
    TLeafList exporters;
    ListChildrenSupportingClass<MaterialExporter>(exporters);

    for (TLeafList::const_iterator bi = exporters.begin(); bi != exporters.end(); ++bi)
    {
        boost::shared_ptr<MaterialExporter> mb = static_pointer_cast<MaterialExporter>(*bi);
        mb->RegisterMaterial(material);
    }
}
