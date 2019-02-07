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
#ifndef OXYGEN_TIMERSYSTEM_H
#define OXYGEN_TIMERSYSTEM_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace oxygen
{
/** \class TimerSystem defines the interface for an external timer to control
 * the simulation.
 */
class TimerSystem: public zeitgeist::Leaf
{
public:
    /** initialize the timer system. It is called once at the beginning of a
     * simulation.
     */
    virtual void Initialize() = 0;

    /** \return the elapsed time since the last call of this function or since
     * the initialization for the first call.
     */
    virtual float GetTimeSinceLastQuery() = 0;

    /** waits until \param deadline seconds is passed since the last call to
     * GetTimeSinceLastQuery(). If that is already passed, it'll return
     * immediately.
     */
    virtual void WaitFromLastQueryUntil(float deadline) = 0;

    /** this is called at the end of the simulation */
    virtual void Finalize() {}
};

DECLARE_ABSTRACTCLASS(TimerSystem);

} // namespace oxygen

#endif // OXYGEN_TIMERSYSTEM_H
