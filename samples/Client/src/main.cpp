#include "simpleNET.hpp"
#include <iostream>

#include <cereal/archives/binary.hpp>
#include <sstream>




struct MyClass
{
    public:
        int x, y, z;
    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive & archive)
    {
      archive( x, y, z ); // serialize things by passing them to the archive
    }
};

void TestCereal()
{
    std::cout << "\nSTART Cereal Test" << std::endl;
    // Test Cereal
    std::stringstream ss; // any stream can be used

    // RAII --> Data guaranteed  to flush on destruction --> create scope to force destruction
    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

        MyClass m1, m2, m3;
        m1.x = 23;
        m2.y = 444;

        oarchive(m1, m2, m3); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive

        MyClass m1, m2, m3;
        iarchive(m1, m2, m3); // Read the data from the archive

        std::cout << m1.x << " -- " << m2.y << std::endl;
    }

    std::cout << "END Cereal Test \n" << std::endl;
}


namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Test Cereal
    TestCereal();


    // Init
    Net::SimpleNetInit();

    // Test purpose
    std::cout << "I'm the CLIENT" << std::endl;

    // Test lib import
    Net::Client aClient;
    Net::Server aServer;

    // Try to connect to a website
    // Net::ClientSocket mySocket("www.example.com", 7777);


    // SELECT
    char buffer[2048] = "Bonjour Server.";

    // Connect to local server (server must be start first)
    Net::ClientSocket mySocket;
    if (mySocket.Connect("127.0.0.1", 7777))
    {
        // Using Protocol
        Net::TextProtocol myProto(mySocket);

        myProto.Send(buffer);
        printf("Send: %s \n", buffer);

        myProto.Receive(buffer);
        printf("Receive: %s \n", buffer);
    }

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
