/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh.cpp,v 1.8 2004/04/18 16:26:26 rollmark Exp $

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
#include "staticmesh.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/materialserver/material.h>
#include <kerosin/materialserver/materialserver.h>
#include <salt/fileclasses.h>
#include "helper/NVMeshMender.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;
using namespace oxygen;
// using namespace Opcode;

StaticMesh::StaticMesh()
{
    mVertexCount = 0;
}

StaticMesh::~StaticMesh()
{
}

void StaticMesh::ComputeBoundingBox()
{
}

void StaticMesh::CalcBoundingBox()
{
    mLocalBoundingBox.Init();

    const int n = mVertexCount * 3;

    float* arPos = mPos.get();
    if (arPos == 0)
        {
            return;
        }

    for(int i = 0; i<n; ++i)
        {
            float* v = arPos + (i * 3);
            mLocalBoundingBox.Encapsulate(v[0], v[1], v[2]);
        }
}

void StaticMesh::RenderInternal()
{
    glVertexPointer(3, GL_FLOAT, 0, mPos.get());
    glEnableClientState ( GL_VERTEX_ARRAY );

    glClientActiveTextureARB ( GL_TEXTURE0_ARB );
    glTexCoordPointer( 3, GL_FLOAT, 0, mTexCoords.get());
    glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

    for (unsigned int i=0; i < mMaterials.size(); ++i)
        {
            mMaterials[i]->Bind();
            glDrawElements(GL_TRIANGLES, mFaceCount[i]*3,
                           GL_UNSIGNED_INT, mFaces[i].get());
        }

    glDisableClientState (GL_VERTEX_ARRAY);
    glClientActiveTextureARB ( GL_TEXTURE0_ARB );
    glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}

bool StaticMesh::Load(const std::string& fileName)
{
    // open file
    shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>
        (GetCore()->Get("/sys/server/file"));
    shared_ptr<salt::RFile> file = fileServer->Open(fileName.c_str());

    if (file.get() == 0)
        {
            return false;
        }

    NVMeshMender meshmender;
    NVMeshMender::VAVector input, output;
    NVMeshMender::VertexAttribute  att;

    char buffer[1024];
    int temp;
    int i;

    GetLog()->Normal() << "Loading " << fileName << "\n";
    file->Gets(buffer, 1024);
    sscanf(buffer, "TotalTriCount: %d", &temp);
    GetLog()->Normal() << "  TotalTriCount: " << temp << "\n";

    int vertexCount;
    file->Gets(buffer, 1024);
    sscanf(buffer, "VertexCount:   %d", &vertexCount);
    GetLog()->Normal() << "  VertexCount:   " << vertexCount << "\n";

    int meshCount;
    file->Gets(buffer, 1024);
    sscanf(buffer, "MeshCount:     %d", &meshCount);
    GetLog()->Normal() << "  MeshCount:     " << meshCount << "\n";
    file->Gets(buffer, 1024);

    // load position and texture coordinates of this object
    NVMeshMender::VertexAttribute   position;
    position.Name_ = "position";

    NVMeshMender::VertexAttribute   tex0;
    tex0.Name_ = "tex0";

    for (int v = 0; v < vertexCount; ++v)
        {
            float x, y, z;

            file->Gets(buffer, 1024);
            sscanf(buffer, "%f %f %f", &x, &y, &z);
            position.floatVector_.push_back(x);     position.floatVector_.push_back(y);     position.floatVector_.push_back(z);

            file->Gets(buffer, 1024);
            sscanf(buffer, "%f %f", &x, &y);
            tex0.floatVector_.push_back(x); tex0.floatVector_.push_back(y); tex0.floatVector_.push_back(1);
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
    shared_ptr<MaterialServer> materialServer = shared_static_cast<MaterialServer>(GetCore()->Get("/sys/server/material"));
    while (!file->Eof())
        {
            if (file->Gets(buffer, 1024)>0 && buffer[0] == 'M')
                {
                    char matName[256];
                    int dummy;
                    sscanf(buffer, "Material %d: %s", &dummy, matName);
                    mMaterials.push_back(materialServer->GetMaterial(matName));
                }
        }

    file->Close();

    // output
    att.Name_ = "position"; output.push_back(att);
    att.Name_ = "tex0";             output.push_back(att);
    att.Name_ = "normal";   output.push_back(att);
    att.Name_ = "tangent";  output.push_back(att);
    att.Name_ = "binormal"; output.push_back(att);
    att.Name_ = "indices";  output.push_back(att);
    att.Name_ = "material"; output.push_back(att);

    //loadVoidModel("model/knot.void", input);

    // munge
    meshmender.Munge(input, output, 3.141592654f/3.0f, 0, NVMeshMender::FixTangents, NVMeshMender::DontFixCylindricalTexGen, NVMeshMender::WeightNormalsByFaceSize);

    NVMeshMender::VertexAttribute   outIndices;
    NVMeshMender::VertexAttribute   outMaterial;

    for (i=0; i < (int)output.size(); ++i)
        {
            string b = output[i].Name_;

            if (output[i].Name_ == "position")
                {
                    mVertexCount = output[i].floatVector_.size()/3;
                    mPos = shared_array<float>(new float[mVertexCount * 3]);
                    for (int j = 0; j < (mVertexCount*3); ++j)
                        {
                            mPos[j] = output[i].floatVector_[j];
                        }
                }

            if (output[i].Name_ == "tex0")
                {
                    int size = output[i].floatVector_.size()/3;
                    mTexCoords = shared_array<float>(new float[size * 3]);

                    for (int j = 0; j < (size*3); ++j)
                        {
                            mTexCoords[j] = output[i].floatVector_[j];
                        }
                }

            if (output[i].Name_ == "tangent")
                {
                    int size = output[i].floatVector_.size()/3;
                    mBasisX = shared_array<float>(new float[size * 3]);

                    for (int j = 0; j < (size*3); ++j)
                        {
                            mBasisX[j] = output[i].floatVector_[j];
                        }
                }

            if (output[i].Name_ == "binormal")
                {
                    int size = output[i].floatVector_.size()/3;
                    mBasisY = shared_array<float>(new float[size * 3]);

                    for (int j = 0; j < (size * 3); ++j)
                        {
                            mBasisY[j] = output[i].floatVector_[j];
                        }
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

    mFaceCount.resize(meshCount);
    mFaces.resize(meshCount);

    for (i = 0; i<meshCount; ++i)
        {
            //mMaterials[i] = materialServer->GetMaterial("material/door");
            mFaceCount[i] = 0;
        }

    int totalFaceCount = outIndices.intVector_.size()/3;
    for (i = 0; i < totalFaceCount; ++i)
        {
            mFaceCount[outMaterial.intVector_[outIndices.intVector_[3*i]*3]]++;
        }

    for (i = 0; i<meshCount; ++i)
        {
            //printf("Resorting: %d %d\n", i, mFaceCount[i]);
            mFaces[i] = shared_array<unsigned int>
                (new unsigned int[mFaceCount[i] * 3]);
            unsigned int* faces = mFaces[i].get();

            int current = 0;
            for (int f = 0; f < totalFaceCount; ++f)
                {
                    if (outMaterial.intVector_[outIndices.intVector_[3*f]*3]==i)
                        {
                            // add face
                            unsigned int* currFace = faces + (current * 3);

                            currFace[0] = outIndices.intVector_[3*f];
                            currFace[1] = outIndices.intVector_[3*f+1];
                            currFace[2] = outIndices.intVector_[3*f+2];
                            ++current;

                            mFaceToMaterial.push_back(i);
                        }
                }
        }

    /*
      PrepareCollision();
    */

    CalcBoundingBox();
    return true;
}
