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
#ifndef OXYGEN_PARAMSTORER_H__
#define OXYGEN_PARAMSTORER_H__

#include "paramreader.h"
#include <oxygen/oxygen_defines.h>
#include <spades/ParamReader.hpp>

namespace oxygen
{

/** This class implements the spades::ParamReader::ParamStorer
    interface. It's purpose is to enable the user to set ruby
    variables using the command line.

    A note about the implementation: ParamStorer uses the ParamReader
    class to do the real parsing. This has the following reason.

    In order to access the ScriptServer to modify ruby variables the
    ParamStorer must be a ZeitGeist class. This involves that the
    corresponding class object holds a shared pointer to every
    instance of this class.

    However spades is not aware of these shared pointers and destructs
    a registered ParamStorer instance using delete without notifying
    the shared pointers. This would result in dangling references.

    In order to play well with spades this class is not derived from
    Zeitgeist::object. In order to access the ScriptServer this class
    uses the ParamReader instance.
*/
class OXYGEN_API ParamStorer : public spades::ParamReader::ParamStorer
{
public:
    ParamStorer(oxygen::ParamReader* paramReader) :
        spades::ParamReader::ParamStorer(), mParamReader(paramReader) {}

    virtual int readArgsFrom(const std::string& /*key*/, const char* /*fileid*/,
                             const char* /*path*/, std::istream& /*is*/,
                             bool /*have_argument*/)
    {
        // we read nothing from any files
        return RR_None;
    }

    virtual void print(std::ostream& /*o*/) const
    {
        // no output
    }

    // returns the number of args processed
    // otherwise see the RR_ values above
    virtual int readCmdLineArgs(const std::string& key,
                                int argc, const char* const* argv);

protected:
    /** The ParamReader we use to do the parsing */
    oxygen::ParamReader* mParamReader;
};

} // namespace oxygen
#endif // OXYGEN_PARAMSTORER_H__
