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
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace zeitgeist;
using namespace salt;
using namespace oxygen;
using namespace std;
using namespace boost;

ObjImporter::ObjImporter()
    : MeshImporter()
{
}

ObjImporter::~ObjImporter()
{
}

shared_ptr<TriMesh> ObjImporter::ImportMesh
(const string& name, const ParameterList& parameter)
{
    // open file
    string fileName;

    if (! GetFile()->LocateResource(name, fileName))
        {
            GetLog()->Error()
                << "(ObjImporter) ERROR: cannot locate file '" << name << "'\n";
            return shared_ptr<TriMesh>();
        }

    ifstream ifs;
    ifs.open(fileName.c_str());

    if (! ifs)
        {
            GetLog()->Error()
                << "(ObjImporter) ERROR: cannot open file '" << name << "'\n";
            return shared_ptr<TriMesh>();
        }

    // buffers to hold lines from the file
    string buffer, templine;

    // skip first 2 lines (comment)
    getline(ifs, buffer);
    getline(ifs, buffer);

    // skip material lib info, we take care of this in a later version ;-)
    getline(ifs, buffer);

    // skip object name
    getline(ifs, buffer);

    // we need to store vertices, texture coords, vertex normals,
    // and material (texture) names
    vector<Vector3f> pos, tex, norm;
    vector<string> materials;
    TFaceIndexList faces;

    GetLog()->Normal() << "(ObjImporter) Loading " << name << "\n";

    float x,y,z;
    string mat, f1, f2, f3;
    VTNIndex idx1, idx2, idx3;
    FaceIndex tempFace;

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
                    tex.push_back(Vector3f(x, y, 0.0));

                }
            else if (buffer == "vn") // vertex normal
                {
                    str >> x >> y >> z;
                    norm.push_back(Vector3f(x, y, z));
                }
            else if (buffer == "usemtl") // material
                {
                    str >> mat;

                    // cut away the 'Material_' part
                    mat = mat.substr(9, mat.length() - 9);
                    materials.push_back(mat);
                }
            else if (buffer == "f") // face
                {
                    // three indeces for each face:
                    // vertex pos / texture coord / normal
                    str >> f1 >> f2 >> f3;

                    string del("/");
                    vector<string> exploded;
                    ExplodeString(f1, exploded, del);

                    idx1.v  = atoi(exploded[0].c_str());
                    idx1.vt = atoi(exploded[1].c_str());
                    idx1.vn = atoi(exploded[2].c_str());

                    exploded.clear();
                    ExplodeString(f2, exploded, del);

                    idx2.v  = atoi(exploded[0].c_str());
                    idx2.vt = atoi(exploded[1].c_str());
                    idx2.vn = atoi(exploded[2].c_str());

                    exploded.clear();
                    ExplodeString(f3, exploded, del);

                    idx3.v  = atoi(exploded[0].c_str());
                    idx3.vt = atoi(exploded[1].c_str());
                    idx3.vn = atoi(exploded[2].c_str());

                    exploded.clear();

                    tempFace.vertex[0] = idx1;
                    tempFace.vertex[1] = idx2;
                    tempFace.vertex[2] = idx3;

                    faces.push_back(tempFace);
                }
        }

    // close the file
    ifs.close();

    // Now we should have all the vertex positions, texture coordinates,
    // normals, faces, and material names. These need to be converted into
    // shared arrays for the respective TriMesh methods. We need to use the
    // information in the face list to put the right information to the
    // correct indeces in the arrays.

    shared_array<float> posArray(new float[faces.size() * 9]);
    shared_array<float> texArray(new float[faces.size() * 9]);
    shared_array<float> normArray(new float[faces.size() * 9]);

    // store the vertex indices in an IndexBuffer
    shared_ptr<IndexBuffer> indeces(new IndexBuffer());
    indeces->EnsureFit(faces.size() * 3);

    shared_ptr<TriMesh> triMesh(new TriMesh());

    unsigned int faceCount = 0;

    // loop over the face list and setup the new arrays
    for (
         TFaceIndexList::iterator iter = faces.begin();
         iter != faces.end();
         ++iter
         )
        {
            unsigned int j = 0;

            for (int i = 0; i < 3; ++i)
                {
                    j = iter->vertex[i].v - 1;

                    posArray[faceCount * 9 + i * 3]      = pos[j].x();
                    posArray[faceCount * 9 + i * 3 + 1]  = pos[j].y();
                    posArray[faceCount * 9 + i * 3 + 2]  = pos[j].z();

                    // cache vertex index
                    indeces->Cache(faceCount * 3 + i);

                    j = iter->vertex[i].vt - 1;

                    texArray[faceCount * 9 + i * 3]      = tex[j].x();
                    texArray[faceCount * 9 + i * 3 + 1]  = tex[j].y();
                    texArray[faceCount * 9 + i * 3 + 2]  = tex[j].z();

                    j = iter->vertex[i].vn - 1;

                    normArray[faceCount * 9 + i * 3]     = norm[j].x();
                    normArray[faceCount * 9 + i * 3 + 1] = norm[j].y();
                    normArray[faceCount * 9 + i * 3 + 2] = norm[j].z();
                }

            // increase counter
            ++faceCount;
        }

    // add faces (only 1 material allowed right now!)
    triMesh->AddFace(indeces, materials[0]);

    triMesh->SetPos(posArray, faces.size() * 3);
    triMesh->SetTexCoords(texArray);
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
            pos2 = input.find(del, pos1);
        }

    output.push_back(input.substr(pos1, input.length() - pos1));
}
