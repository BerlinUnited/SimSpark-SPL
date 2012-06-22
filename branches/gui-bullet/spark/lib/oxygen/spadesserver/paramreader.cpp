/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: paramreader.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "paramreader.h"
#include "paramstorer.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <sstream>

using namespace zeitgeist;
using namespace oxygen;
using namespace boost;
using namespace std;

ParamReader::ParamReader()
  : zeitgeist::Leaf(), spades::EngineParam()
{
  addParamStorer(new oxygen::ParamStorer(this));
}

int ParamReader::readCmdLineArgs(const std::string& key,
                                 int argc, const char* const* argv)
{
    // lookup the ScriptServer
    if (argc < 2)
        {
            return spades::ParamReader::ParamStorer::RR_None;
        }

    // check if varName is valid
    if (! GetScript()->ExistsVariable(key))
        {
            GetLog()->Warning() << "WARNING: (ParamReader) Unknown variable '" << key << "'\n";
            return spades::ParamReader::ParamStorer::RR_FormatErr;
        }

    // take next as the value
    std::string value = * (argv + 1);

    // eval "<key> = <value>" using ruby
    stringstream ss;
    ss << key << "=" << value;

    if (GetScript()->Eval(ss.str()))
        {
            GetLog()->Normal() << "(ParamReader) set '" << key
                                << "' to " << value << "\n";
        }

    return 1;
}


