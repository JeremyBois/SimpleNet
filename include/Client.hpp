#ifndef __CLIENT__HPP
#define __CLIENT__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition

namespace simpleNET
{

    // Client interface
    class SIMPLENET_API Client
    {
        public:
          Client() = default;
          virtual ~Client() = default;
    };

} // namespace simpleNET


#endif
