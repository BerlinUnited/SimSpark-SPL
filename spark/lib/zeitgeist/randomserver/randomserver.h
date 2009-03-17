/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef ZEITGEIST_RANDOMSERVER_H
#define ZEITGEIST_RANDOMSERVER_H

#include <salt/random.h>
#include <zeitgeist/node.h>

namespace zeitgeist
{

/** The random server provides access to the salt RandomEngine from with the
 *  zeitgeist framework. Additionally, it provides interfaces to set a seed
 *  and access some distributions.
 */
class ZEITGEIST_API RandomServer : public Node
{
public:
    /** constructs a RandomServer */
    RandomServer() {}

    virtual ~RandomServer()  {}

    /** set a random seed */
    void Seed(salt::RandomEngine::result_type seed);

    /** get a uniformly distributed random number */
    template<class RealType>
    RealType GetUniformRandom(RealType min, RealType max) const
    {
        return salt::UniformRNG<RealType>(min,max)();
    }

    template<class RealType>
    RealType GetNormalRandom(RealType mean, RealType sigma) const
    {
        return salt::NormalRNG<RealType>(mean,sigma)();
    }

    template<class RealType>
    RealType GetExponentialRandom(RealType lambda) const
    {
        return salt::ExponentialRNG<RealType>(lambda)();
    }

};

DECLARE_CLASS(RandomServer);

} //namespace zeitgeist

#endif //ZEITGEIST_LOGSERVER_H
