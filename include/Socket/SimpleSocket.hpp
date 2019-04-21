#ifndef __SIMPLE_SOCKET__HPP
#define __SIMPLE_SOCKET__HPP


#include "Socket/AbstractSocket.hpp"


namespace simpleNET
{

    // Base Socket interface
    class SIMPLENET_API SimpleSocket: public AbstractSocket
    {
        public:
            SimpleSocket();
            SimpleSocket(SOCKET socketID);

        public:
            int Send(char* buffer, int bufferSize);
            int Send(char* buffer, int bufferSize,
                             sockaddr* socAdress, socklen_t socAdressLength);
            int Receive(char* buffer, int bufferSize);
            int Receive(char* buffer, int bufferSize,
                                sockaddr* socAdress, socklen_t* socAdressLength);

        protected:
            virtual int SendImplementation(char* buffer, int bufferSize,
                                           sockaddr* socAdress, socklen_t socAdressLength,
                                           int flags=0);
            virtual int ReceiveImplementation(char* buffer, int bufferSize,
                                              sockaddr* socAdress, socklen_t* socAdressLength,
                                              int flags=0);
    };

} // namespace simpleNET

#endif
