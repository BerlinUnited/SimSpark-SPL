#include "staticmesh.h"
#include "../openglserver/openglserver.h"
#include "../materialserver/material.h"
#include "../materialserver/materialserver.h"
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include "helper/NVMeshMender.h"
#include <salt/fileclasses.h>

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;
// using namespace Opcode;

StaticMesh::StaticMesh()
{
        mVertexCount = 0;
        mPos = 0;
        mTexCoords = 0;
        mBasisX = 0;
        mBasisY = 0;
}

StaticMesh::~StaticMesh()
{
        if (mVertexCount)
        {
                delete[] mPos;
                delete[] mTexCoords;
                delete[] mBasisX;
                delete[] mBasisY;
        }
}

bool StaticMesh::Load(const std::string& fileName)
{
        // open file
        shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));
        salt::RFile *file = fileServer->Open(fileName.c_str());

        if (!file) return false;

        NVMeshMender                                    meshmender;
        NVMeshMender::VAVector                  input, output;
        NVMeshMender::VertexAttribute   att;

        char buffer[1024];
        int temp;
        int i;

        printf("Loading '%s'...\n", fileName.c_str());
        file->Gets(buffer, 1024);
        sscanf(buffer, "TotalTriCount: %d", &temp);
        printf("  TotalTriCount: %d\n", temp);

        int vertexCount;
        file->Gets(buffer, 1024);
        sscanf(buffer, "VertexCount:   %d", &vertexCount);
        printf("  VertexCount:   %d\n", vertexCount);

        int meshCount;
        file->Gets(buffer, 1024);
        sscanf(buffer, "MeshCount:     %d", &meshCount);
        printf("  MeshCount:     %d\n", meshCount);
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

        int maxMaterial = 0;
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
                        mPos = new Float3[mVertexCount];
                        for (int j = 0; j < mVertexCount; ++j)
                        {
                                mPos[j].x = output[i].floatVector_[3*j];
                                mPos[j].y = output[i].floatVector_[3*j + 1];
                                mPos[j].z = output[i].floatVector_[3*j + 2];
                        }
                }

                if (output[i].Name_ == "tex0")
                {
                        int size = output[i].floatVector_.size()/3;
                        mTexCoords = new Float3[size];

                        for (int j = 0; j < size; ++j)
                        {
                                mTexCoords[j].x = output[i].floatVector_[3*j];
                                mTexCoords[j].y = output[i].floatVector_[3*j + 1];
                                mTexCoords[j].z = output[i].floatVector_[3*j + 2];
                        }
                }

                if (output[i].Name_ == "tangent")
                {
                        int size = output[i].floatVector_.size()/3;
                        mBasisX = new Float3[size];

                        for (int j = 0; j < size; ++j)
                        {

                                mBasisX[j].x = output[i].floatVector_[3*j];
                                mBasisX[j].y = output[i].floatVector_[3*j + 1];
                                mBasisX[j].z = output[i].floatVector_[3*j + 2];
                        }
                }

                if (output[i].Name_ == "binormal")
                {
                        int size = output[i].floatVector_.size()/3;
                        mBasisY = new Float3[size];

                        for (int j = 0; j < size; ++j)
                        {
                                mBasisY[j].x = output[i].floatVector_[3*j];
                                mBasisY[j].y = output[i].floatVector_[3*j + 1];
                                mBasisY[j].z = output[i].floatVector_[3*j + 2];
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
                mFaces[i] = new Face[mFaceCount[i]];

                int current = 0;
                for (int f = 0; f < totalFaceCount; ++f)
                {
                        if (outMaterial.intVector_[outIndices.intVector_[3*f]*3]==i)
                        {
                                // add face
                                mFaces[i][current].a = outIndices.intVector_[3*f];
                                mFaces[i][current].b = outIndices.intVector_[3*f+1];
                                mFaces[i][current].c = outIndices.intVector_[3*f+2];
                                current++;
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

/*
void StaticMesh::PrepareCollision()
{
        unsigned int i;

        IndexBuffer indexBuffer;
        for(i=0; i<mFaces.size(); ++i)
        {
                indexBuffer.Cache(mFaceCount[i]*3, (unsigned int*)mFaces[i]);
        }

        mCollisionModel = new OPCODE_Model();

        OPCODECREATE OPCC;

        //surface data
        OPCC.NbTris     = indexBuffer.GetNumIndex()/3;
        mOCFaces = new IndexedTriangle[indexBuffer.GetNumIndex()/3];

        for (int l = 0; l < indexBuffer.GetNumIndex(); l+=3)
        {
                mOCFaces[l/3].mVRef[0] = indexBuffer.GetIndex()[l];
                mOCFaces[l/3].mVRef[1] = indexBuffer.GetIndex()[l+1];
                mOCFaces[l/3].mVRef[2] = indexBuffer.GetIndex()[l+2];
        }

        OPCC.NbVerts= mVertexCount;
        OPCC.Tris       = indexBuffer.GetIndex();
        OPCC.Verts  = (IceMaths::Point*)mPos;
        OPCC.Rules      = SPLIT_COMPLETE|SPLIT_SPLATTERPOINTS|SPLIT_GEOMCENTER;
        OPCC.NoLeaf     = true;
        OPCC.Quantized = true;
        bool status = mCollisionModel->Build(OPCC);

        // create our spherecollider
        mSphereCollider = new SphereCollider();
        // we want all contacts
        mSphereCollider->SetFirstContact(false);
        mSphereCollider->SetTemporalCoherence(false);
        mSphereCollider->SetPointers(mOCFaces, (IceMaths::Point*)mPos);

        // create our planescollider
        mPlanesCollider = new PlanesCollider();
        mPlanesCollider->SetFirstContact(false);
        mPlanesCollider->SetTemporalCoherence(false);
        mPlanesCollider->SetPointers(mOCFaces, (IceMaths::Point*)mPos);

        // create our raycollider
        mRayCollider = new RayCollider();
        mRayCollider->SetFirstContact(false);
        mRayCollider->SetTemporalCoherence(false);
        mRayCollider->SetClosestHit(true);
        mRayCollider->SetCulling(true);
        mRayCollider->SetPointers(mOCFaces, (IceMaths::Point*)mPos);
}
*/

/*
void StaticMesh::ConvertMatrix(IceMaths::Matrix4x4& matrix) const
{
        for (int i=0; i<4; ++i)
                for (int j=0; j<4; ++j)
                        matrix.m[j][i] = GetWorldTransform()(i, j);
}
*/

/*
void StaticMesh::FindInfluencedTriangles(Opcode::SphereCache &cache, const Meshmerizer::Sphere &sphere, std::vector<unsigned int> &triangles) const
{
        Matrix4x4 worldMat;
        ConvertMatrix(worldMat);
        mSphereCollider->Collide(cache, sphere, mCollisionModel, 0, &worldMat);
        const int numTriangles  = mSphereCollider->GetNbTouchedPrimitives();
        const udword* colTriangles      = mSphereCollider->GetTouchedPrimitives();

        triangles.clear();
        triangles.reserve(numTriangles);

        for (int i=0; i<numTriangles; ++i)
        {
                triangles.push_back(colTriangles[i]);
        }
}
*/

/*
void StaticMesh::FindInfluencedTriangles(Opcode::PlanesCache &cache, const IceMaths::OPCPlane *planes, int numPlanes, std::vector<unsigned int> &triangles) const
{
        Matrix4x4 worldMat;
        ConvertMatrix(worldMat);
        mPlanesCollider->Collide(cache, planes, numPlanes, mCollisionModel, &worldMat);
        const int numTriangles  = mPlanesCollider->GetNbTouchedPrimitives();
        const udword* colTriangles      = mPlanesCollider->GetTouchedPrimitives();

        triangles.clear();
        triangles.reserve(numTriangles);
        //printf("PlaneCollider: %d\n", numTriangles);
        for (int i=0; i<numTriangles; ++i)
        {
                triangles.push_back(colTriangles[i]);
        }
}
*/

/*
void StaticMesh::FillIndexBuffers(TIndexBuffers &iBuffers, const std::vector<unsigned int> &triangles) const
{
        if (mFaces.size() > iBuffers.size())
        {
                iBuffers.resize(mFaces.size());
        }
        const int n = triangles.size();
        for (int i=0; i<n; ++i)
        {
                iBuffers[mFaceToMaterial[triangles[i]]].Cache(mOCFaces[triangles[i]].mVRef[0]);
                iBuffers[mFaceToMaterial[triangles[i]]].Cache(mOCFaces[triangles[i]].mVRef[1]);
                iBuffers[mFaceToMaterial[triangles[i]]].Cache(mOCFaces[triangles[i]].mVRef[2]);
        }
}
*/

/*
void StaticMesh::Render(const TIndexBuffers &iBuffers) const
{
        glPushMatrix();
        glMultMatrixf(GetWorldTransform().m);

        glVertexPointer(3, GL_FLOAT, 0, mPos);
        glEnableClientState ( GL_VERTEX_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mTexCoords);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE1_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mBasisX);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE2_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mBasisY);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        for (unsigned int i=0; i < mMaterials.size(); ++i)
        {
                if (iBuffers[i].GetNumIndex()>0)
                {
                        mMaterials[i]->Bind();
                        glDrawElements(GL_TRIANGLES, iBuffers[i].GetNumIndex(), GL_UNSIGNED_INT, iBuffers[i].GetIndex());
                }
        }

        glDisableClientState (GL_VERTEX_ARRAY);
        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
        glClientActiveTextureARB ( GL_TEXTURE1_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
        glClientActiveTextureARB ( GL_TEXTURE2_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );

        glPopMatrix();
}
*/

void StaticMesh::ComputeBoundingBox()
{
}

void StaticMesh::CalcBoundingBox()
{
        mLocalBoundingBox.Init();

        const int n = mVertexCount;

        for(int i = 0; i<n; ++i)
        {
                Float3 &v = mPos[i];
                mLocalBoundingBox.Encapsulate(v.x, v.y, v.z);
        }
}

/*
bool StaticMesh::Intersect(const salt::Vector3f& start, const salt::Vector3f& end, salt::Vector3f& ret) const
{
        CollisionFaces cf;

        mRayCollider->SetDestination(&cf);
        OPCRay ray;
        //start.Dump();
        ray.mOrig.Set(start.x(), start.y(), start.z());
        Vector3f dir = end - start;

        mRayCollider->SetMaxDist(dir.Length());
        dir.Normalize();
        //dir.Dump();
        ray.mDir.Set(dir.x(), dir.y(), dir.z());

        Matrix4x4 worldMat;
        ConvertMatrix(worldMat);

        if (mRayCollider->Collide(ray, mCollisionModel, &worldMat, NULL))
        {
                if (cf.GetNbFaces())
                {
                        //printf("%d %f\n", cf.GetNbFaces(), cf.GetFaces()[0].mDistance);
                        ret = start + cf.GetFaces()[0].mDistance*dir;
                        return true;
                }
        }

        return false;
}
*/

void StaticMesh::RenderInternal()
{
    // comment this method, when reenabling fancy lighting

        glVertexPointer(3, GL_FLOAT, 0, mPos);
        glEnableClientState ( GL_VERTEX_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mTexCoords);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE1_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mBasisX);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE2_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mBasisY);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        for (unsigned int i=0; i < mMaterials.size(); ++i)
        {
                mMaterials[i]->Bind();
                glDrawElements(GL_TRIANGLES, mFaceCount[i]*3, GL_UNSIGNED_INT, mFaces[i]);
        }

        glDisableClientState (GL_VERTEX_ARRAY);
        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
        glClientActiveTextureARB ( GL_TEXTURE1_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
        glClientActiveTextureARB ( GL_TEXTURE2_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}

/*
void StaticMesh::RenderAmbientInternal()
{
        glVertexPointer(3, GL_FLOAT, 0, mPos);
        glEnableClientState ( GL_VERTEX_ARRAY );

        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glTexCoordPointer( 3, GL_FLOAT, 0, mTexCoords);
        glEnableClientState ( GL_TEXTURE_COORD_ARRAY );

        for (unsigned int i=0; i < mMaterials.size(); ++i)
        {
                mMaterials[i]->BindAmbient();
                glDrawElements(GL_TRIANGLES, mFaceCount[i]*3, GL_UNSIGNED_INT, mFaces[i]);
        }

        glDisableClientState (GL_VERTEX_ARRAY);
        glClientActiveTextureARB ( GL_TEXTURE0_ARB );
        glDisableClientState ( GL_TEXTURE_COORD_ARRAY );
}
*/
