/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: predicate.h,v 1.1.2.2 2003/12/25 13:52:57 rollmark Exp $

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
#ifndef OXYGEN_PREDICATE_H
#define OXYGEN_PREDICATE_H

#include <list>
#include <string>
#include <boost/any.hpp>

namespace oxygen
{
/** A \class Predicate encapsulates a predicate name together with its
    list of parameters.

    It is used as an internal representation for commands issued by
    agents and is interpreted by Effector classes. A BaseParser object
    is responsible to translate the received agent commands into a
    list of Predicates. An Effector is therefore independent from the
    command format used between an agent and the simulator.

    Predicates are further used to collect sensor data from the agents
    perceptors. A BaseParser is then used to translate these percepts
    into the format used to describe them to the corresponding
    agent. Again, the Perceptor is independent from the format used to
    transmit its data to the agent.
*/
class Predicate
{
 public:
    /** This declares the list of parameters of a predicate. Boost::Any
        is used as a typesafe container to realize a sequence of
        values of arbitrary types.
     */
    typedef std::list<boost::any> TParameterList;

    /** This declares a list of predicates. It is used as output
        format of the \class BaseParser interface
    */
    typedef std::list<Predicate> TList;

 public:
    std::string name;
    TParameterList parameter;
};

}  // namespace oxygen

#endif // OXYGEN_PREDICATE_H
