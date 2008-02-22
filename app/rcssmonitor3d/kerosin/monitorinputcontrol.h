/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorinputcontrol.h,v 1.2 2008/02/22 16:48:21 hedayat Exp $

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
#include <kerosin/inputserver/inputcontrol.h>
#include <commserver.h>

/** \class MonitorInputControl is an InputControl node that handles
    custom soccer monitor input events, like the KickOff command
 */
class MonitorInputControl : public kerosin::InputControl
{
public:
    /** define user input constants */
    static const int CmdKickOff = CmdUser+1;

public:
    /** handle user defined input constants */
    virtual void ProcessInput(kerosin::Input& input);

protected:
    virtual void OnLink();

protected:
    /** the cached CommServer instance */
    boost::shared_ptr<CommServer> mCommServer;
};

DECLARE_CLASS(MonitorInputControl);
