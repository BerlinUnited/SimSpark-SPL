#include "simulator.h"
#include "dirnode.h"
#include "entitytree.h"

#include <iostream>

using namespace rcss;
using namespace std;

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
    cerr << "~Simulator\n";
}

bool
Simulator::execute()
{
    // execute simulation steps and return false when simulation is over
    entity::EntityTree::instance().getRoot()->process();
    return true;
}
