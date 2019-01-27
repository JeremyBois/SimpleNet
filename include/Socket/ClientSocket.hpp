#ifndef __CLIENT_SOCKET__HPP
#define __CLIENT_SOCKET__HPP

#include "Socket/SimpleSocket.hpp"


namespace simpleNET
{

    // A socket that connect to a server
    class SIMPLENET_API ClientSocket: public SimpleSocket
    {
        public:
            ClientSocket(std::string const& ipAdress, int port);

        protected:
            bool Connect(const addrinfo* adressInfos);
    };


} // namespace simpleNET

#endif
