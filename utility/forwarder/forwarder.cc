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
#include "forwarder.h"
#include "forwarderstreambuf.h"

using namespace std;
using namespace Utility;

static Forwarder Utility::smux(1024);

Forwarder::Forwarder(unsigned int size) :
ostream(new ForwarderStreamBuf(size))
{
}

Forwarder::~Forwarder()
{
    // delete the created streambuf
    delete rdbuf();
}

void 
Forwarder::addStream(std::ostream* stream, unsigned int mask)
{
    getStreamBuf().addStream(stream, mask);
}

bool 
Forwarder::removeStream(const std::ostream* stream)
{
    return getStreamBuf().removeStream(stream);
}
    
unsigned int 
Forwarder::getPriorityMask(const std::ostream* stream) const
{
    return getStreamBuf().getPriorityMask(stream);
}
    
bool 
Forwarder::setPriorityMask(const std::ostream* stream, unsigned int mask)
{
    return getStreamBuf().setPriorityMask(stream, mask);
}

Forwarder& 
Forwarder::priority(unsigned int prio)
{
    getStreamBuf().setCurrentPriority(prio);
    return *this;
}

const ForwarderStreamBuf& 
Forwarder::getStreamBuf() const
{
    ForwarderStreamBuf* streamBuf = dynamic_cast<ForwarderStreamBuf*>(rdbuf());
    return *streamBuf;
}

ForwarderStreamBuf& 
Forwarder::getStreamBuf()
{
    ForwarderStreamBuf* streamBuf = dynamic_cast<ForwarderStreamBuf*>(rdbuf());
    return *streamBuf;
}

