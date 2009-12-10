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
#include "stdmeshimporter.h"
#include <salt/gmath.h>
#include <sstream>

#include <zeitgeist/logserver/logserver.h>
using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace salt;
using namespace std;

StdMeshImporter::StdMeshImporter() : MeshImporter()
{
}

StdMeshImporter::~StdMeshImporter()
{
}

static const string gSphereStr = "StdUnitSphere";
static const string gBoxStr = "StdUnitBox";
static const string gCapsuleStr = "StdCapsule";
static const string gCylinderStr = "StdUnitCylinder";

shared_ptr<TriMesh>
StdMeshImporter::ImportMesh(const std::string& name,const ParameterList& parameter)
{
    if (name == gSphereStr)
        {
            return UnitSphereMesh();
        }

    if (name == gBoxStr)
        {
            return UnitBoxMesh();
        }

    if (name == gCapsuleStr)
        {
            return UnitCapsule(parameter);
        }

    if (name == gCylinderStr)
        {
            return UnitCylinder();
        }

    return shared_ptr<TriMesh>();
}

//
// unit sphere
//

// This code is thanks to John Ratcliff
static const int gNumSphereVertices = 114;
static const int gNumSphereFaces = 224;

static float gSphereVertices[gNumSphereVertices*3] =
    {
        0.0000f,0.0000f,1.0000f,  0.0000f,0.3827f,0.9239f,  -0.1464f,0.3536f,0.9239f,
        -0.2706f,0.2706f,0.9239f,  -0.3536f,0.1464f,0.9239f,  -0.3827f,0.0000f,0.9239f,
        -0.3536f,-0.1464f,0.9239f,  -0.2706f,-0.2706f,0.9239f,  -0.1464f,-0.3536f,0.9239f,
        0.0000f,-0.3827f,0.9239f,  0.1464f,-0.3536f,0.9239f,  0.2706f,-0.2706f,0.9239f,
        0.3536f,-0.1464f,0.9239f,  0.3827f,0.0000f,0.9239f,  0.3536f,0.1464f,0.9239f,
        0.2706f,0.2706f,0.9239f,  0.1464f,0.3536f,0.9239f,  0.0000f,0.7071f,0.7071f,
        -0.2706f,0.6533f,0.7071f,  -0.5000f,0.5000f,0.7071f,  -0.6533f,0.2706f,0.7071f,
        -0.7071f,0.0000f,0.7071f,  -0.6533f,-0.2706f,0.7071f,  -0.5000f,-0.5000f,0.7071f,
        -0.2706f,-0.6533f,0.7071f,  0.0000f,-0.7071f,0.7071f,  0.2706f,-0.6533f,0.7071f,
        0.5000f,-0.5000f,0.7071f,  0.6533f,-0.2706f,0.7071f,  0.7071f,0.0000f,0.7071f,
        0.6533f,0.2706f,0.7071f,  0.5000f,0.5000f,0.7071f,  0.2706f,0.6533f,0.7071f,
        0.0000f,0.9239f,0.3827f,  -0.3536f,0.8536f,0.3827f,  -0.6533f,0.6533f,0.3827f,
        -0.8536f,0.3536f,0.3827f,  -0.9239f,0.0000f,0.3827f,  -0.8536f,-0.3536f,0.3827f,
        -0.6533f,-0.6533f,0.3827f,  -0.3536f,-0.8536f,0.3827f,  0.0000f,-0.9239f,0.3827f,
        0.3536f,-0.8536f,0.3827f,  0.6533f,-0.6533f,0.3827f,  0.8536f,-0.3536f,0.3827f,
        0.9239f,0.0000f,0.3827f,  0.8536f,0.3536f,0.3827f,  0.6533f,0.6533f,0.3827f,
        0.3536f,0.8536f,0.3827f,  0.0000f,1.0000f,0.0000f,  -0.3827f,0.9239f,0.0000f,
        -0.7071f,0.7071f,0.0000f,  -0.9239f,0.3827f,0.0000f,  -1.0000f,0.0000f,0.0000f,
        -0.9239f,-0.3827f,0.0000f,  -0.7071f,-0.7071f,0.0000f,  -0.3827f,-0.9239f,0.0000f,
        0.0000f,-1.0000f,0.0000f,  0.3827f,-0.9239f,0.0000f,  0.7071f,-0.7071f,0.0000f,
        0.9239f,-0.3827f,0.0000f,  1.0000f,0.0000f,0.0000f,  0.9239f,0.3827f,0.0000f,
        0.7071f,0.7071f,0.0000f,  0.3827f,0.9239f,0.0000f,  0.0000f,0.9239f,-0.3827f,
        -0.3536f,0.8536f,-0.3827f,  -0.6533f,0.6533f,-0.3827f,  -0.8536f,0.3536f,-0.3827f,
        -0.9239f,0.0000f,-0.3827f,  -0.8536f,-0.3536f,-0.3827f,  -0.6533f,-0.6533f,-0.3827f,
        -0.3536f,-0.8536f,-0.3827f,  0.0000f,-0.9239f,-0.3827f,  0.3536f,-0.8536f,-0.3827f,
        0.6533f,-0.6533f,-0.3827f,  0.8536f,-0.3536f,-0.3827f,  0.9239f,0.0000f,-0.3827f,
        0.8536f,0.3536f,-0.3827f,  0.6533f,0.6533f,-0.3827f,  0.3536f,0.8536f,-0.3827f,
        0.0000f,0.7071f,-0.7071f,  -0.2706f,0.6533f,-0.7071f,  -0.5000f,0.5000f,-0.7071f,
        -0.6533f,0.2706f,-0.7071f,  -0.7071f,0.0000f,-0.7071f,  -0.6533f,-0.2706f,-0.7071f,
        -0.5000f,-0.5000f,-0.7071f,  -0.2706f,-0.6533f,-0.7071f,  0.0000f,-0.7071f,-0.7071f,
        0.2706f,-0.6533f,-0.7071f,  0.5000f,-0.5000f,-0.7071f,  0.6533f,-0.2706f,-0.7071f,
        0.7071f,0.0000f,-0.7071f,  0.6533f,0.2706f,-0.7071f,  0.5000f,0.5000f,-0.7071f,
        0.2706f,0.6533f,-0.7071f,  0.0000f,0.3827f,-0.9239f,  -0.1464f,0.3536f,-0.9239f,
        -0.2706f,0.2706f,-0.9239f,  -0.3536f,0.1464f,-0.9239f,  -0.3827f,0.0000f,-0.9239f,
        -0.3536f,-0.1464f,-0.9239f,  -0.2706f,-0.2706f,-0.9239f,  -0.1464f,-0.3536f,-0.9239f,
        0.0000f,-0.3827f,-0.9239f,  0.1464f,-0.3536f,-0.9239f,  0.2706f,-0.2706f,-0.9239f,
        0.3536f,-0.1464f,-0.9239f,  0.3827f,0.0000f,-0.9239f,  0.3536f,0.1464f,-0.9239f,
        0.2706f,0.2706f,-0.9239f,  0.1464f,0.3536f,-0.9239f,  0.0000f,0.0000f,-1.0000f
    };

static unsigned int gSphereFaces[gNumSphereFaces*3] =
    {
        0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,8, 0,8,9, 0,9,10,
        0,10,11, 0,11,12, 0,12,13, 0,13,14, 0,14,15, 0,15,16, 0,16,1, 1,17,18, 1,18,2,
        2,18,19, 2,19,3, 3,19,20, 3,20,4, 4,20,21, 4,21,5, 5,21,22, 5,22,6, 6,22,23,
        6,23,7, 7,23,24, 7,24,8, 8,24,25, 8,25,9, 9,25,26, 9,26,10, 10,26,27, 10,27,11,
        11,27,28, 11,28,12, 12,28,29, 12,29,13, 13,29,30, 13,30,14, 14,30,31, 14,31,15, 15,31,32,
        15,32,16, 16,32,17, 16,17,1, 17,33,34, 17,34,18, 18,34,35, 18,35,19, 19,35,36, 19,36,20,
        20,36,37, 20,37,21, 21,37,38, 21,38,22, 22,38,39, 22,39,23, 23,39,40, 23,40,24, 24,40,41,
        24,41,25, 25,41,42, 25,42,26, 26,42,43, 26,43,27, 27,43,44, 27,44,28, 28,44,45, 28,45,29,
        29,45,46, 29,46,30, 30,46,47, 30,47,31, 31,47,48, 31,48,32, 32,48,33, 32,33,17, 33,49,50,
        33,50,34, 34,50,51, 34,51,35, 35,51,52, 35,52,36, 36,52,53, 36,53,37, 37,53,54, 37,54,38,
        38,54,55, 38,55,39, 39,55,56, 39,56,40, 40,56,57, 40,57,41, 41,57,58, 41,58,42, 42,58,59,
        42,59,43, 43,59,60, 43,60,44, 44,60,61, 44,61,45, 45,61,62, 45,62,46, 46,62,63, 46,63,47,
        47,63,64, 47,64,48, 48,64,49, 48,49,33, 49,65,66, 49,66,50, 50,66,67, 50,67,51, 51,67,68,
        51,68,52, 52,68,69, 52,69,53, 53,69,70, 53,70,54, 54,70,71, 54,71,55, 55,71,72, 55,72,56,
        56,72,73, 56,73,57, 57,73,74, 57,74,58, 58,74,75, 58,75,59, 59,75,76, 59,76,60, 60,76,77,
        60,77,61, 61,77,78, 61,78,62, 62,78,79, 62,79,63, 63,79,80, 63,80,64, 64,80,65, 64,65,49,
        65,81,82, 65,82,66, 66,82,83, 66,83,67, 67,83,84, 67,84,68, 68,84,85, 68,85,69, 69,85,86,
        69,86,70, 70,86,87, 70,87,71, 71,87,88, 71,88,72, 72,88,89, 72,89,73, 73,89,90, 73,90,74,
        74,90,91, 74,91,75, 75,91,92, 75,92,76, 76,92,93, 76,93,77, 77,93,94, 77,94,78, 78,94,95,
        78,95,79, 79,95,96, 79,96,80, 80,96,81, 80,81,65, 81,97,98, 81,98,82, 82,98,99, 82,99,83,
        83,99,100, 83,100,84, 84,100,101, 84,101,85, 85,101,102, 85,102,86, 86,102,103, 86,103,87, 87,103,104,
        87,104,88, 88,104,105, 88,105,89, 89,105,106, 89,106,90, 90,106,107, 90,107,91, 91,107,108, 91,108,92,
        92,108,109, 92,109,93, 93,109,110, 93,110,94, 94,110,111, 94,111,95, 95,111,112, 95,112,96, 96,112,97,
        96,97,81, 113,98,97, 113,99,98, 113,100,99, 113,101,100, 113,102,101, 113,103,102, 113,104,103, 113,105,104,
        113,106,105, 113,107,106, 113,108,107, 113,109,108, 113,110,109, 113,111,110, 113,112,111, 113,97,112
    };

shared_ptr<TriMesh> StdMeshImporter::UnitSphereMesh()
{
    shared_ptr<TriMesh> mesh(new TriMesh());

    shared_array<float> pos(new float[gNumSphereVertices*3]);
    memcpy(pos.get(),gSphereVertices,gNumSphereVertices*3*sizeof(float));
    mesh->SetPos(pos,gNumSphereVertices);

    // vertices on a unit sphere are identical to their corresponding
    // normal
    mesh->SetNormals(pos);

    shared_ptr<IndexBuffer> idx(new IndexBuffer());
    idx->Cache(gNumSphereFaces*3,gSphereFaces);
    mesh->AddFace(idx);

    return mesh;
}

//
// unit box
//

static const int gNumBoxVertices = 24;
static const int gNumBoxFaces = 12;

static float gBoxVertices[gNumBoxVertices*3] =
    {
        -0.5f,-0.5f,0.5f,  0.5f,-0.5f, 0.5f,   0.5f,0.5f,0.5f,
        -0.5f,0.5f,0.5f,  -0.5f,-0.5f,-0.5f,  -0.5f,0.5f,-0.5f,
        0.5f,0.5f,-0.5f,   0.5f,-0.5f,-0.5f,  -0.5f,0.5f,-0.5f,
        -0.5f,0.5f,0.5f,   0.5f,0.5f,0.5f,     0.5f,0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f, 0.5f,-0.5f,-0.5f,   0.5f,-0.5f,0.5f,
        -0.5f,-0.5f,0.5f,  0.5f,-0.5f,-0.5f,   0.5f,0.5f,-0.5f,
        0.5f,0.5f,0.5f,    0.5f,-0.5f,0.5f,   -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,0.5f,  -0.5f,0.5f,0.5f,   -0.5f,0.5f,-0.5f
    };

static float gBoxNormals[gNumBoxVertices*3] =
    {
         0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,
         0.0f,0.0f,1.0f,  0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f,
         0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, 0.0f,1.0f,0.0f,
         0.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,
         0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f,
         0.0f,-1.0f,0.0f, 1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,
         1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  -1.0f,0.0f,0.0f,
         -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f
    };

// by jan
static float gBoxTexCoords[gNumBoxVertices*3] =
    {
        0.0f,1.0f,0.0f,  1.0f,1.0f,0.0f,  1.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,
        0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,  1.0f,1.0f,0.0f,  1.0f,0.0f,0.0f,
        1.0f,1.0f,0.0f, 1.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,  1.0f,1.0f,0.0f,  1.0f,0.0f,0.0f, 0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,  1.0f,1.0f,0.0f, 1.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,
        1.0f,1.0f,0.0f, 1.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,  0.0f,1.0f,0.0f
    };

//Original
// static float gBoxTexCoords[gNumBoxVertices*3] =
//     {
//         0.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,
//         0.0f,1.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,
//         0.0f,1.0f,0.0f,  0.0f,0.0f,0.0f,  0.0f,1.0f,0.0f,
//         0.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,
//         1.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,0.0f,
//         1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,
//         0.0f,1.0f,0.0f,  0.0f,0.0f,0.0f,  0.0f,0.0f,0.0f,
//         1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,  0.0f,1.0f,0.0f
//     };

static unsigned int gBoxFaces[gNumSphereFaces*3] =
    {
        0,1,2, 0,2,3, 4,5,6, 4,6,7, 8,9,10, 8,10,11,
        12,13,14, 12,14,15, 16,17,18, 16,18,19,
        20,21,22, 20,22,23
    };

shared_ptr<TriMesh> StdMeshImporter::UnitBoxMesh()
{
    shared_ptr<TriMesh> mesh(new TriMesh());

    shared_array<float> pos(new float[gNumBoxVertices*3]);
    memcpy(pos.get(),gBoxVertices,gNumBoxVertices*3*sizeof(float));
    mesh->SetPos(pos,gNumBoxVertices);

    shared_array<float> normals(new float[gNumBoxVertices*3]);
    memcpy(normals.get(),gBoxNormals,gNumBoxVertices*3*sizeof(float));
    mesh->SetNormals(normals);

    shared_array<float> tex(new float[gNumBoxVertices*3]);
    memcpy(tex.get(), gBoxTexCoords,gNumBoxVertices*3*sizeof(float));
    mesh->SetTexCoords(tex);

    shared_ptr<IndexBuffer> idx(new IndexBuffer());
    idx->Cache(gNumBoxFaces*3,gBoxFaces);
    mesh->AddFace(idx);

    return mesh;
}

//
// unit capsule
//
static void AddVertex(float** at, float x, float y, float z)
{
    (**at) = x; ++(*at);
    (**at) = y; ++(*at);
    (**at) = z; ++(*at);
}

std::string StdMeshImporter::MangleName (const string& name, const ParameterList& parameter)
{
    if (
        (name != gCapsuleStr) ||
        (parameter.GetSize() < 2)
        )
        {
            return name;
        }

    stringstream ss;
    ss << gCapsuleStr;

    float radius;
    if (parameter.GetValue(parameter[0],radius))
        {
            ss << "R=" << radius;
        }

    float length;
    if (parameter.GetValue(parameter[1],length))
        {
            ss << "L=" << length;
        }

    return ss.str();
}

shared_ptr<TriMesh> StdMeshImporter::UnitCapsule(const ParameterList& parameter)
{
    //
    // code adapted from ODE's drawstuff lib
    //

    // generated capsule with r=1 and l=1
    float ccRadius = 1;
    float ccLength = 1;

    GetLog()->Debug()
        << "(StdMeshImporter::UnitCapsule) paramSize="
        << parameter.GetSize() << "\n";
    if (parameter.GetSize() >= 2)
        {
            parameter.GetValue(parameter[0],ccRadius);
            parameter.GetValue(parameter[1],ccLength);
        }

    // number of sides to the cylinder (divisible by 4):
    const int capsule_quality = 3;
    const int n = capsule_quality*4;

    int innerLoop = (n+1);
    int numVertices = innerLoop * 2;
    numVertices += (innerLoop * (n/4) * 2);
    numVertices += (innerLoop * (n/4) * 2);

    shared_array<float> pos(new float[numVertices * 3]);
    shared_array<float> normals(new float[numVertices * 3]);
    float* cVert = pos.get();
    float* cNormal = normals.get();

    int i,j;
    float tmp,nx,start_nx,start_ny;

    float l = ccLength * 0.5f;
    float a = static_cast<float>(g2PI / n);
    float sa = gSin(a);
    float ca = gCos(a);

    // draw cylinder body

    // normal vector = (0,ny,nz)
    float ny = 1;
    float nz = 0;

    for (i=0; i<=n; ++i)
        {
            AddVertex(&cNormal,ny,nz,0);
            AddVertex(&cVert,ny*ccRadius,nz*ccRadius,l);
            AddVertex(&cNormal,ny,nz,0);
            AddVertex(&cVert,ny*ccRadius,nz*ccRadius,-l);

            // rotate ny,nz
            tmp = ca * ny - sa * nz;
            nz = sa*ny + ca*nz;
            ny = tmp;
        }

    // draw first cylinder cap
    start_nx = 0;
    start_ny = 1;

    for (j=0; j<(n/4); ++j)
        {
            // get start_n2 = rotated start_n
            float start_nx2 =  ca*start_nx + sa*start_ny;
            float start_ny2 = -sa*start_nx + ca*start_ny;

            // get n=start_n and n2=start_n2
            nx = start_nx;
            ny = start_ny;
            nz = 0;
            float nx2 = start_nx2;
            float ny2 = start_ny2;
            float nz2 = 0;

            for (i=0; i<=n; i++)
                {
                    AddVertex(&cNormal,ny2,nz2,nx2);
                    AddVertex(&cVert,ny2*ccRadius,nz2*ccRadius,l+nx2*ccRadius);
                    AddVertex(&cNormal,ny,nz,nx);
                    AddVertex(&cVert,ny*ccRadius,nz*ccRadius,l+nx*ccRadius);

                    // rotate n,n2
                    tmp = ca*ny - sa*nz;
                    nz = sa*ny + ca*nz;
                    ny = tmp;
                    tmp = ca*ny2- sa*nz2;
                    nz2 = sa*ny2 + ca*nz2;
                    ny2 = tmp;
                }

            start_nx = start_nx2;
            start_ny = start_ny2;
        }

    // draw second cylinder cap
    start_nx = 0;
    start_ny = 1;

    for (j=0; j<(n/4); j++)
        {
            // get start_n2 = rotated start_n
            float start_nx2 = ca*start_nx - sa*start_ny;
            float start_ny2 = sa*start_nx + ca*start_ny;

            // get n=start_n and n2=start_n2
            nx = start_nx; ny = start_ny; nz = 0;
            float nx2 = start_nx2;
            float ny2 = start_ny2;
            float nz2 = 0;

            for (i=0; i<=n; i++)
                {
                    AddVertex(&cNormal,ny,nz,nx);
                    AddVertex(&cVert,ny*ccRadius,nz*ccRadius,-l+nx*ccRadius);
                    AddVertex(&cNormal,ny2,nz2,nx2);
                    AddVertex(&cVert,ny2*ccRadius,nz2*ccRadius,-l+nx2*ccRadius);
                    // rotate n,n2
                    tmp = ca*ny - sa*nz;
                    nz = sa*ny + ca*nz;
                    ny = tmp;
                    tmp = ca*ny2- sa*nz2;
                    nz2 = sa*ny2 + ca*nz2;
                    ny2 = tmp;
                }

            start_nx = start_nx2;
            start_ny = start_ny2;
        }

    shared_ptr<IndexBuffer> idx(new IndexBuffer());
    // triangle strip -> triangles
    int numFaces = (numVertices - 2);
    idx->EnsureFit(numFaces*3);

    for (i=0;i<numFaces;++i)
        {
            if (i%2)
                {
                    idx->Cache(i+1);
                    idx->Cache(i);
                    idx->Cache(i+2);
                } else
                    {
                        idx->Cache(i);
                        idx->Cache(i+1);
                        idx->Cache(i+2);
                    }
        }

    shared_ptr<TriMesh> mesh(new TriMesh());
    mesh->SetPos(pos,numVertices);
    mesh->SetNormals(normals);
    mesh->AddFace(idx);
    mesh->SetName(MangleName(gCapsuleStr,parameter));

    return mesh;
}

//
// unit cylinder (radius = 1, length = 1)
//

static const int gNumCylinderVertices = 66;
static const int gNumCylinderFaces = 128;

static float gCylinderVertices[gNumCylinderVertices*3] =
    {
        0.70711f,0.70711f,-0.5f,
        0.83147f,0.55557f,-0.5f,
        0.92388f,0.38268f,-0.5f,
        0.98079f,0.19509f,-0.5f,
        1.0f,0.0f,-0.5f,
        0.98079f,-0.19509f,-0.5f,
        0.92388f,-0.38268f,-0.5f,
        0.83147f,-0.55557f,-0.5f,
        0.70711f,-0.70711f,-0.5f,
        0.55557f,-0.83147f,-0.5f,
        0.38268f,-0.92388f,-0.5f,
        0.19509f,-0.98079f,-0.5f,
        -0.0f,-1.0f,-0.5f,
        -0.19509f,-0.98079f,-0.5f,
        -0.38268f,-0.92388f,-0.5f,
        -0.55557f,-0.83147f,-0.5f,
        -0.70711f,-0.70711f,-0.5f,
        -0.83147f,-0.55557f,-0.5f,
        -0.92388f,-0.38268f,-0.5f,
        -0.98079f,-0.19509f,-0.5f,
        -1.0f,0.0f,-0.5f,
        -0.98079f,0.19509f,-0.5f,
        -0.92388f,0.38268f,-0.5f,
        -0.83147f,0.55557f,-0.5f,
        -0.70711f,0.70711f,-0.5f,
        -0.55557f,0.83147f,-0.5f,
        -0.38268f,0.92388f,-0.5f,
        -0.19509f,0.98079f,-0.5f,
        0.0f,1.0f,-0.5f,
        0.19509f,0.98078f,-0.5f,
        0.38269f,0.92388f,-0.5f,
        0.55557f,0.83147f,-0.5f,
        0.70711f,0.70711f,0.5f,
        0.83147f,0.55557f,0.5f,
        0.92388f,0.38268f,0.5f,
        0.98079f,0.19509f,0.5f,
        1.0f,-0.0f,0.5f,
        0.98078f,-0.19509f,0.5f,
        0.92388f,-0.38268f,0.5f,
        0.83147f,-0.55557f,0.5f,
        0.70711f,-0.70711f,0.5f,
        0.55557f,-0.83147f,0.5f,
        0.38268f,-0.92388f,0.5f,
        0.19509f,-0.98079f,0.5f,
        0.0f,-1.0f,0.5f,
        -0.19509f,-0.98079f,0.5f,
        -0.38268f,-0.92388f,0.5f,
        -0.55557f,-0.83147f,0.5f,
        -0.7071f,-0.70711f,0.5f,
        -0.83147f,-0.55557f,0.5f,
        -0.92388f,-0.38269f,0.5f,
        -0.98078f,-0.19509f,0.5f,
        -1.0f,-0.0f,0.5f,
        -0.98079f,0.19509f,0.5f,
        -0.92388f,0.38268f,0.5f,
        -0.83147f,0.55557f,0.5f,
        -0.70711f,0.7071f,0.5f,
        -0.55558f,0.83147f,0.5f,
        -0.38269f,0.92388f,0.5f,
        -0.1951f,0.98078f,0.5f,
        -1e-05f,1.0f,0.5f,
        0.19508f,0.98079f,0.5f,
        0.38268f,0.92388f,0.5f,
        0.55556f,0.83147f,0.5f,
        0.0f,-0.0f,-0.5f,
        0.0f,0.0f,0.5f
    };

static float gCylinderNormals[gNumCylinderVertices*3] =
    {
        0.49876f,0.49876f,-0.70879f,
        0.5865f,0.39189f,-0.70879f,
        0.65169f,0.26994f,-0.70879f,
        0.69182f,0.13761f,-0.70879f,
        0.70537f,0.0f,-0.70879f,
        0.69182f,-0.13761f,-0.70879f,
        0.65169f,-0.26994f,-0.70879f,
        0.5865f,-0.39189f,-0.70879f,
        0.49876f,-0.49876f,-0.70879f,
        0.39189f,-0.5865f,-0.70879f,
        0.26994f,-0.65169f,-0.70879f,
        0.13761f,-0.69182f,-0.70879f,
        0.0f,-0.70537f,-0.70879f,
        -0.13761f,-0.69182f,-0.70879f,
        -0.26994f,-0.65169f,-0.70879f,
        -0.39189f,-0.5865f,-0.70879f,
        -0.49876f,-0.49876f,-0.70879f,
        -0.5865f,-0.39189f,-0.70879f,
        -0.65169f,-0.26994f,-0.70879f,
        -0.69182f,-0.13761f,-0.70879f,
        -0.70537f,0.0f,-0.70879f,
        -0.69182f,0.13761f,-0.70879f,
        -0.65169f,0.26994f,-0.70879f,
        -0.5865f,0.39189f,-0.70879f,
        -0.49876f,0.49876f,-0.70879f,
        -0.39189f,0.5865f,-0.70879f,
        -0.26994f,0.65169f,-0.70879f,
        -0.13761f,0.69182f,-0.70879f,
        0.0f,0.70537f,-0.70879f,
        0.13761f,0.69182f,-0.70879f,
        0.26994f,0.65169f,-0.70879f,
        0.39189f,0.5865f,-0.70879f,
        0.49876f,0.49876f,0.70879f,
        0.5865f,0.39189f,0.70879f,
        0.65169f,0.26994f,0.70879f,
        0.69182f,0.13761f,0.70879f,
        0.70537f,0.0f,0.70879f,
        0.69182f,-0.13761f,0.70879f,
        0.65169f,-0.26994f,0.70879f,
        0.5865f,-0.39189f,0.70879f,
        0.49876f,-0.49876f,0.70879f,
        0.39189f,-0.5865f,0.70879f,
        0.26994f,-0.65169f,0.70879f,
        0.13761f,-0.69182f,0.70879f,
        0.0f,-0.70537f,0.70879f,
        -0.13761f,-0.69182f,0.70879f,
        -0.26994f,-0.65169f,0.70879f,
        -0.39189f,-0.5865f,0.70879f,
        -0.49876f,-0.49876f,0.70879f,
        -0.5865f,-0.39189f,0.70879f,
        -0.65169f,-0.26994f,0.70879f,
        -0.69182f,-0.13761f,0.70879f,
        -0.70537f,0.0f,0.70879f,
        -0.69182f,0.13761f,0.70879f,
        -0.65169f,0.26994f,0.70879f,
        -0.5865f,0.39189f,0.70879f,
        -0.49876f,0.49876f,0.70879f,
        -0.39189f,0.5865f,0.70879f,
        -0.26994f,0.65169f,0.70879f,
        -0.13761f,0.69182f,0.70879f,
        0.0f,0.70537f,0.70879f,
        0.13761f,0.69182f,0.70879f,
        0.26994f,0.65169f,0.70879f,
        0.39189f,0.5865f,0.70879f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,1.0f
    };

static unsigned int gCylinderFaces[gNumCylinderFaces*3] =
    {
        64,0,1,
        65,33,32,
        64,1,2,
        65,34,33,
        64,2,3,
        65,35,34,
        64,3,4,
        65,36,35,
        64,4,5,
        65,37,36,
        64,5,6,
        65,38,37,
        64,6,7,
        65,39,38,
        64,7,8,
        65,40,39,
        64,8,9,
        65,41,40,
        64,9,10,
        65,42,41,
        64,10,11,
        65,43,42,
        64,11,12,
        65,44,43,
        64,12,13,
        65,45,44,
        64,13,14,
        65,46,45,
        64,14,15,
        65,47,46,
        64,15,16,
        65,48,47,
        64,16,17,
        65,49,48,
        64,17,18,
        65,50,49,
        64,18,19,
        65,51,50,
        64,19,20,
        65,52,51,
        64,20,21,
        65,53,52,
        64,21,22,
        65,54,53,
        64,22,23,
        65,55,54,
        64,23,24,
        65,56,55,
        64,24,25,
        65,57,56,
        64,25,26,
        65,58,57,
        64,26,27,
        65,59,58,
        64,27,28,
        65,60,59,
        64,28,29,
        65,61,60,
        64,29,30,
        65,62,61,
        64,30,31,
        65,63,62,
        31,0,64,
        65,32,63,
        0,32,33,
        33,1,0,
        1,33,34,
        34,2,1,
        2,34,35,
        35,3,2,
        3,35,36,
        36,4,3,
        4,36,37,
        37,5,4,
        5,37,38,
        38,6,5,
        6,38,39,
        39,7,6,
        7,39,40,
        40,8,7,
        8,40,41,
        41,9,8,
        9,41,42,
        42,10,9,
        10,42,43,
        43,11,10,
        11,43,44,
        44,12,11,
        12,44,45,
        45,13,12,
        13,45,46,
        46,14,13,
        14,46,47,
        47,15,14,
        15,47,48,
        48,16,15,
        16,48,49,
        49,17,16,
        17,49,50,
        50,18,17,
        18,50,51,
        51,19,18,
        19,51,52,
        52,20,19,
        20,52,53,
        53,21,20,
        21,53,54,
        54,22,21,
        22,54,55,
        55,23,22,
        23,55,56,
        56,24,23,
        24,56,57,
        57,25,24,
        25,57,58,
        58,26,25,
        26,58,59,
        59,27,26,
        27,59,60,
        60,28,27,
        28,60,61,
        61,29,28,
        29,61,62,
        62,30,29,
        30,62,63,
        63,31,30,
        32,0,31,
        31,63,32
    };

shared_ptr<TriMesh> StdMeshImporter::UnitCylinder()
{
    shared_ptr<TriMesh> mesh(new TriMesh());

    shared_array<float> pos(new float[gNumCylinderVertices*3]);
    memcpy(pos.get(),gCylinderVertices,gNumCylinderVertices*3*sizeof(float));
    mesh->SetPos(pos,gNumCylinderVertices);

    shared_array<float> normals(new float[gNumCylinderVertices*3]);
    memcpy(normals.get(),gCylinderNormals,gNumCylinderVertices*3*sizeof(float));
    mesh->SetNormals(normals);

    shared_ptr<IndexBuffer> idx(new IndexBuffer());
    idx->Cache(gNumCylinderFaces*3,gCylinderFaces);
    mesh->AddFace(idx);

    return mesh;
}
