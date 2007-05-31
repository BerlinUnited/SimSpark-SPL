/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simcontrolnode.cpp,v 1.2.6.1 2007/05/31 14:17:03 jboedeck Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "simcontrolnode.h"
#include "simulationserver.h"

using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

SimControlNode::SimControlNode() : Node(),mTime(0),mStep(0.02)
{
}

SimControlNode::~SimControlNode()
{
}

shared_ptr<SimulationServer> SimControlNode::GetSimulationServer()
{
    return shared_static_cast<SimulationServer>
        (make_shared(GetParent()));
}

void SimControlNode::Run()
{
    InitSimulation();
    boost::shared_ptr<SimulationServer> ss = GetSimulationServer();
    
    while ( !ss->isExit() )
    {
        boost::mutex::scoped_lock lock(mMutex);
        while( !ss->isExit() && int(ss->GetTime()*100) < int(mTime*100) )
        {
            //std::cout<<GetName()<<' '<<__FUNCTION__<<" wait "<<ss->GetTime()<<' '<<mTime<<std::endl;
            mCond.wait(lock);
        }
        //std::cout<<GetName()<<' '<<__FUNCTION__<<std::endl;
        StartCycle();
        SenseAgent();
        ActAgent();
        EndCycle();
        mTime = ss->GetTime()+mStep;
        mCond.notify_one();
    }
    
    DoneSimulation();
}

void SimControlNode::Wait(boost::mutex::scoped_lock& lock)
{
    boost::shared_ptr<SimulationServer> ss = GetSimulationServer();
    while ( !ss->isExit() && int(ss->GetTime()*100) >= int(GetTime()*100) )
    {
        //std::cout<<GetName()<<' '<<__FUNCTION__<<' '<<GetTime()<<std::endl;
        mCond.wait(lock);
    }
}
