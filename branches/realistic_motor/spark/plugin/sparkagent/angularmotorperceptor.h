/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef ANGULAR_MOTOR_PERCEPTOR_H
#define ANGULAR_MOTOR_PERCEPTOR_H

#include <oxygen/agentaspect/jointperceptor.h>
#include <oxygen/physicsserver/angularmotor.h>
#include <salt/random.h>

class AngularMotorPerceptor : public oxygen::JointPerceptor<oxygen::AngularMotor>
{
public:
    AngularMotorPerceptor();
    virtual ~AngularMotorPerceptor();

    //! \return true, if valid data is available and false otherwise.
    bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

    void SetSenseAngle(bool v) { mSenseAngle = v; }
    void SetSenseRate(bool v) { mSenseRate = v; }
    void SetSenseTorque(bool v) { mSenseTorque = v; }
    void SetSenseCurrent(bool v) { mSenseCurrent = v; }
    void SetSenseTemperature(bool v) { mSenseTemperature = v; }

protected:
    void InsertAxisAngle(oxygen::Predicate& predicate);
    void InsertAxisRate(oxygen::Predicate& predicate);
    void InsertAxisTorque(oxygen::Predicate& predicate);
    void InsertCurrent(oxygen::Predicate& predicate);
    void InsertTemperature(oxygen::Predicate& predicate);

private:
    bool mSenseAngle;
    bool mSenseRate;
    bool mSenseTorque;
    bool mSenseCurrent;
    bool mSenseTemperature;
    float mLastTemperature;
};

DECLARE_CLASS(AngularMotorPerceptor);

#endif //ANGULAR_MOTOR_PERCEPTOR_H
