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

#endif
