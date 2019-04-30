/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of rcssserver3D
   Thu Mar 24 2011
   Copyright (C) 2003-1011 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef OXYGEN_BOOSTTIMERSYSTEM_H
#define OXYGEN_BOOSTTIMERSYSTEM_H

#ifndef Q_MOC_RUN
#include <boost/thread/thread.hpp>
#endif
#include <oxygen/simulationserver/timersystem.h>

namespace oxygen
{
/** \class TimerSystemBoost is a timer system based on Boost timing facilities
 */
class TimerSystemBoost: public TimerSystem
{
public:
    /** initialize the timer system. It is called once at the beginning of a
     * simulation.
     */
    virtual void Initialize();

    /** \return the elapsed time since the last call of this function or since
     * the initialization for the first call.
     */
    virtual float GetTimeSinceLastQuery();

    /** waits until \param deadline seconds is passed since the last call to
     * GetTimeSinceLastQuery(). If that is already passed, it'll return
     * immediately.
     */
    virtual void WaitFromLastQueryUntil(float deadline);

    /** this is called at the end of the simulation */
    virtual void Finalize() {}

private:
    /** the last time GetTimeSinceLastQuery is called or zero on Initialize() */
    boost::system_time mLastQueryTime;
};

DECLARE_CLASS(TimerSystemBoost);

} // namespace oxygen

#endif // OXYGEN_BOOSTTIMERSYSTEM_H
