#ifndef __ABSTRACT_SOCKET__HPP
#define __ABSTRACT_SOCKET__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition


namespace simpleNET
{

    // Base Socket interface
    class SIMPLENET_LOCAL AbstractSocket
    {
        private:
            SOCKET _socketID = INVALID_SOCKET;

            // If _iblockingMode = 0, blocking is enabled;
            // If _iblockingMode != 0, non-blocking mode is enabled.
            u_long _iblockingMode;

        protected:
            // Can only be created by a children instance
            AbstractSocket(SOCKET socketID);

            inline void  SetID(SOCKET socketID) { _socketID = socketID; }
            virtual bool SetAsBlocking(bool shouldBlock);

            // Copy is not allowed to avoid closing wrapped socket
            AbstractSocket(const AbstractSocket &otherSocket)            = delete;
            AbstractSocket &operator=(AbstractSocket const &otherSocket) = delete;

        public:
            AbstractSocket();

            // Add move semantic to transfer ownership
            // only on rvalues preventing object to steal ownership
            // when parameter is a lvalue
            // noexcept needed for STL containers
            AbstractSocket(AbstractSocket &&otherSocket)              noexcept;
            AbstractSocket &operator=(AbstractSocket &&otherSocket)   noexcept;
            void swap(AbstractSocket &other)                          noexcept;

            virtual ~AbstractSocket();

        public:
            virtual void Init();
            virtual void Close();
            bool         MarkAsBlocking();
            bool         MarkAsNonBlocking();
            bool         IsBlocking();

            /// Wrapper around select.
            /// numfds Only needed with berkeley socket (Unix)
            /// Return the number of file descriptors contained in the three descriptor sets
            /// and SOCKET_ERROR on error
            static int Select(int numfds, fd_set *readfds, fd_set *writefds,
                              fd_set *exceptfds, timeval *timeout);

            inline SOCKET GetID() { return _socketID; }
    };

} // namespace simpleNET

#endif
