# SimpleNET

**SimpleNET** is a crossplatform Network engine managing sockets in a non-blocking way allowing for
a large number of connections simultaneously.


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
