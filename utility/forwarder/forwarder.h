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
#ifndef UTILITY_FORWARDER_H
#define UTILITY_FORWARDER_H

/*! \class Forwarder
  $Id: forwarder.h,v 1.3 2002/08/16 15:42:29 fruit Exp $
    
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
    enum PriorityLevel 
    {
        S_NONE    = 0,
        S_DEBUG   = 1,
        S_NORMAL  = 2,
        S_WARNING = 4,
        S_ERROR   = 8,
        S_ALL     = 0xffffffff
    };

    // functions
    //
    Forwarder(unsigned int size = 1024);
    virtual ~Forwarder();

    /*! Add a stream to the list of streams.

        First, it is checked if the stream is already in. If the stream
        is found, we only install a new priority mask, so no stream can
        be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
    */
    void addStream(std::ostream* stream, unsigned int mask = S_ALL);
    /*! Remove a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool removeStream(const std::ostream* stream);
    /*! Set priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @param mask     the new priority mask
        @return         true if the stream was found
    */
    bool setPriorityMask(const std::ostream* stream, unsigned int mask);
    /*! Get priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @return         the priority mask; 0 if stream was not found
    */
    unsigned int getPriorityMask(const std::ostream* stream) const;

    //! Priority selection for the messages to be written
    Forwarder& priority(unsigned int prio);

    Forwarder& debug()    { return priority(S_DEBUG); }
    Forwarder& normal()   { return priority(S_NORMAL); }
    Forwarder& warning()  { return priority(S_WARNING); }
    Forwarder& error()    { return priority(S_ERROR); }

private:
    Forwarder(const Forwarder& obj);
    Forwarder& operator=(const Forwarder& obj);

    const ForwarderStreamBuf& getStreamBuf() const;
    ForwarderStreamBuf& getStreamBuf();

};

static Forwarder smux;

} //namespace 

#endif // UTILITY_FORWARDER_H
