/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#include "objimporter.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <kerosin/materialserver/materialserver.h>
#include <kerosin/materialserver/material2dtexture.h>
#include <kerosin/materialserver/materialsolid.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace zeitgeist;
using namespace salt;
using namespace oxygen;
using namespace kerosin;
using namespace std;
using namespace boost;

ObjImporter::ObjImporter()
    : MeshImporter()
{
}

ObjImporter::~ObjImporter()
{
}

boost::shared_ptr<TriMesh> ObjImporter::ImportMesh
(const string& name, const ParameterList& parameter)
{
    // open file
    string fileName;

    if (! GetFile()->LocateResource(name, fileName))
        {
            GetLog()->Error()
                << "(ObjImporter) ERROR: cannot locate file '" << name << "'\n";
            return boost::shared_ptr<TriMesh>();
        }

    ifstream ifs;
    ifs.open(fileName.c_str());

    if (! ifs)
        {
            GetLog()->Error()
                << "(ObjImporter) ERROR: cannot open file '" << name << "'\n";
            return boost::shared_ptr<TriMesh>();
        }

    // buffers to hold lines from the file
    string buffer, templine;

    // skip first 2 lines (comment)
    getline(ifs, buffer);
    getline(ifs, buffer);

    // material lib name
    getline(ifs, buffer);

    stringstream matlibline(buffer);
    string matlibtag, matlibname;

    matlibline >> matlibtag >> matlibname;

    TObjMatValueVector materials;

    if (! (matlibtag == "mtllib"))
        {
            GetLog()->Debug()
                << "(ObjImporter) WARNING: no material library name given.\n";
        }
    else
        {
            if (! SetupMaterials(matlibname, materials))
                {
                    GetLog()->Debug()
                        << "(ObjImporter) WARNING: could not setup materials, "
                        << "will set default material matWhite which has to be defined manually.'\n";
                }
        }

    // skip object name
    getline(ifs, buffer);

    // we need to store vertices, texture coords, vertex normals,
    // and material (texture) names
    vector<Vector3f> pos, tex, norm;
    TMatNameFaceIndexListMap matFaceListMap;
    GetLog()->Normal() << "(ObjImporter) Loading " << name << "\n";

    float x,y,z;
    string mat, f1, f2, f3;
    VTNIndex idx1, idx2, idx3;
    FaceIndex tempFace;

    unsigned int totalFaceCount = 0;

    while(! ifs.eof())
        {
            getline(ifs, templine);

            stringstream str(templine);

            str >> buffer;

            if (buffer == "v") // vertex pos
                {
                    str >> x >> y >> z;
                    pos.push_back(Vector3f(x, y, z));
                }
            else if (buffer == "vt") // vertex texture coords
                {
                    str >> x >> y;
                    // ...
                    //tex.push_back(Vector3f(0.5 + (0.5 - (x - 0.25)), 0.75 - (0.5 - y), 0.0));
                    tex.push_back(Vector3f(1.0 - (1.0 - x), 1.0 - y, 0.0));
                }
            else if (buffer == "vn") // vertex normal
                {
                    str >> x >> y >> z;
                    norm.push_back(Vector3f(x, y, z));
                }
            else if (buffer == "usemtl") // material
                {
                    if (materials.size() != 0)
                        {
                            str >> mat;
                        }
                    else
                        {
                            mat = "matWhite";
                        }
                }
            else if (buffer == "f") // face
                {
                    str >> f1 >> f2 >> f3;

                    // we need to have normals
                    if (norm.size() == 0)
                        {
                            GetLog()->Error()
                                << "(ObjImporter) ERROR: normals are missing in the OBJ file, exiting.\n";
                            return boost::shared_ptr<TriMesh>();
                        }

                    // three indeces for each face:
                    // vertex pos / texture coord / normal

                    string del("/");
                    vector<string> exploded;
                    ExplodeString(f1, exploded, del);

                    assert(exploded.size() == 3);

                    idx1.v  = atoi(exploded[0].c_str());
                    idx1.vt = atoi(exploded[1].c_str());
                    idx1.vn = atoi(exploded[2].c_str());

                    exploded.clear();
                    ExplodeString(f2, exploded, del);

                    assert(exploded.size() == 3);

                    idx2.v  = atoi(exploded[0].c_str());
                    idx2.vt = atoi(exploded[1].c_str());
                    idx2.vn = atoi(exploded[2].c_str());

                    exploded.clear();
                    ExplodeString(f3, exploded, del);

                    assert(exploded.size() == 3);

                    idx3.v  = atoi(exploded[0].c_str());
                    idx3.vt = atoi(exploded[1].c_str());
                    idx3.vn = atoi(exploded[2].c_str());

                    exploded.clear();

                    tempFace.vertex[0] = idx1;
                    tempFace.vertex[1] = idx2;
                    tempFace.vertex[2] = idx3;

                    if (mat.size() != 0)
                        {
                            matFaceListMap[mat].push_back(tempFace);

                            ++totalFaceCount;
                        }
                    else
                        {
                            GetLog()->Error()
                                << "(ObjImporter) WARNING: no material defined for use with "
                                << "this face index, so I'll ignore it and go on.\n";
                        }
                }
        }

    // close the file
    ifs.close();

    // Now we should have all the vertex positions, texture coordinates,
    // normals, faces, and material names. These need to be converted into
    // shared arrays for the respective TriMesh methods. We need to use the
    // information in the face list to put the right information to the
    // correct indeces in the arrays.
    //
    // We store 9 values (for 3 vertices) per face for positions, tex coords,
    // and normals.
    //
    // We need to have separate index buffers for each material; tex coords and
    // normals don't care about the material, however.

    // for one face: 3 x <x,y,z>
    shared_array<float> posArray(new float[totalFaceCount * 9]);

    // for one face: 3 x <u,v,0.0> (last value ignored)
    shared_array<float> texArray(new float[totalFaceCount * 9]);

    // for one face: 3 x <x,y,z>
    shared_array<float> normArray(new float[totalFaceCount * 9]);

    boost::shared_ptr<TriMesh> triMesh(new TriMesh());

    unsigned int faceCount = 0;

    for (
         TMatNameFaceIndexListMap::iterator matIter = matFaceListMap.begin();
         matIter != matFaceListMap.end();
         ++matIter
         )
        {
            // store the vertex indices in an IndexBuffer
            boost::shared_ptr<IndexBuffer> indeces(new IndexBuffer());
            // 3 indices per face
            indeces->EnsureFit((matIter->second).size() * 3);

            // loop over the face list for the current material and setup the new arrays
            for (
                 TFaceIndexList::iterator faceIter = (matIter->second).begin();
                 faceIter != (matIter->second).end();
                 ++faceIter
                 )
                {
                    unsigned int j = 0;

                    for (int i = 0; i < 3; ++i)
                        {
                            // indices in the OBJ file start with 1 so subtract 1 here
                            j = faceIter->vertex[i].v - 1;

                            // for every face, we have 9 values (3 x <x,y,z>)
                            posArray[faceCount * 9 + i * 3]      = pos[j].x();
                            posArray[faceCount * 9 + i * 3 + 1]  = pos[j].y();
                            posArray[faceCount * 9 + i * 3 + 2]  = pos[j].z();

                            // cache this vertex index
                            indeces->Cache(faceCount * 3 + i);

                            if (tex.size() != 0)
                                {
                                    // indices in the OBJ file start with 1 so subtract 1 here
                                    j = faceIter->vertex[i].vt - 1;

                                    // for every face, we have 9 values (3 x <x,y,z>)
                                    texArray[faceCount * 9 + i * 3]      = tex[j].x();
                                    texArray[faceCount * 9 + i * 3 + 1]  = tex[j].y();
                                    texArray[faceCount * 9 + i * 3 + 2]  = tex[j].z();
                                }

                            // indices in the OBJ file start with 1 so subtract 1 here
                            j = faceIter->vertex[i].vn - 1;

                            // for every face, we have 9 values (3 x <x,y,z>)
                            normArray[faceCount * 9 + i * 3]     = norm[j].x();
                            normArray[faceCount * 9 + i * 3 + 1] = norm[j].y();
                            normArray[faceCount * 9 + i * 3 + 2] = norm[j].z();
                        }

                    // increase face counter
                    ++faceCount;
                }

            // add face with all the indices for the current material
            triMesh->AddFace(indeces, matIter->first);
        }

    triMesh->SetPos(posArray, faceCount * 3);

    if (tex.size() != 0)
        {
            triMesh->SetTexCoords(texArray);
        }

    triMesh->SetNormals(normArray);

    GetLog()->Normal() << "(ObjImporter) Done importing .obj file "
                       << name << "\n";

    return triMesh;
}

void ObjImporter::ExplodeString(string & input, vector<string> & output, string & del)
{
    size_t pos1 = 0;
    size_t pos2 = 0;

    while ((pos2 = input.find(del, pos1)) != string::npos)
        {
            output.push_back(input.substr(pos1, pos2 - pos1));
            pos1 = pos2 + 1;
            //pos2 = input.find(del, pos1);
        }

    output.push_back(input.substr(pos1, input.length() - pos1));
}

bool ObjImporter::SetupMaterials(string & matLibName, TObjMatValueVector & matVector)
{
    // open file
    string fileName;

    if (! GetFile()->LocateResource("materials/" + matLibName, fileName))
        {
            GetLog()->Error()
                << "(ObjImporter) WARNING: cannot locate file '" << matLibName << "'\n";
            return false;
        }

    ifstream ifs;
    ifs.open(fileName.c_str());

    if (! ifs)
        {
            GetLog()->Error()
                << "(ObjImporter) WARNING: cannot open file '" << fileName << "'\n";
            return false;
        }

    // buffers to hold lines from the file
    string buffer, templine;

    // skip first 2 lines (comment)
    getline(ifs, buffer);
    getline(ifs, buffer);

    string matName, texName;
    float x1,x2,x3;
    ObjMatValues* currentMat = 0;

    while(! ifs.eof())
        {
            getline(ifs, templine);

            stringstream str(templine);

            str >> buffer;

            if (buffer == "newmtl")
                {
                    // whenever we find the "newmtl" keyword, we create a
                    // new struct and store it in the vector; we fill in
                    // the values as we read the following lines
                    str >> matName;
                    ObjMatValues temp;
                    temp.name = matName;
                    matVector.push_back(temp);
                    currentMat = &(matVector.back());
                }

            if (currentMat == 0)
                {
                    continue;
                }

            if (buffer == "Ka")
                {
                    str >> x1 >> x2 >> x3;
                    currentMat->ambient[0] = x1;
                    currentMat->ambient[1] = x2;
                    currentMat->ambient[2] = x3;
                }
            else if (buffer == "Kd")
                {
                    str >> x1 >> x2 >> x3;
                    currentMat->diffuse[0] = x1;
                    currentMat->diffuse[1] = x2;
                    currentMat->diffuse[2] = x3;
                }
            else if (buffer == "Ks")
                {
                    str >> x1 >> x2 >> x3;
                    currentMat->specular[0] = x1;
                    currentMat->specular[1] = x2;
                    currentMat->specular[2] = x3;
                }
            else if (buffer == "Ns")
                {
                    str >> x1;
                    currentMat->shininess = x1;
                }
            else if (buffer == "d")
                {
                    str >> x1;
                    currentMat->alpha = x1;
                }

            else if (buffer == "map_Kd")
                {
                    str >> texName;

                    if (! GetFile()->LocateResource("textures/" + texName, currentMat->diffuseTexture))
                        {
                            GetLog()->Error()
                                << "(ObjImporter) ERROR: cannot locate texture file '"
                                <<  "textures/" << texName << "'\n";
                        }
                }
        }

    GetLog()->Debug() << "(ObjImporter) read " << matVector.size()
                      << " material definitions.\n";

    // now we read all the materials values, so create the materials and
    // register them.

    // get reference to the MaterialServer
    boost::shared_ptr<MaterialServer> materialServer = shared_dynamic_cast<MaterialServer>
        (GetCore()->Get("/sys/server/material"));

    if (materialServer.get() == 0)
        {
            GetLog()->Error()
                << "(ObjImporter) ERROR: cannot find MaterialServer\n";
            return false;
        }

    RGBA rgba;
    boost::shared_ptr<MaterialSolid> material;

    for (
         TObjMatValueVector::iterator iter = matVector.begin();
         iter != matVector.end();
         ++iter
         )
        {
            // decide whether we need to setup a Material2DTexture or whether
            // a MaterialSolid is sufficient (we only support diffuse textures for now)
            if (iter->diffuseTexture.size())
                {
                    material = shared_dynamic_cast<Material2DTexture>
                        (GetCore()->New("/kerosin/Material2DTexture"));

                    if (material.get() == 0)
                        {
                            GetLog()->Error()
                                << "(ObjImporter) ERROR: could not create new Material2DTexture\n";
                            return false;
                        }

                    GetLog()->Debug() << "(ObjImporter) defining Material2DTexture "
                                      << iter->name << "\n";

                    //we only support diffuse textures for now
                    (shared_dynamic_cast<Material2DTexture>(material))->SetDiffuseTexture(iter->diffuseTexture);
                }
            else
                {
                    material = shared_dynamic_cast<MaterialSolid>
                        (GetCore()->New("/kerosin/MaterialSolid"));

                    if (material.get() == 0)
                        {
                            GetLog()->Error()
                                << "(ObjImporter) ERROR: could not create new MaterialSolid\n";
                            return false;
                        }

                    GetLog()->Debug() << "(ObjImporter) defining MaterialSolid "
                                      << iter->name << "\n";
                }

            material->SetName(iter->name);

            rgba = RGBA(
                        iter->ambient[0],
                        iter->ambient[1],
                        iter->ambient[2],
                        iter->alpha
                        );

            material->SetAmbient(rgba);

            rgba = RGBA(
                        iter->diffuse[0],
                        iter->diffuse[1],
                        iter->diffuse[2],
                        iter->alpha
                        );

            material->SetDiffuse(rgba);

            rgba = RGBA(
                        iter->specular[0],
                        iter->specular[1],
                        iter->specular[2],
                        iter->alpha
                        );

            material->SetSpecular(rgba);

            material->SetShininess(iter->shininess);

            materialServer->RegisterMaterial(material);

            GetLog()->Debug() << "(ObjImporter) Material "
                              << iter->name << " set up and successfully registered.\n";
        }

    GetLog()->Debug() << "(ObjImporter) Material setup finished for material library "
                      << matLibName << ".\n";

    return true;
}
