// -*-c++-*-

/***************************************************************************
               socket.hpp  -  Base newtork socket class
                             -------------------
    begin                : 08-JAN-2003
    copyright            : (C) 2003 by The RoboCup Soccer Server 
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#ifndef RCSS_NET_SOCKET_HPP
#define RCSS_NET_SOCKET_HPP

#include <boost/cstdint.hpp>
#include "addr.hpp"

namespace rcss
{
    namespace net
    {
        class Socket
        {
        public:
            enum CheckingType { CHECK, DONT_CHECK };

            Socket();
            
            virtual
            ~Socket();
            
            void
            open();

            void
            bind( const Addr& addr );
            
            Addr
            getName() const;
        
            void
            connect( const Addr& addr );

            Addr
            getPeer() const;

            void
            close();

            int
            setCloseOnExec( bool on = true );

            int 
            setNonBlocking( bool on = true );

            int 
            setAsync( bool on = true );

            int
            setBroadcast( bool on = true );

            int 
            getFD() const;
            
            bool 
            isOpen() const;
            
            bool 
            isConnected() const;
            
            Addr 
            getDest() const; // deprecated.  Use getPeer instead.

            int 
            send( const char* msg, 
                  size_t len, 
                  const Addr& dest,
                  int flags = 0,
                  CheckingType check = CHECK );

            int 
            send( const char* msg, 
                  size_t len, 
                  int flags = 0,
                  CheckingType check = CHECK );

            int 
            recv( char* msg, 
                  size_t len, 
                  Addr& from,
                  int flags = 0,
                  CheckingType check = CHECK );
            
            int
            recv( char* msg, 
                  size_t len, 
                  int flags = 0,
                  CheckingType check = CHECK );
            
    // The following two methods allow a timeout to be specified.
    // Overall, it's slower than the untimed varients so if you do
    // need to specify a timeout and you just want it the recv to
    // block or not to block, then you are better off setting the
    // socket to blocking or non-blocking and using the version
    // without timeouts.
            int
            recv( int timeout,
                  char* msg, 
                  size_t len, 
                  Addr& from, 
                  int flags = 0 );

            int
            recv( int timeout,
                  char* msg, 
                  size_t len, 
                  int flags = 0 );
            
        private:
            virtual
            void
            doOpen( int& fd ) = 0;

            // not used
            Socket( const Socket& );
            Socket& operator=( const Socket& );

        private:
            int m_socket;
            bool m_open;
            bool m_connected;
            Addr m_dest;
        };
    }
}

#endif
