/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorsystem.h,v 1.1.2.3 2003/11/30 17:01:31 fruit Exp $

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
#ifndef OXYGEN_MONITORSYSTEM_H
#define OXYGEN_MONITORSYSTEM_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace oxygen
{

class MonitorSystem : public zeitgeist::Leaf
{
public:
    MonitorSystem() : Leaf() {}

    virtual ~MonitorSystem() {}

    // one note about std::string. This class is capable of handling
    // binary data (i.e. not NULL terminated strings). Use the
    // std::string(basic_string(const charT* s, size_type n)
    // constructor to construct a string containing binary data and
    // the const charT* data() member to access binary data.

    /** This function is called once for every MonitorSystem. It
     *  should return any header/setup information that is needed.
     */
    virtual std::string GetMonitorHeaderInfo() = 0;

    /** This function will be called periodically to get information
     * about the current state of the world.
     */
    virtual std::string GetMonitorInfo() = 0;

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     */
    virtual void ParseMonitorMessage(std::string /* data */) = 0;
};

DECLARE_ABSTRACTCLASS(MonitorSystem);

} // namespace oxygen


#endif // OXYGEN_MONITORSYSTEM_H
