/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserverbase.h,v 1.1 2004/05/11 09:15:14 fruit Exp $

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
#ifndef COMMSERVERBASE_H
#define COMMSERVERBASE_H

#include <zeitgeist/class.h>
#include <oxygen/gamecontrolserver/baseparser.h>

class CommServerBase : public zeitgeist::Leaf
{
public:
    virtual ~CommServerBase() {};

    virtual bool Init(const std::string& parser, const std::string& s, int n) = 0;
    virtual bool ReadMessage() = 0;
    virtual bool ReadMessage(std::string& msg) = 0;
    virtual boost::shared_ptr<oxygen::PredicateList> GetPredicates() const = 0;

    virtual void SendKickOffCmd() = 0;
    virtual void SendTrainerCmd(const std::string& cmd) = 0;
    virtual void SendPauseCmd() = 0;
    virtual void SendRunCmd() = 0;
    virtual void SendDisconnectCmd() = 0;
    virtual void SendToWorldModel(const std::string& msg) = 0;
};

DECLARE_ABSTRACTCLASS(CommServerBase);

#endif // COMMSERVERBASE_H
