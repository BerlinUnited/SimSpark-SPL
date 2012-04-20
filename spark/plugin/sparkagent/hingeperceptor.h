/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Nov 8 2005
   Copyright (C) 2005 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef HINGEPERCEPTOR_H
#define HINGEPERCEPTOR_H

#include <oxygen/agentaspect/jointperceptor.h>
#include <oxygen/physicsserver/hingejoint.h>

class HingePerceptor : public oxygen::JointPerceptor<oxygen::HingeJoint>
{
public:
    HingePerceptor();
    virtual ~HingePerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    void InsertAxisAngle(oxygen::Predicate& predicate);
    void InsertAxisRate(oxygen::Predicate& predicate);
    void InsertAxisTorque(oxygen::Predicate& predicate);
};

DECLARE_CLASS(HingePerceptor);

#endif //HINGEPERCEPTOR_H
