/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: tcpsocket.h,v 1.1 2004/03/09 17:13:33 fruit Exp $

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
#ifndef RCSS_NET_TCPSOCKET_HPP
#define RCSS_NET_TCPSOCKET_HPP

#include <rcssbase/net/socket.hpp>

namespace rcss
{
    namespace net
    {
        class TCPSocket
            : public Socket
        {
        public:
            TCPSocket();
            TCPSocket( const Addr& addr );
            TCPSocket( const Addr& addr, const Addr& dest );

        private:
            void
            doOpen( int& fd );
        };
    }
}

#endif // RCSS_NET_TCPSOCKET_HPP
