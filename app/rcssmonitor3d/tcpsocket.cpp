#include "tcpsocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <rcssbase/net/exception.hpp>

#include <iostream>

namespace rcss
{
    namespace net
    {
        TCPSocket::TCPSocket()
        {
            open();
        }

        TCPSocket::TCPSocket( const Addr& addr )
        {
            open();
            bind( addr );
        }

        TCPSocket::TCPSocket( const Addr& addr, const Addr& dest )
        {
            open();
            bind( addr );
            connect( dest );
        }

        void
        TCPSocket::doOpen( int& fd )
        {
            close();
            fd = ::socket(AF_INET, SOCK_STREAM, 0 );
            if( fd < 0 )
                throw OpenErr( errno );
        }
    }
}
