/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Sat April 12 2008
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef OXYGEN_BEHAVIOR_H
#define OXYGEN_BEHAVIOR_H

#include <string>
#include <oxygen/oxygen_defines.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/simulationserver/traincontrol.h>

namespace oxygen
{
/** \class Behavior provides an interface for integrated agents'
    register. Any agent which want to run internally should be
    derivated from this class, and overload 'Init' & 'Think'
    to implement its special behavior.
 */
class OXYGEN_API Behavior : public zeitgeist::Leaf
{
public:
    /** called once when the initially connected to the server */
    virtual std::string Init() = 0;

    /** called for every message received from the server; should
        return an action string
    */
    virtual void Think(boost::shared_ptr<oxygen::PredicateList> senselist) = 0;

    /** set command message of behavior */
    void SetCommand(const std::string& command);

    /** return the command message of behavior */
    std::string GetCommand() const;

protected:
    virtual void OnLink();

    virtual void OnUnlink();

protected:
    /** cached reference to the TrainControl */
    CachedPath<TrainControl> mTrainControl;

    /** command message produced by 'Think' */
    std::string mCommand;
};

DECLARE_ABSTRACTCLASS(Behavior);

} // namespace oxygen

#endif // OXYGEN_BEHAVIOR_H
