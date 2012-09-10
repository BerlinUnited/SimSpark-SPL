/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef OXYGEN_PARAMETERREADER_H
#define OXYGEN_PARAMETERREADER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <oxygen/oxygen_defines.h>
#include <spades/EngineParam.hpp>

namespace oxygen
{

/*!
 */
class OXYGEN_API ParamReader :
        public zeitgeist::Leaf,
        public spades::EngineParam
{
public:
    ParamReader();

    /** returns the number of args processed otherwise see the spades
        RR_values. This method is called from within the ParamStorer.
    */
     int readCmdLineArgs(const std::string& key,
                                int argc, const char* const* argv);
};

DECLARE_CLASS(ParamReader);

} // namespace oxygen

#endif // OXYGEN_PARAMETERREADER_H
