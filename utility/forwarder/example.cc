/* -*-c++-*- ***************************************************************
                                  example.cc
    example file for basic forwarder usage
                             -------------------
    begin                : Aug 16 2002 Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/
 
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#include <iostream>

#include "forwarder.h"

using namespace Utility;
using namespace std;

int
main()
{
    // smux is a global object derived from ostream in the Utility namespace
    // (like cout or cerr in the std namespace).

    smux.addStream(&cout, Forwarder::S_ALL);
    smux.addStream(&cerr, Forwarder::S_ALL);

    smux.normal() << "Normal: Output #1" << endl;
    smux << "Normal: Output #2" << endl;

    smux.setPriorityMask(&cout, Forwarder::S_NORMAL);

    smux.warning() << "Warning: Output #3" << endl;
    smux << "Warning: Output #4" << endl;

    smux.removeStream(&cerr);
    smux.setPriorityMask(&cout, Forwarder::S_ALL);

    smux << "Warning: Output #5" << endl;
    smux << "Warning: cout priority mask set to: " 
         << smux.getPriorityMask(&cout) << endl;
}
