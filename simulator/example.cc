#include "basenode.h"
#include "dirnode.h"
#include "entitytree.h"
#include "simulator.h"
#include "objects/geometrynode.h"

#include <console.h>
#include <forwarder.h>

#include <pthread.h>
#include <drawstuff/drawstuff.h>
#include <set>
#include <string>

using namespace rcss::EntityTree;
using namespace Utility;
using namespace std;

pthread_mutex_t mutex;

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif

void drawTree(BaseNode* node = 0);

static void 
start()
{
    static float xyz[3] = {0.8317f,-0.9817f,0.8000f};
    static float hpr[3] = {121.0000f,-27.5000f,0.0000f};
    dsSetViewpoint(xyz,hpr);
}

static void 
simLoop(int pause)
{
    static rcss::Simulator sim;

    pthread_mutex_lock(&mutex);
    // loop =
    sim.execute();
    pthread_mutex_unlock(&mutex);
    drawTree(0);
}

static void 
command(int cmd)
{
}

void*
simulator_thread(void *)
{
    // setup pointers to drawstuff callback functions
    dsFunctions fn;
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.command = &command;
    fn.stop = 0;
    fn.path_to_textures = "textures";

    // run simulation
    dsSimulationLoop(0,0,352,288,&fn);
}

void*
console_thread(void *)
{
    flush(*Console::instance().getStream());
    smux.addStream(&cout);
    cin.tie(&smux);
    
    string s;
    long int i = 0;
    
    Console::instance().execute("help");
    BaseNode* node;
    
    while (true) 
    {    
        node = EntityTree::instance().getCurrentNode();
        smux.normal() << "console:"
                      << EntityTree::instance().getCurrentNode()->getPath()
                      << " " << i << ") " << flush;

        ++i;
        getline(std::cin,s);
        pthread_mutex_lock(&mutex);
        Console::instance().execute(s);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void 
drawGeom (dGeomID g, const dReal *pos, const dReal *R)
{
    if (!g) return;
    if (!pos) pos = dGeomGetPosition (g);
    if (!R) R = dGeomGetRotation (g);

    int type = dGeomGetClass (g);
    if (type == dBoxClass) {
        dVector3 sides;
        dGeomBoxGetLengths (g,sides);
        dsDrawBox (pos,R,sides);
    }
    else if (type == dSphereClass) {
        dsDrawSphere (pos,R,dGeomSphereGetRadius (g));
    }
    else if (type == dCCylinderClass) {
        dReal radius,length;
        dGeomCCylinderGetParams (g,&radius,&length);
        dsDrawCylinder (pos,R,length,radius);
    }
    else if (type == dGeomTransformClass) {
        dGeomID g2 = dGeomTransformGetGeom (g);
        const dReal *pos2 = dGeomGetPosition (g2);
        const dReal *R2 = dGeomGetRotation (g2);
        dVector3 actual_pos;
        dMatrix3 actual_R;
        dMULTIPLY0_331 (actual_pos,R,pos2);
        actual_pos[0] += pos[0];
        actual_pos[1] += pos[1];
        actual_pos[2] += pos[2];
        dMULTIPLY0_333 (actual_R,R,R2);
        drawGeom (g2,actual_pos,actual_R);
    }
}

void
drawTree(BaseNode* node)
{
    if (node == 0)
        node = EntityTree::instance().getRoot();
    
    DirNode* dir = dynamic_cast<DirNode*>(node);
    if (dir != 0)
    {
        set<string> subnodes;
        dir->leafNames(subnodes);
        for (set<string>::const_iterator i = subnodes.begin();
             i != subnodes.end();
             ++i)
        {
            drawTree(dir->getChild(*i));
        }
        return;
    }
    GeometryNode* geometry = dynamic_cast<GeometryNode*>(node);
    if (geometry != 0)
    {
        drawGeom(geometry->getID(),0,0);
        return;
    }
    return;
}


int
main(int argc, char **argv)
{

    pthread_t p1, p2;
    pthread_mutex_init (&mutex, 0);

    pthread_create(&p1, 0, simulator_thread, 0);
    pthread_create(&p2, 0, console_thread, 0);

    pthread_join(p1, 0);
    pthread_join(p2, 0);

    return 0;
}
