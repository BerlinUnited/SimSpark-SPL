/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.h,v 1.6 2004/05/10 14:10:45 fruit Exp $

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
#ifndef _COMMSERVER_H
#define _COMMSERVER_H

#include "communit.h"

#include <zeitgeist/class.h>
#include <oxygen/gamecontrolserver/baseparser.h>

class CommServer : public zeitgeist::Leaf
{
public:

public:
    CommServer();
    virtual ~CommServer() {};

    bool Init(const std::string& parser, const std::string& host, int port);
    bool ReadMessage();
    bool ReadMessage(std::string& msg);
    boost::shared_ptr<oxygen::PredicateList> GetPredicates() const;

    void SendKickOffCmd();
    void SendTrainerCmd(const std::string& cmd);
    void SendPauseCmd();
    void SendRunCmd();
    void SendDisconnectCmd();
    void SendToWorldModel(const std::string& msg);

protected:
    void Parse(const std::string& message);
    void SendMessage(const std::string& message);

protected:
    //! socket wrapper
    CommUnit mCommUnit;

    //! cache for parsed predicates
    boost::shared_ptr<oxygen::PredicateList> mPredicates;

    //! the parser used for the SExpressions
    boost::shared_ptr<oxygen::BaseParser> mParser;
};

DECLARE_CLASS(CommServer);

#endif // _COMMSERVER_H
