#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "basenode.h"
#include "dirnode.h"
#include "entitytree.h"
#include "simulator.h"
#include "objects/geometrynode.h"

#include <console.h>
#include <forwarder.h>

#include <pthread.h>
#include <drawstuff/drawstuff.h>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

#if HAVE_READLINE_READLINE_H
#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace rcss::entity;
using namespace Utility;
using namespace std;

pthread_mutex_t mutex;
rcss::Simulator* sim;

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
    pthread_mutex_lock(&mutex);
    // loop =
    sim->execute();
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
    smux.addStream(&cout, Forwarder::S_NORMAL | Forwarder::S_WARNING |
                   Forwarder::S_ERROR);
    cin.tie(&smux);
    
    string s;
    long int i = 0;
    
    Console::instance().execute("create \"World\" \"world\"");
    Console::instance().execute("selectnode \"world\"");
    Console::instance().execute("create \"Plane\" 0 0 1 0 \"plane\"");
    Console::instance().execute("selectnode \"/\"");
    
    Console::instance().execute("help");
    BaseNode* node;

    ostringstream prompt;
    
    while (!Console::instance().hasQuit())
    {    
        node = EntityTree::instance().getCurrentNode();
        prompt << "console:"
               << EntityTree::instance().getCurrentNode()->getPath()
               << " " << i << ") ";

#if HAVE_READLINE_READLINE_H        
        s = readline(prompt.str().c_str());
#else
        cout << prompt.str() << flush;
        getline(std::cin,s);
#endif
        prompt.str("");
        string::size_type j = s.length();
        while (j > 0 && s[j-1] == ' ') --j;
        s = s.substr(0,j);
        if (!s.empty()) 
        {
            ++i;
            pthread_mutex_lock(&mutex);
            Console::instance().execute(s);
            pthread_mutex_unlock(&mutex);
        }
    }
    cerr << "Console thread finished\n";
    return 0;
}

void 
drawGeom(dGeomID g, const dReal *pos, const dReal *R)
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
    ofstream debug_log("debug.log");
    smux.addStream(&debug_log,Forwarder::S_DEBUG);
    smux.debug() << "rcssserver3D debug log" << endl;

    sim = new rcss::Simulator();
#if 1
    pthread_t p1, p2;
    pthread_mutex_init (&mutex, 0);

    pthread_create(&p1, 0, simulator_thread, 0);
    pthread_create(&p2, 0, console_thread, 0);

//    pthread_join(p1, 0);
    pthread_join(p2, 0);
#else
    console_thread(0);
#endif
    
    delete sim;
    return 0;
}
