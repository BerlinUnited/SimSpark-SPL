/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: behavior.h,v 1.1.2.2 2004/02/08 14:01:46 rollmark Exp $

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
#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <zeitgeist/object.h>
#include "worldmodel.h"
#include "commserver.h"

class Behavior : public zeitgeist::Object
{
public:
    Behavior();
    virtual ~Behavior();

    void SetWorldModel(boost::shared_ptr<WorldModel> wm);
    void SetCommServer(boost::shared_ptr<CommServer> comm);

    void ProcessInitMessage();
    void ProcessThinkTimeMessage();

    virtual void Think();

    static std::string DoneThinkingMessage() { return "D"; }
    static std::string DoneInitMessage() { return "I"; }

protected:
    /** a reference to the WorldModel */
    boost::shared_ptr<WorldModel> mWM;

    /** a reference to the CommServer */
    boost::shared_ptr<CommServer> mComm;
};

DECLARE_CLASS(Behavior)

#endif // BEHAVIOR_H
