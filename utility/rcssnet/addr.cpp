// -*-c++-*-

/***************************************************************************
                          addr.cpp  - A network address class
                             -------------------
    begin                : 07-JAN-2003
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

#include "addr.hpp"
#include "exception.hpp"
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace rcss
{
    namespace net
    {
        const Addr::HostType Addr::BROADCAST = INADDR_BROADCAST;
        const Addr::HostType Addr::ANY = INADDR_ANY;

        class AddrImpl
        {
        public:
            AddrImpl()
                : m_addr()
            { memset( (char *)&m_addr, 0, sizeof( m_addr ) ); }

            AddrImpl( const Addr::AddrType& addr )
                : m_addr( addr )
            {}

            AddrImpl( Addr::PortType port )
            {
                memset( (char *)&m_addr, 0, sizeof( m_addr ) ) ;
                m_addr.sin_family = AF_INET ;
                m_addr.sin_addr.s_addr = htonl( INADDR_ANY );
                m_addr.sin_port = htons( port );
            }
            AddrImpl( Addr::PortType port, Addr::HostType host )
            {
                memset( (char *)&m_addr, 0, sizeof( m_addr ) ) ;
                m_addr.sin_family = AF_INET ;
                m_addr.sin_addr.s_addr = htonl( host );
                m_addr.sin_port = htons( port );
            }

            AddrImpl( Addr::PortType port, const std::string& host )
                : m_host_name( host )
            {
                struct hostent* host_ent 
                    = (struct hostent*)gethostbyname( host.c_str() );
                if( host_ent == NULL )
                    throw HostNotFound( h_errno );
               
                memset( (char *)&m_addr, 0, sizeof( m_addr ) ) ;
                m_addr.sin_family = AF_INET ;
                m_addr.sin_addr.s_addr 
                    = ((struct in_addr *)host_ent->h_addr_list[0])->s_addr;
                m_addr.sin_port = htons( port );
            }

            const Addr::AddrType&
            getAddr() const
            { return m_addr; }

            Addr::PortType
            getPort() const
            { return ntohs( m_addr.sin_port );  }
            
            Addr::HostType
            getHost() const
            { return htonl( m_addr.sin_addr.s_addr ); }

            std::string
            getHostStr() const
            {
                if( m_host_name.empty() )
                {
                    m_host_name = inet_ntoa( m_addr.sin_addr );
                }
                return m_host_name;
            }
        private:
            Addr::AddrType m_addr;
            mutable std::string m_host_name;
        };

        Addr::Addr()
            : m_impl( new AddrImpl() )
        {}

        Addr::Addr( const AddrType& addr )
            : m_impl( new AddrImpl( addr ) )
        {}

        Addr::Addr( PortType port )
            : m_impl( new AddrImpl( port ) )
        {}

        Addr::Addr( PortType port, HostType host )
            : m_impl( new AddrImpl( port, host ) )
        {}

        Addr::Addr( PortType port, const std::string& host )
            : m_impl( new AddrImpl( port, host ) )
        {}

        const Addr::AddrType&
        Addr::getAddr() const
        { return m_impl->getAddr(); }

        Addr::PortType
        Addr::getPort() const
        { return m_impl->getPort(); }

        Addr::HostType
        Addr::getHost() const
        { return m_impl->getHost(); }

        std::string
        Addr::getHostStr() const
        { return m_impl->getHostStr(); }

        bool 
        operator==( const Addr& a,
                    const Addr& b )
        {
            return ( a.getAddr().sin_port == b.getAddr().sin_port
                     && ( a.getAddr().sin_addr.s_addr 
                          == b.getAddr().sin_addr.s_addr ) );
        }

        std::ostream&
        operator<<( std::ostream& o, const rcss::net::Addr& addr )
        { return o << '(' << addr.getPort() << ':' << addr.getHostStr() << ')'; }
    }
}
