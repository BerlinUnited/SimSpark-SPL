/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: logfileserver.h,v 1.4 2006/05/19 07:52:12 jamu Exp $

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
#ifndef _LOGFILESERVER_H
#define _LOGFILESERVER_H

#include "commserverbase.h"
#include "communit.h"
#include <fstream>
#include <stack>

class LogfileServer : public CommServerBase
{
public:
    LogfileServer() ;

    virtual ~LogfileServer() {};

    virtual bool Init(const std::string& parser, const std::string& filename, int dummy);

    virtual void GoBack(int lineCount);
    virtual bool ReadMessage();
    virtual bool ReadMessage(std::string& msg);
    virtual boost::shared_ptr<oxygen::PredicateList> GetPredicates() const;

    virtual void SendPauseCmd();
    virtual void SendRunCmd();

    void Reset();

protected:
    void Parse(const std::string& message);

private:
    //! cache for parsed predicates
    boost::shared_ptr<oxygen::PredicateList> mPredicates;

    //! the parser used for the SExpressions
    boost::shared_ptr<oxygen::BaseParser> mParser;

    //! the logfile name
    std::string mLogfileName;

    //! the logfile
    std::ifstream mLog;

    //! flag if we are paused
    bool mPaused;

    //! line numbers storage
    std::stack<unsigned> linePositions;
};

DECLARE_CLASS(LogfileServer);

#endif // _LOGFILESERVER_H
