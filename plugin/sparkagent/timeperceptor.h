/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: timeperceptor.h,v 1.1 2004/05/07 12:12:40 rollmark Exp $

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
#ifndef TIMEPERCEPTOR_H
#define TIMEPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/simulationserver/simulationserver.h>

class TimePerceptor : public oxygen::Perceptor
{
public:
    TimePerceptor();
    virtual ~TimePerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    virtual void OnLink();
    virtual void OnUnlink();

protected:
    /** cached reference to the SimulationServer */
    boost::shared_ptr<oxygen::SimulationServer> mSimulationServer;
};

DECLARE_CLASS(TimePerceptor);

#endif //TIMEPERCEPTOR_H
