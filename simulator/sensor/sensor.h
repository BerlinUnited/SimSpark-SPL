// -*-c++-*-

/***************************************************************************
                                   sensor.h
                       Abstract Base Class for Sensors
                             -------------------
    begin                : 2002-09-24
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

#ifndef RCSS3D_SENSOR_SENSOR_H
#define RCSS3D_SENSOR_SENSOR_H

namespace RCSS3D
{
    namespace Sensor
    {
        class Sensor
        {
        public:
            virtual
            void
            newCycle() = 0;
        };
    };
};
