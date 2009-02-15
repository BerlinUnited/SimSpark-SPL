#include "md5mesh.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <kerosin/openglserver/openglserver.h>
#include <kerosin/openglserver/openglwrapper.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;
using namespace zeitgeist;

char gCurrentReadLine[1024];

bool ReadLine(const shared_ptr<salt::RFile> &fp)
{
   int pos = 0;

   while (1)
   {
      int inchar = fp->Getc();

      if (inchar == EOF)
      {
         gCurrentReadLine[pos] = 0;
         return (false);
      }

      if (inchar == 13)
      {
         fp->Getc();
         break;
      }

      gCurrentReadLine[pos++] = inchar;
   }

   gCurrentReadLine[pos] = 0;

   return (true);
}

MD5Mesh::MD5Mesh() :
oxygen::BaseNode(), mMeshes(0), mBones(0)
{
}

MD5Mesh::~MD5Mesh()
{
}

bool MD5Mesh::Load(const std::string &name)
{
        shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

        if (fileServer.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not locate FileServer" << endl;
                return false;
        }

        shared_ptr<salt::RFile> file(fileServer->Open(name.c_str()));

        if (file.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not locate file '" << name << "'" << endl;
                return false;
        }

        // Skip header
        ReadLine(file);
        ReadLine(file);

        // ========== BONES ==========
        ReadLine(file);
        if (!memcmp(gCurrentReadLine, "numbones", 8))
        {
                sscanf(gCurrentReadLine, "numbones %d", &mNumBones);
                mBones = new Bone[mNumBones];

                printf("Number of bones: %d\n", mNumBones);

                for (int i = 0; i < mNumBones; i++)
                {
                        Bone& bone = mBones[i];

                        // Start of the bone
                        ReadLine(file);

                        // ========== NAME ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tname \"%s", bone.name);
                        bone.name[strlen(bone.name) - 1] = 0;

                        // ========== POSITION ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tbindpos %f %f %f", &bone.pos[0], &bone.pos[1], &bone.pos[2]);

                        // ========== ROTATION ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tbindmat %f %f %f %f %f %f %f %f %f",
                                &bone.rot[0],
                                &bone.rot[1],
                                &bone.rot[2],
                                &bone.rot[3],
                                &bone.rot[4],
                                &bone.rot[5],
                                &bone.rot[6],
                                &bone.rot[7],
                                &bone.rot[8]);

                        // ========== PARENT ==========
                        ReadLine(file);
                        bone.parent = -1;
                        if (gCurrentReadLine[0] != '}')
                        {
                                char parent[64];
                                sscanf(gCurrentReadLine, "\tparent \"%s", parent);
                                parent[strlen(parent) - 1] = 0;
                                for (int j = 0; j < i; j++)
                                {
                                        if (!strcmp(parent, mBones[j].name))
                                        {
                                                bone.parent = j;
                                                break;
                                        }
                                }
                                ReadLine(file);
                        }

                        // Separating line
                        ReadLine(file);
                }
        }

   // ========== MESHES ==========
   ReadLine(file);
   if (!memcmp(gCurrentReadLine, "nummeshes", 9))
   {
      sscanf(gCurrentReadLine, "nummeshes %d", &mNumMeshes);
      mMeshes = new Mesh[mNumMeshes];

      printf("Number of meshes: %d\n", mNumMeshes);

      ReadLine(file);

      for (int i = 0; i < mNumMeshes; i++)
      {
                  int j;
         //printf("Mesh %d\n", i);

         Mesh& mesh = mMeshes[i];

         // Start of mesh
         ReadLine(file);

         // ========== SHADER ==========
         ReadLine(file);
         char* ptr = gCurrentReadLine;
         while (*ptr != '\"')
            ptr++;
         strcpy(mesh.texture, ptr + 1);
         mesh.texture[strlen(mesh.texture) - 1] = 0;
         ReadLine(file);
         printf("   Shader: %s\n", mesh.texture);

         // ========== VERTICES ==========
         ReadLine(file);
         sscanf(gCurrentReadLine, "\tnumverts %d", &mesh.nb_vertices);
         printf("   Number of vertices: %d\n", mesh.nb_vertices);
         mesh.vertices = new Vertex[mesh.nb_vertices];
         for (j = 0; j < mesh.nb_vertices; j++)
         {
            int index;
            Vertex& vertex = mesh.vertices[j];
            ReadLine(file);
            sscanf(gCurrentReadLine, "\tvert %d %f %f %d %d",
               &index,
               &vertex.u,
               &vertex.v,
               &vertex.weight_index,
               &vertex.weight_count);
            vertex.v = 1 - vertex.v;
         }
         ReadLine(file);

         // ========== TRIANGLES ==========
         ReadLine(file);
         sscanf(gCurrentReadLine, "\tnumtris %d", &mesh.nb_triangles);
         printf("   Number of triangles: %d\n", mesh.nb_triangles);
         mesh.triangles = new Triangle[mesh.nb_triangles];
         for (j = 0; j < mesh.nb_triangles; j++)
         {
            int index;
            Triangle& triangle = mesh.triangles[j];
            ReadLine(file);
            sscanf(gCurrentReadLine, "\ttri %d %d %d %d",
               &index,
               &triangle.a,
               &triangle.b,
               &triangle.c);
         }
         ReadLine(file);

         // ========== WEIGHTS ==========
         ReadLine(file);
         sscanf(gCurrentReadLine, "\tnumweights %d", &mesh.nb_weights);
         printf("   Number of weights: %d\n", mesh.nb_weights);
         mesh.weights = new Weight[mesh.nb_weights];
         for (j = 0; j < mesh.nb_weights; j++)
         {
            int index;
            Weight& weight = mesh.weights[j];
            ReadLine(file);
            sscanf(gCurrentReadLine, "\tweight %d %d %f %f %f %f",
               &index,
               &weight.bone,
               &weight.t,
               &weight.x,
               &weight.y,
               &weight.z);
         }
         ReadLine(file);

         ReadLine(file);
      }
   }


        int i;
        for (i = 0; i < mNumMeshes; i++)
        {
                Mesh& mesh = mMeshes[i];

                for (int j = 0; j < mesh.nb_vertices; j++)
                {
                        Vertex& v = mesh.vertices[j];

                        v.x = 0;
                        v.y = 0;
                        v.z = 0;

                        for (int k = 0; k < v.weight_count; k++)
                        {
                                Weight& w = mesh.weights[v.weight_index + k];

                                Bone& b = mBones[w.bone];

                                v.x += (b.rot[0] * w.x + b.rot[3] * w.y + b.rot[6] * w.z + b.pos[0]) * w.t;
                                v.z += (b.rot[1] * w.x + b.rot[4] * w.y + b.rot[7] * w.z + b.pos[1]) * w.t;
                                v.y += (b.rot[2] * w.x + b.rot[5] * w.y + b.rot[8] * w.z + b.pos[2]) * w.t;
                        }
                }
        }


        CalcBasis();
        return true;
}

bool MD5Mesh::LoadAnimation(const std::string &name)
{
        shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));

        if (fileServer.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not locate FileServer" << endl;
                return false;
        }

        shared_ptr<salt::RFile> file(fileServer->Open(name.c_str()));

        if (file.get() == NULL)
        {
                GetLog()->Error() << "ERROR: Could not locate file '" << name << "'" << endl;
                return false;
        }

        // Skip header
        ReadLine(file);
        ReadLine(file);
        ReadLine(file);
        ReadLine(file);

        // CHANNELS
        if (!memcmp(gCurrentReadLine, "numchannels", 11))
        {
                sscanf(gCurrentReadLine, "numchannels %d", &mNumChannels);
                //printf("Number of Channels: %d\n", mNumChannels);
                mChannels = new Channel[mNumChannels];

                for (int i = 0; i < mNumChannels; i++)
                {
                        char name[64];
                        Channel& channel = mChannels[i];

                        // skip line
                        ReadLine(file);

                        // Start of the channel
                        ReadLine(file);

                        // ========== NAME ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tjoint \"%s", name);
                        name[strlen(name) - 1] = 0;
                        //printf("Bone: %s\n", name);
                        // find bone id
                        channel.boneId = -1;
                        for (int j = 0; j < mNumBones; j++)
                        {
                                if (!strcmp(name, mBones[j].name))
                                {
                                        channel.boneId = j;
                                        break;
                                }
                        }

                        // ========== ATTRIBUTE ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tattribute \"%s", name);
                        name[strlen(name) - 1] = 0;
                        channel.paramId = -1;
                        if (!strcmp(name, "x"))
                                channel.paramId = 0;
                        if (!strcmp(name, "y"))
                                channel.paramId = 1;
                        if (!strcmp(name, "z"))
                                channel.paramId = 2;
                        if (!strcmp(name, "pitch"))
                                channel.paramId = 3;
                        if (!strcmp(name, "yaw"))
                                channel.paramId = 4;
                        if (!strcmp(name, "roll"))
                                channel.paramId = 5;

                        // ========== STARTTIME ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tstarttime %f", &channel.startTime);

                        // ========== ENDTIME ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tendtime %f", &channel.endTime);

                        // ========== FRAMERATE ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tframerate %f", &channel.frameRate);

                        // skip strings
                        ReadLine(file);
                        ReadLine(file);
                        ReadLine(file);

                        // ========== RANGE ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\trange %d %d", &channel.range[0], &channel.range[1]);

                        // ========== KEYS ==========
                        ReadLine(file);
                        sscanf(gCurrentReadLine, "\tkeys %d", &channel.numKeys);
                        //printf("boneId %d, paramId %d - keys %d\n", channel.boneId, channel.paramId, channel.numKeys);
                        channel.keys = new float[channel.numKeys];

                        int keyLeft = channel.numKeys;
                        int k = 0;
                        while (keyLeft > 0)
                        {
                                ReadLine(file);
                                if (keyLeft>=8)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f %f %f %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=8;
                                }else if (keyLeft==7)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f %f %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=7;
                                }else if (keyLeft==6)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=6;
                                }else if (keyLeft==5)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=5;
                                }else  if (keyLeft==4)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=4;
                                }else  if (keyLeft==3)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=3;
                                }else if (keyLeft==2)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f %f",
                                                &channel.keys[k++],
                                                &channel.keys[k++]);
                                        keyLeft-=2;
                                }else  if (keyLeft==1)
                                {
                                        sscanf(gCurrentReadLine, "\t\t %f",
                                                &channel.keys[k++]);
                                        keyLeft-=1;
                                }
                        }

                        // }
                        ReadLine(file);
                }
        }

        return true;
}

void MD5Mesh::RenderInternal()
{
        if (mMeshes!=0)
        {
                const salt::Matrix &mat = GetWorldTransform();

#ifndef WIN32
#warning "Commented out calls to glProgramLocalParameter4fARB"
#endif

#if 0

                glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, mat(0,0), mat(0,1), mat(0,2), mat(0,3));
                glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 1, mat(1,0), mat(1,1), mat(1,2), mat(1,3));
                glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 2, mat(2,0), mat(2,1), mat(2,2), mat(2,3));
                glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 3, mat(3,0), mat(3,1), mat(3,2), mat(3,3));
#endif
                glColor3f(1, 0, 0);
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_LIGHTING);
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glCullFace(GL_BACK);
                glEnable(GL_CULL_FACE);
                glEnableClientState(GL_VERTEX_ARRAY);          // enabling vertex arrays

                static float time = 0.041667f;

                time += 0.001f;
                if (time > 13.375f) time = 0.041667f;

                int i;
                for (i = 0; i < mNumChannels; i++)
                {
                        Channel& channel = mChannels[i];

                        if (channel.boneId != -1 && channel.boneId < mNumBones && channel.paramId != -1)
                        {
                                Bone &bone = mBones[channel.boneId];

                                float baseKey = (time * channel.frameRate - (float)channel.range[0])/(float)channel.range[1]*(float)channel.numKeys;
                                float frac = baseKey - gFloor(baseKey);
                                float val = channel.keys[(int)baseKey]*(1.0f-frac)+channel.keys[((int)baseKey+1)%channel.numKeys]*frac;
                                bone.params[channel.paramId] = val;
                        }
                }

                for (i = 0; i < mNumBones; i++)
                {
                        Bone& b = mBones[i];

                        //b.matrix.Translation(Vector3f(b.params[0], b.params[1], b.params[2]));
                        b.matrix.RotationZ(gDegToRad(b.params[5]));
                        b.matrix.RotateX(gDegToRad(b.params[4]));
                        b.matrix.RotateY(gDegToRad(b.params[3]));
                        b.matrix.Translate(Vector3f(-b.params[0], -b.params[1], -b.params[2]));

                        //if (b.parent != -1)
                        //{
                        //      b.matrix = mBones[b.parent].matrix * b.matrix;
                        //}
                }

                for (i = 0; i < mNumMeshes; i++)
                {
                        Mesh& mesh = mMeshes[i];
                        for (int j = 0; j < mesh.nb_vertices; j++)
                        {
                                Vertex& v = mesh.vertices[j];

                                v.x = 0;
                                v.y = 0;
                                v.z = 0;

                                for (int k = 0; k < v.weight_count; k++)
                                {
                                        Weight& w = mesh.weights[v.weight_index + k];

                                        Bone& b = mBones[w.bone];

                                        v.x += (b.rot[0] * w.x + b.rot[3] * w.y + b.rot[6] * w.z + b.pos[0]) * w.t;
                                        v.z += (b.rot[1] * w.x + b.rot[4] * w.y + b.rot[7] * w.z + b.pos[1]) * w.t;
                                        v.y += (b.rot[2] * w.x + b.rot[5] * w.y + b.rot[8] * w.z + b.pos[2]) * w.t;
                                        //v.x += (b.matrix.m[0] * w.x + b.matrix.m[4] * w.y + b.matrix.m[8] * w.z + b.pos[0]) * w.t;
                                        //v.z += (b.matrix.m[1] * w.x + b.matrix.m[5] * w.y + b.matrix.m[9] * w.z + b.pos[1]) * w.t;
                                        //v.y += (b.matrix.m[2] * w.x + b.matrix.m[6] * w.y + b.matrix.m[10] * w.z + b.pos[2]) * w.t;
                                        /*v.x += (b.matrix.m[0] * w.x + b.matrix.m[4] * w.y + b.matrix.m[8] * w.z + b.matrix.m[12]) * w.t;
                                        v.y += (b.matrix.m[1] * w.x + b.matrix.m[5] * w.y + b.matrix.m[9] * w.z + b.matrix.m[13]) * w.t;
                                        v.z += (b.matrix.m[2] * w.x + b.matrix.m[6] * w.y + b.matrix.m[10] * w.z + b.matrix.m[14]) * w.t;*/
                                        //v.x += (matrix.m[0] * w.x + matrix.m[1] * w.y + matrix.m[2] * w.z + b.pos[0]+b.params[0]) * w.t;
                                        //v.z += (matrix.m[4] * w.x + matrix.m[5] * w.y + matrix.m[6] * w.z + b.pos[1]+b.params[1]) * w.t;
                                        //v.y += (matrix.m[8] * w.x + matrix.m[9] * w.y + matrix.m[10] * w.z + b.pos[2]+b.params[2]) * w.t;
                                }
                        }
                        glBegin(GL_TRIANGLES);
                        for (int j = 0; j < mesh.nb_triangles; ++j)
                        {
                                Triangle &t = mesh.triangles[j];
                                glMultiTexCoord2f(GL_TEXTURE0_ARB, mesh.vertices[t.a].u, -mesh.vertices[t.a].v);
                                glMultiTexCoord3f(GL_TEXTURE1_ARB, mesh.vertices[t.a].tux, mesh.vertices[t.a].tuy, mesh.vertices[t.a].tuz);
                                glMultiTexCoord3f(GL_TEXTURE2_ARB, mesh.vertices[t.a].tvx, mesh.vertices[t.a].tvy, mesh.vertices[t.a].tvz);
                                glVertex3f(mesh.vertices[t.a].x, mesh.vertices[t.a].y, mesh.vertices[t.a].z);
                                glMultiTexCoord2f(GL_TEXTURE0_ARB, mesh.vertices[t.b].u, -mesh.vertices[t.b].v);
                                glMultiTexCoord3f(GL_TEXTURE1_ARB, mesh.vertices[t.b].tux, mesh.vertices[t.b].tuy, mesh.vertices[t.b].tuz);
                                glMultiTexCoord3f(GL_TEXTURE2_ARB, mesh.vertices[t.b].tvx, mesh.vertices[t.b].tvy, mesh.vertices[t.b].tvz);
                                glVertex3f(mesh.vertices[t.b].x, mesh.vertices[t.b].y, mesh.vertices[t.b].z);
                                glMultiTexCoord2f(GL_TEXTURE0_ARB, mesh.vertices[t.c].u, -mesh.vertices[t.c].v);
                                glMultiTexCoord3f(GL_TEXTURE1_ARB, mesh.vertices[t.c].tux, mesh.vertices[t.c].tuy, mesh.vertices[t.c].tuz);
                                glMultiTexCoord3f(GL_TEXTURE2_ARB, mesh.vertices[t.c].tvx, mesh.vertices[t.c].tvy, mesh.vertices[t.c].tvz);
                                glVertex3f(mesh.vertices[t.c].x, mesh.vertices[t.c].y, mesh.vertices[t.c].z);
                        }
                        glEnd();
                }
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
}

void MD5Mesh::CalcBasis()
{
        // generate tangent space
        for (int i = 0; i < mNumMeshes; i++)
        {
                Mesh& mesh = mMeshes[i];

                for (int j = 0; j < mesh.nb_triangles; j++)
                {
                        Triangle& t = mesh.triangles[j];

                        Vector3f edge0;
                        Vector3f edge1;
                        Vector3f sxt;

                        Vertex& v0 = mesh.vertices[t.a];
                        Vertex& v1 = mesh.vertices[t.b];
                        Vertex& v2 = mesh.vertices[t.c];

                        edge0.x() = v1.x - v0.x;
                        edge0.y() = v1.u - v0.u;
                        edge0.z() = v1.v - v0.v;

                        edge1.x() = v2.x - v0.x;
                        edge1.y() = v2.u - v0.u;
                        edge1.z() = v2.v - v0.v;

                        sxt = edge0.Cross(edge1);

                        float dsdx = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dsdx = - sxt.y() / sxt.x();
                        }

                        float dtdx = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dtdx = - sxt.z() / sxt.x();
                        }

                        edge0.x() = v1.y - v0.y;
                        edge1.x() = v2.y - v0.y;

                        sxt = edge0.Cross(edge1);

                        float dsdy = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dsdy = - sxt.y() / sxt.x();
                        }

                        float dtdy = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dtdy = - sxt.z() / sxt.x();
                        }

                        edge0.x() = v1.z - v0.z;
                        edge1.x() = v2.z - v0.z;

                        sxt = edge0.Cross(edge1);

                        float dsdz = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dsdz = - sxt.y() / sxt.x();
                        }

                        float dtdz = 0.0f;
                        if ( fabs( sxt.x() ) > 0.000001f )
                        {
                                dtdz = - sxt.z() / sxt.x();
                        }

                        Vector3f tangentU(dsdx, dsdy, dsdz);
                        Vector3f tangentV(dtdx, dtdy, dtdz);

                        tangentU.Normalize();
                        tangentV.Normalize();

                        Vector3f newNormal = tangentU.Cross(tangentV);

                        v0.tux = v1.tux = v2.tux = tangentU.x();
                        v0.tuy = v1.tuy = v2.tuy = tangentU.y();
                        v0.tuz = v1.tuz = v2.tuz = tangentU.z();
                        v0.tvx = v1.tvx = v2.tvx = tangentV.x();
                        v0.tvy = v1.tvy = v2.tvy = tangentV.y();
                        v0.tvz = v1.tvz = v2.tvz = tangentV.z();
                }
        }
}
