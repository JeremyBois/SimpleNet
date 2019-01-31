#ifndef __ABSTRACT_PROTOCOL__HPP
#define __ABSTRACT_PROTOCOL__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition
#include "Socket/SimpleSocket.hpp"

namespace simpleNET
{

    // Base Protocol interface
    class SIMPLENET_LOCAL AbstractProtocol
    {
        protected:
            SimpleSocket& _attachedSocket;

        protected:
            // Can only be created by a children instance
            AbstractProtocol(SimpleSocket& socket);

            // Disable copy
            AbstractProtocol(AbstractProtocol const &otherProtocol)            = delete;
            AbstractProtocol &operator=(AbstractProtocol const &otherProtocol) = delete;



        public:
            // Add move semantic to transfer ownership
            // only on rvalues preventing object to steal ownership
            // when parameter is a lvalue
            // noexcept needed for STL containers
            AbstractProtocol(AbstractProtocol &&otherProtocol)              noexcept;
            AbstractProtocol &operator=(AbstractProtocol &&otherProtocol)   noexcept;
            void swap(AbstractProtocol &other)                              noexcept;

            virtual ~AbstractProtocol() = default;

        public:
            inline SimpleSocket& GetSocket() { return _attachedSocket; }

            // How protocol is working
            virtual int Send(char* buffer);

            virtual int Send(char* buffer,
                             sockaddr* socAdress, socklen_t socAdressLength);

            virtual int Receive(char* buffer);

            virtual int Receive(char* buffer,
                                sockaddr* socAdress, socklen_t* socAdressLength);
    };

} // namespace simpleNET

#endif
