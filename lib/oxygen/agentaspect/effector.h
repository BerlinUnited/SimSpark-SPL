/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: effector.h,v 1.4.4.2 2003/12/10 10:19:48 rollmark Exp $

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
#ifndef OXYGEN_EFFECTOR_H
#define OXYGEN_EFFECTOR_H

// #ifdef HAVE_CONFIG_H
// #include <config.h>
// #endif

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/gamecontrolserver/baseparser.h>

namespace oxygen
{
class ActionObject;

class Effector : public BaseNode
{
public:
    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<ActionObject> action) = 0;

    /** returns the name of the predicate this effector implements.
     */
    virtual std::string GetPredicate() = 0;

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<ActionObject>
    GetActionObject(const BaseParser::TPredicate& predicate) = 0;
};

DECLARE_ABSTRACTCLASS(Effector);

} // namespace oxygen

#endif //OXYGEN_EFFECTOR_H
