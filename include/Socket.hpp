#ifndef __SOCKET__HPP
#define __SOCKET__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition

namespace simpleNET
{

    // Socket interface
    class SIMPLENET_API Socket
    {
        public:
          Socket() = default;
          virtual ~Socket() = default;

        public:
            void Init();
            void Clean();
    };

} // namespace simpleNET

#endif
