/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: geometryserver.cpp 246 2011-02-18 18:44:14Z sgvandijk $

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
#include "geometryserver.h"
#include "meshimporter.h"
#include "meshexporter.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

GeometryServer::GeometryServer() : Node()
{
}

GeometryServer::~GeometryServer()
{
}

void
GeometryServer::OnLink()
{
    if (mChildren.size() == 0)
    {
        InitMeshImporter("oxygen/StdMeshImporter");
    }
}

bool
GeometryServer::InitMeshImporter(const string& importerName)
{
    boost::shared_ptr<MeshImporter> importer
        = shared_dynamic_cast<MeshImporter>(GetCore()->New(importerName));

    if (importer.get() == 0)
    {
        GetLog()->Error() << "(GeometryServer) ERROR: "
                          << "unable to create '" << importerName << "'\n";
        return false;
    }

    importer->SetName(importerName);
    AddChildReference(importer);

    GetLog()->Debug()
        << "(GeometryServer) MeshImporter '" << importerName << "' registered\n";

    return true;
}

boost::shared_ptr<TriMesh>
GeometryServer::GetMesh(const string& name, const::ParameterList& parameter)
{
    // try a direct match
    string meshName = name;
    TMeshMap::const_iterator meshIter = mMeshMap.find(meshName);

    if (meshIter != mMeshMap.end())
    {
        return (*meshIter).second;
    }

    TLeafList importer;
    ListChildrenSupportingClass<MeshImporter>(importer);

    if (importer.size() == 0)
    {
        GetLog()->Error()
            << "(GeometryServer) Warning: no MeshImporter registered\n";

        return boost::shared_ptr<TriMesh>();
    }

    // try to mangle the name
    for (
         TLeafList::iterator iter = importer.begin();
         iter != importer.end();
         ++iter
         )
    {
        boost::shared_ptr<MeshImporter> importer =
            shared_static_cast<MeshImporter>(*iter);

        string str = importer->MangleName(name, parameter);

        if (str != meshName)
        {
            meshName = str;
            meshIter = mMeshMap.find(meshName);
            if (meshIter != mMeshMap.end())
            {
                return (*meshIter).second;
            }
        }
    }

    // try to import the mesh
    for (
         TLeafList::iterator iter = importer.begin();
         iter != importer.end();
         ++iter
         )
    {
        boost::shared_ptr<MeshImporter> importer =
            shared_static_cast<MeshImporter>(*iter);

        boost::shared_ptr<TriMesh> mesh = importer->ImportMesh(name,parameter);

        if (mesh.get() == 0)
        {
            continue;
        }

        string meshName = mesh->GetName();
        if (meshName.empty())
        {
            meshName = name;
            mesh->SetName(name);
        }

        GetLog()->Debug() << "(GeometryServer) imported mesh '" << meshName
                           << " with '" << importer->GetName() << "'\n";

        if (mesh.get() == 0 || mesh->GetVertexCount() == 0)
        {
            continue;
        }

        RegisterMesh(mesh);

        return mesh;
    }

    GetLog()->Error() << "(GeometryServer) ERROR: cannot import mesh '"
                      << name << "'\n";

    return boost::shared_ptr<TriMesh>();
}

void
GeometryServer::RegisterMesh(boost::shared_ptr<TriMesh> mesh)
{
    if (mesh.get() == 0)
    {
        GetLog()->Debug()
            << "(GeometryServer) RegisterMesh called with NULL mesh\n";
        return;
    }

    std::string name = mesh->GetName();
    if (name.empty())
    {
        GetLog()->Error()
            << "(GeometryServer) Cannot register a mesh without a name\n";
        return;
    }

    TMeshMap::const_iterator iter = mMeshMap.find(name);
    if (iter != mMeshMap.end())
    {
        GetLog()->Debug() << "(GeometryServer) replacing mesh " << name << "\n";
    }

    mMeshMap[name] = mesh;
    GetLog()->Debug() << "(GeometryServer) mesh " << name << " registered\n";

    TLeafList exporters;
    ListChildrenSupportingClass<MeshExporter>(exporters);

    for (TLeafList::const_iterator bi = exporters.begin(); bi != exporters.end(); ++bi)
    {
        GetLog()->Debug() << "(GeometryServer) additionally registered mesh "
                           << name << " via MeshExporter '" << (*bi)->GetName() << "'\n";
        boost::shared_ptr<MeshExporter> mb = shared_static_cast<MeshExporter>(*bi);
        mb->RegisterMesh(mesh);
    }
}

bool
GeometryServer::InitMeshExporter(const string& name)
{
    boost::shared_ptr<MeshExporter> exporter
        = shared_dynamic_cast<MeshExporter>(GetCore()->New(name));

    if (exporter.get() == 0)
    {
        GetLog()->Error() << "(GeometryServer) ERROR: "
                          << "unable to create MeshExporter '" << name << "'\n";
        return false;
    }

    exporter->SetName(name);
    AddChildReference(exporter);

    GetLog()->Debug() << "(GeometryServer) MeshExporter '" << name << "' registered\n";

    return true;
}
