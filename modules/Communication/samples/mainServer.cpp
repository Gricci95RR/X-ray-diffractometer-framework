#include <iostream>
#include <vector>
#include <cstring>
#include <direct.h>
#include <filesystem>

#include <TCPServer.hpp>

int main(int argc, char* argv[]) {
    //std::shared_ptr<TCPServer> serverTCP =
    //        std::make_shared<TCPServer>(1234);

    //serverTCP->run();

    TCPServer server {IPV::V4, 1337};

    server.OnJoin = [](TCPSocket::pointer server) {
        std::cout << "User has joined the server: " << server->GetUsername() << std::endl;
    };

    server.OnLeave = [](TCPSocket::pointer server) {
        std::cout << "User has left the server: " << server->GetUsername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string& message) {
        // Parse the message
        // Do game server things
        std::cout << "User message: " << message << std::endl;
        // Send message to client
        server.Broadcast(message);
    };

    server.open();

    return 0;
}
