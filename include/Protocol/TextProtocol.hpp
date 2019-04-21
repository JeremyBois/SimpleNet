#ifndef __TEXT_PROTOCOL__HPP
#define __TEXT_PROTOCOL__HPP

#include "Protocol/AbstractProtocol.hpp"

namespace simpleNET
{

    // Base Protocol interface
    class SIMPLENET_API TextProtocol: public AbstractProtocol
    {
        public:
            TextProtocol(SimpleSocket* socket);

            // How protocol is working
            virtual int Send(char* buffer) override;

            virtual int Send(char* buffer,
                             sockaddr* socAdress, socklen_t socAdressLength) override;

            virtual int Receive(char* buffer) override;

            virtual int Receive(char* buffer,
                                sockaddr* socAdress, socklen_t* socAdressLength) override;
    };

} // namespace simpleNET

#endif
