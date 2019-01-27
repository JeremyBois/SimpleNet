#ifndef __SERVER_SOCKET__HPP
#define __SERVER_SOCKET__HPP

#include "Socket/AbstractSocket.hpp"



namespace simpleNET
{

    class SimpleSocket;


    // A socket that listen for connections on a specific port
    class SIMPLENET_API ServerSocket: public AbstractSocket
    {
        public:
            ServerSocket(int port);

        protected:
            void Listen();

        public:
            SimpleSocket Accept();

    };


} // namespace simpleNET

#endif
