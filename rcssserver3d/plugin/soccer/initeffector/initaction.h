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
#ifndef INITACTION_H
#define INITACTION_H

#include <oxygen/gamecontrolserver/actionobject.h>


class InitAction : public oxygen::ActionObject
{
public:
    InitAction(const std::string& predicate,
               const std::string& name, int number)
        : ActionObject(predicate), mName(name), mNumber(number) {}
    virtual ~InitAction() {}

    /** @return the requested team name */
    const std::string& GetName() { return mName; }
    /** @return the requested uniform number */
    int GetNumber() { return mNumber; }

protected:
    /** the team name to set */
    std::string mName;
    /** the requested uniform number */
    int mNumber;
};

#endif // INITACTION_H
