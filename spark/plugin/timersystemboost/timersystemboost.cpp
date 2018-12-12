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
#include "timersystemboost.h"
#ifndef Q_MOC_RUN
#include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include <zeitgeist/logserver/logserver.h>

#ifdef _MSC_VER
#ifndef Q_MOC_RUN
#include <boost/math/special_functions/round.hpp>
#endif
using boost::math::round;
#endif

using namespace oxygen;

void TimerSystemBoost::Initialize()
{
    mLastQueryTime = boost::get_system_time();
}

float TimerSystemBoost::GetTimeSinceLastQuery()
{
    boost::system_time currentTime = boost::get_system_time();
    boost::posix_time::time_duration timeDiff = currentTime - mLastQueryTime;
    mLastQueryTime = currentTime;
    return timeDiff.total_milliseconds() / 1000.0f;
}

void TimerSystemBoost::WaitFromLastQueryUntil(float deadline)
{
    int milliseconds = round(deadline * 1000);
    boost::thread::sleep(mLastQueryTime +
        boost::posix_time::milliseconds(milliseconds));

//    GetLog()->Debug() << "(TimerSystemBoost) Waiting for " << deadline
//            << " seconds or " << milliseconds << " millisecs\n";
//    GetLog()->Debug() << "CURRENT TIME: " << boost::get_system_time() << '\n';
}

//void TimerSystemBoost::Finalize()
//{
//}
