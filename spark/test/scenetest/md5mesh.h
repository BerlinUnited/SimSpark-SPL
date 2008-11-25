#ifndef ME5MESH_H__
#define ME5MESH_H__

#include <oxygen/sceneserver/basenode.h>
// #include "lightvertexbuffer.h"

struct Bone
{
   char name[64];
   float rot[9];
   float pos[3];
   float params[6];
   salt::Matrix matrix;
   int parent;
};

struct Vertex
{
   float u, v;
   int weight_index;
   int weight_count;
   float x, y, z;
   float nx, ny, nz;
   float tux, tuy, tuz;
   float tvx, tvy, tvz;
};

struct Triangle
{
   int a, b, c;
};

struct Weight
{
   int bone;
   float t;
   float x, y, z;
};

struct Mesh
{
   char texture[512];
   int          nb_vertices;
   Vertex*      vertices;
   int          nb_triangles;
   Triangle*    triangles;
   int          nb_weights;
   Weight*      weights;
};

struct Channel
{
        int boneId;             // the bone, this channel modifies
        int paramId;    // parameter being interpolated (x=0, y, z, pitch, yaw, roll)
        float startTime;
        float endTime;
        float frameRate;
        int range[2];
        int     numKeys;
        float *keys;

        Channel() : boneId(-1), paramId(-1), startTime(0), endTime(0), frameRate(0), numKeys(0), keys(0)        {}
};

class MD5Mesh : public oxygen::BaseNode
{
        //
        // Function
        //
public:
        MD5Mesh();
        ~MD5Mesh();

        bool Load(const std::string &name);
        bool LoadAnimation(const std::string &name);

private:
        virtual void RenderInternal();

        void CalcBasis();

        //
        // Members
        //
private:
        int             mNumMeshes;
        Mesh    *mMeshes;
        int             mNumBones;
        Bone    *mBones;
        int             mNumChannels;
        Channel *mChannels;
};

DECLARE_CLASS(MD5Mesh);

#endif //ME5MESH_H__
