#ifndef __SIMPLE_SOCKET__HPP
#define __SIMPLE_SOCKET__HPP


#include "Socket/AbstractSocket.hpp"


namespace simpleNET
{

    // Base Socket interface
    class SIMPLENET_API SimpleSocket: public AbstractSocket
    {
        public:
            SimpleSocket(SOCKET socketID);

        public:
            virtual bool Send();
            virtual bool Receive();
    };

} // namespace simpleNET

#endif
