/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _FORWARDER_H_
#define _FORWARDER_H_

/*! \class Forwarder
  $Id: forwarder.h,v 1.2 2002/08/13 13:26:19 fruit Exp $
    
    Forwarder

    The Forwarder is a special output stream, which can multiplex
    streamed data to a list of registered streams. It can be used
    anywhere, where a regular std::ostream is used. Most of the
    functionality is handled in ForwarderStreamBuf, the Forwarder
    provides just a comfortable user interface.
    
    HISTORY:
    The forwarder subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Marcus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>

namespace Utility
{

class ForwarderStreamBuf;

class Forwarder : public std::ostream
{
public:
    // types
    //
    enum EPriorityLevel 
    {
        eNone    = 0,
        eDebug   = 1,
        eNormal  = 2,
        eWarning = 4,
        eError   = 8,
        eAll     = 0xffffffff
    };

    // functions
    //
    Forwarder(unsigned int size = 1024);
    virtual ~Forwarder();

    void addStream(std::ostream* stream, unsigned int mask = eAll);

    // Priority Selection
    Forwarder& priority(unsigned int mask);

    Forwarder& debug()    { return priority(eDebug); }
    Forwarder& normal()   { return priority(eNormal); }
    Forwarder& warning()  { return priority(eWarning); }
    Forwarder& error()    { return priority(eError); }

private:
    Forwarder(const Forwarder& obj);
    Forwarder& operator=(const Forwarder& obj);

    ForwarderStreamBuf& getStreamBuf();
};

} //namespace Utility

#endif // _FORWARDER_H_
