# SimpleNET

**SimpleNET** is a crossplatform Network engine with socket abstraction (low level) and a working in progress
high level interface for client and servers.


# Getting started

```
    #include "simpleNET.hpp"

    namespace Net = simpleNET;


    int main(int argc, char **argv)
    {
        // Init
        Net::SimpleNetInit();

        /// ... Your program goes here

        // Clean
        Net::SimpleNetCleanup();

        return 0;
    }
```
