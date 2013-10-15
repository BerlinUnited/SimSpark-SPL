/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef OXYGEN_NETBUFFER_H
#define OXYGEN_NETBUFFER_H

#include <rcssnet/addr.hpp>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

/** \class NetBuffer is a buffer that is used to hold the raw network
    stream of data. The NetMessage class is responsible to extract
    meaningful messages.
*/
class OXYGEN_API NetBuffer
{
public:
    NetBuffer() {};
    NetBuffer(rcss::net::Addr addr, const std::string& data)
        : mAddr(addr), mData(data) {}

    /** appends a fragment to the buffer */
    void AddFragment(const std::string& d);

    /** returns true iff the buffer is empty*/
    bool IsEmpty() const;

    /** returns the network address associated with this buffer */
    const rcss::net::Addr& GetAddr() const;

    /** returns the managed buffer */
    std::string& GetData();

protected:
    /** the associated network address */
    rcss::net::Addr mAddr;

    /** the managed buffer */
    std::string mData;
};

} // namespace oxygen

#endif // OXYGEN_NETBUFFER_H
