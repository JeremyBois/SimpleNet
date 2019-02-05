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

            // Copy is not allowed
            AbstractSocket(const AbstractSocket &otherSocket)            = delete;
            AbstractSocket &operator=(AbstractSocket const &otherSocket) = delete;

            inline void  SetID(SOCKET socketID) { _socketID = socketID; }
            virtual bool SetAsBlocking(bool shouldBlock);

        public:
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
            bool MarkAsBlocking();
            bool MarkAsNonBlocking();
            bool IsBlocking();

            inline SOCKET GetID() { return _socketID; }
    };

} // namespace simpleNET

#endif
