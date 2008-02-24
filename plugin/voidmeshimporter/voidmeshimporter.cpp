/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: voidmeshimporter.cpp,v 1.2.8.1 2008/02/24 14:31:15 sgvandijk Exp $

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
#include "voidmeshimporter.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <kerosin/sceneserver/helper/NVMeshMender.h>

using namespace zeitgeist;
using namespace oxygen;
using namespace std;
using namespace boost;

VoidMeshImporter::VoidMeshImporter()
    : MeshImporter()
{
}

VoidMeshImporter::~VoidMeshImporter()
{
}

shared_ptr<TriMesh> VoidMeshImporter::ImportMesh
(const string& name, const ParameterList& parameter)
{
    // open file
    shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>
        (GetCore()->Get("/sys/server/file"));

    if (fileServer.get() == 0)
        {
            GetLog()->Error()
                << "(VoidMeshImporter) ERROR: cannot find FileServer\n";

            return shared_ptr<TriMesh>();
        }

    shared_ptr<salt::RFile> file = fileServer->Open(name);

    if (file.get() == 0)
        {
            GetLog()->Error()
                << "(VoidMeshImporter) ERROR: cannot open file '" << name << "'\n";
            return shared_ptr<TriMesh>();
        }

    NVMeshMender meshmender;
    NVMeshMender::VAVector input, output;
    NVMeshMender::VertexAttribute  att;

    char buffer[1024];
    int temp;
    int i;

    // minimal check whether we're importing a void file
    string tempBuffer(buffer);
    if (tempBuffer.find("TotalTriCount") == string::npos)
        {
            GetLog()->Normal() << "(VoidMeshImporter) not a .void mesh, aborting.\n";

            return shared_ptr<TriMesh>();
        }

    GetLog()->Normal() << "(VoidMeshImporter) Loading " << name << "\n";
    file->Gets(buffer, 1024);
    sscanf(buffer, "TotalTriCount: %d", &temp);
    GetLog()->Normal() << "(VoidMeshImporter) TotalTriCount: "
                       << temp << "\n";

    int vertexCount;
    file->Gets(buffer, 1024);
    sscanf(buffer, "VertexCount:   %d", &vertexCount);
    GetLog()->Normal() << "(VoidMeshImporter) VertexCount:   "
                       << vertexCount << "\n";

    int meshCount;
    file->Gets(buffer, 1024);
    sscanf(buffer, "MeshCount:     %d", &meshCount);
    GetLog()->Normal() << "(VoidMeshImporter) MeshCount:     "
                       << meshCount << "\n";
    file->Gets(buffer, 1024);

    // load position and texture coordinates of this object
    NVMeshMender::VertexAttribute position;
    position.Name_ = "position";

    NVMeshMender::VertexAttribute tex0;
    tex0.Name_ = "tex0";

    for (int v = 0; v < vertexCount; ++v)
        {
            float x, y, z;

            file->Gets(buffer, 1024);
            sscanf(buffer, "%f %f %f", &x, &y, &z);
            position.floatVector_.push_back(x);
            position.floatVector_.push_back(y);
            position.floatVector_.push_back(z);

            file->Gets(buffer, 1024);
            sscanf(buffer, "%f %f", &x, &y);
            tex0.floatVector_.push_back(x);
            tex0.floatVector_.push_back(y);
            tex0.floatVector_.push_back(1);
        }

    input.push_back(position);
    input.push_back(tex0);

    file->Gets(buffer, 1024);

    // for every mesh in the object
    NVMeshMender::VertexAttribute accumulatedIndices;
    vector<NVMeshMender::VertexAttribute> indices;
    indices.resize(meshCount);

    NVMeshMender::VertexAttribute   material;
    material.Name_ = "material";
    material.intVector_.resize(position.floatVector_.size());

    accumulatedIndices.Name_ = "indices";
    for (int m = 0; m < meshCount; ++m)
        {
            int matId = 0;
            // skip header
            file->Gets(buffer, 1024);
            // material
            file->Gets(buffer, 1024);
            sscanf(buffer, "  Material:    %d", &matId);

            // face count
            int faceCount;
            file->Gets(buffer, 1024);
            sscanf(buffer, "  FaceCount:   %d", &faceCount);
            //printf("FaceCount: %d\n", faceCount);
            // skip
            file->Gets(buffer, 1024);

            indices[m].Name_ = "indices";
            for(int f = 0; f < faceCount; ++f)
                {
                    int a, b, c;
                    file->Gets(buffer, 1024);
                    sscanf(buffer, "  %d %d %d", &a, &b, &c);
                    indices[m].intVector_.push_back(a);
                    material.intVector_[3*a] = matId;
                    accumulatedIndices.intVector_.push_back(a);
                    indices[m].intVector_.push_back(b);
                    material.intVector_[3*b] = matId;
                    accumulatedIndices.intVector_.push_back(b);
                    indices[m].intVector_.push_back(c);
                    material.intVector_[3*c] = matId;
                    accumulatedIndices.intVector_.push_back(c);
                }
            file->Gets(buffer, 1024);
        }

    input.push_back(accumulatedIndices);
    input.push_back(material);

    // Read Materials
    vector<string> materials;
    while (! file->Eof())
        {
            if (file->Gets(buffer, 1024)>0 && buffer[0] == 'M')
                {
                    char matName[256];
                    int dummy;
                    sscanf(buffer, "Material %d: %s", &dummy, matName);
                    GetLog()->Normal() << "(VoidMeshImporter) Material '"
                                       << matName << "'\n";
                    materials.push_back(matName);
                }
        }

    file->Close();

    // output
    att.Name_ = "position"; output.push_back(att);
    att.Name_ = "tex0";             output.push_back(att);
    att.Name_ = "normal";   output.push_back(att);
    att.Name_ = "indices";  output.push_back(att);
    att.Name_ = "material"; output.push_back(att);

    // munge
    shared_ptr<TriMesh> triMesh(new TriMesh());

    meshmender.Munge(input, output, 3.141592654f/3.0f, 0,
                     NVMeshMender::FixTangents, NVMeshMender::DontFixCylindricalTexGen,
                     NVMeshMender::WeightNormalsByFaceSize);

    NVMeshMender::VertexAttribute   outIndices;
    NVMeshMender::VertexAttribute   outMaterial;

    for (i=0; i < (int)output.size(); ++i)
        {
            string b = output[i].Name_;

            if (output[i].Name_ == "position")
                {
                    vertexCount = output[i].floatVector_.size()/3;
                    shared_array<float> pos(new float[vertexCount * 3]);
                    for (int j = 0; j < (vertexCount*3); ++j)
                        {
                            pos[j] = output[i].floatVector_[j];
                        }

                    triMesh->SetPos(pos,vertexCount);
                }

            if (output[i].Name_ == "tex0")
                {
                    int size = output[i].floatVector_.size()/3;
                    shared_array<float> texCoords(new float[size * 3]);

                    for (int j = 0; j < (size*3); ++j)
                        {
                            texCoords[j] = output[i].floatVector_[j];
                        }

                    triMesh->SetTexCoords(texCoords);
                }

            if (output[i].Name_ == "normal")
                {
                    int size = output[i].floatVector_.size()/3;
                    shared_array<float> normal(new float[size * 3]);

                    for (int j = 0; j < (size * 3); ++j)
                        {
                            normal[j] = output[i].floatVector_[j];
                        }

                    triMesh->SetNormals(normal);
                }

            if (output[i].Name_ == "indices")
                {
                    outIndices = output[i];
                }

            if (output[i].Name_ == "material")
                {
                    outMaterial = output[i];
                }
        }

    std::vector<int> faceCount;
    faceCount.resize(meshCount);

    for (i = 0; i<meshCount; ++i)
        {
            faceCount[i] = 0;
        }

    int totalFaceCount = outIndices.intVector_.size()/3;
    for (i = 0; i < totalFaceCount; ++i)
        {
            faceCount[outMaterial.intVector_[outIndices.intVector_[3*i]*3]]++;
        }

    for (i = 0; i<meshCount; ++i)
        {
            shared_ptr<IndexBuffer> indeces(new IndexBuffer());
            indeces->EnsureFit(faceCount[i] * 3);
            unsigned int* faces = indeces->GetIndex().get();

            for (int f = 0; f < totalFaceCount; ++f)
                {
                    if (outMaterial.intVector_[outIndices.intVector_[3*f]*3]==i)
                        {
                            // add face
                            int triBase = 3*f;
                            indeces->Cache(outIndices.intVector_[triBase]);
                            indeces->Cache(outIndices.intVector_[triBase+1]);
                            indeces->Cache(outIndices.intVector_[triBase+2]);
                        }
                }

            triMesh->AddFace(indeces, materials[i]);
        }

    return triMesh;
}
