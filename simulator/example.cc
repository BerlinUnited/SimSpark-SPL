#include "simulator.h"

int
main(int argc, char **argv)
{
    rcss::Simulator sim;

    while (sim.execute());
    
    return 0;
}
