/* -*-c++-*- ***************************************************************
                             simulator.h
    the base soccer 3d simulator
                           ------------------------
    begin                : Sep 24 2002  Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/
#ifndef RCSS_SIMULATOR_H
#define RCSS_SIMULATOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ode/ode.h>

namespace rcss
{ //} start rcss namespace

/*! \class Simulator
  $Id: simulator.h,v 1.1 2002/09/25 21:02:12 fruit Exp $

    Simulator

*/
class Simulator
{
public:
    Simulator();
    ~Simulator();
    
    bool execute();
    
protected:
    dWorldID M_world;
    dSpaceID M_space;
    dJointGroupID M_contact_group;
};

} // end namespace

#endif                          // RCSS_SIMULATOR_H
