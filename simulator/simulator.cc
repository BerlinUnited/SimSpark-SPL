#include "simulator.h"

using namespace rcss;
using namespace std;

Simulator::Simulator()
{
    M_world = dWorldCreate(); 
    M_space = dHashSpaceCreate();
    M_contact_group = dJointGroupCreate(0);
    // below we want to set parameters from a parameter file
    dWorldSetGravity(M_world,0,0,-9.81);
    dWorldSetCFM(M_world,1e-5);
    dCreatePlane(M_space,0,0,1,0);
}

Simulator::~Simulator()
{
    dJointGroupDestroy(M_contact_group);
    dSpaceDestroy(M_space);
    dWorldDestroy(M_world);
}

bool
Simulator::execute()
{
    // execute simulation steps and return false when simulation is over
    return false;
}
