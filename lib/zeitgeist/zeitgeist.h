/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: zeitgeist.h,v 1.5 2004/03/12 08:24:20 rollmark Exp $

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
#ifndef ZEITGEIST_ZEITGEIST_H
#define ZEITGEIST_ZEITGEIST_H

#include "core.h"
#include "corecontext.h"
#include "scriptserver/scriptserver.h"
#include "logserver/logserver.h"
#include <boost/shared_ptr.hpp>

namespace zeitgeist
{
/** This is the main class, which initializes the Zeitgeist framework,
    manages the core and the main core context. It is the basic
    interface to the client code.
*/
class Zeitgeist
{
    //
    // functions
    //
public:
    /** constructs the main core and starts the zeitgeist
        framework. dotName gives the name of the directory in the
        user's home directory, where the default init scripts are
        searched.
    */
    Zeitgeist(std::string dotName);
    ~Zeitgeist();

    /** creates a new corecontext */
    boost::shared_ptr<CoreContext> CreateContext();

    /** returns a pointer to the main core */
    boost::shared_ptr<Core>&  GetCore();

    //
    // members
    //
private:
    /** the main core */
    boost::shared_ptr<Core> mCore;
};

} //namespace zeitgeist

#endif //ZEITGEIST_ZEITGEIST_H
