#ifndef __CLIENT_SOCKET__HPP
#define __CLIENT_SOCKET__HPP

#include "Socket/SimpleSocket.hpp"


namespace simpleNET
{

    // A socket that connect to a server
    class SIMPLENET_API ClientSocket: public SimpleSocket
    {
        public:
          ClientSocket();
          bool Connect(std::string const &ipAdress, int port);

        protected:
          bool _Connect(const addrinfo *paddrInfos, addrinfo &usedInfo);

    };


} // namespace simpleNET

#endif
