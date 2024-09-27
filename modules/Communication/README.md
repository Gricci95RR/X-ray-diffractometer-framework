# Socket Communication Module

This module contains two interfaces, `ISocket` and `ISocketServer`, used for controlling TCP sockets and TCP servers, respectively. These interfaces provide a standardized way to manage network communication and facilitate the exchange of data between clients and servers.

## Table of Contents

- [Introduction](#socket-communication-interfaces)
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [License](#license)
- [Dependencies](#dependencies)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contact](#contact)

## Description

### `ISocket` Interface

The `ISocket` interface is used to control TCP sockets. It provides methods to open, close, read, and write data to a TCP socket. Users can implement this interface to create concrete socket classes for specific applications. The `ISocket` interface defines the following functions:

- `open`: Method to open a TCP socket.
- `close`: Close the socket.
- `write`: Write data to the socket.
- `read`: Read data from the socket.

### `ISocketServer` Interface

The `ISocketServer` interface is used to control a TCP server. It extends the functionalities of the `ISocket` interface to handle server-related operations, such as accepting new connections and managing client interactions. Users can implement this interface to create concrete server classes for various network applications. The `ISocketServer` interface defines the following functions:

- `open`: Opens the server.
- `close`: Closes the server.
- `acceptConnection`: Start accepting new TCP connections on the server.

The `ISocketServer` interface also relies on the `ISocket` interface to handle the communication with connected clients.

## License

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Dependencies

- C++ Standard Library
- `<functional>` header for using `std::function`
- `<optional>` header for handling optional return values
- `<chrono>` header for time-related functionality
- `<memory>` header for managing smart pointers
- Asio C++ library (https://github.com/chriskohlhoff/asio)

## Usage

To use these interfaces, include the corresponding header files, `ISocket.hpp` and `ISocketServer.hpp`, in your C++ project. Implement classes that inherit from the interfaces and provide concrete implementations for the virtual functions.

Here's an example of how to use the `ISocket` and `ISocketServer` interfaces:

```cpp
#include "ISocket.hpp"
#include "ISocketServer.hpp"
#include <iostream>

// Implement the ISocket interface
class MySocket : public ISocket {
public:
    // Implement the virtual functions of the ISocket interface
    // ...

    // Example usage:
    void sendData() {
        if (open(/* MessageHandler */, /* ErrorHandler */)) {
            std::cout << "Socket opened successfully!" << std::endl;
            write("Hello, Server!");
            read();
            close();
        } else {
            std::cout << "Failed to open socket." << std::endl;
        }
    }
};

// Implement the ISocketServer interface
class MySocketServer : public ISocketServer {
public:
    // Implement the virtual functions of the ISocketServer interface
    // ...

    // Example usage:
    void startServer() {
        if (open()) {
            std::cout << "Server opened successfully!" << std::endl;
            acceptConnection();
            close();
        } else {
            std::cout << "Failed to open server." << std::endl;
        }
    }
};

int main() {
    MySocket clientSocket;
    MySocketServer serverSocket;

    // Start the server and client interactions
    serverSocket.startServer();
    clientSocket.sendData();

    return 0;
}
```

## API Documentation

The detailed API documentation for each function can be found in the `ISocket.hpp` and `ISocketServer.hpp` files as part of the Doxygen comments.

## Contributing

Contributions to this project are welcome. To contribute, please follow the standard GitHub workflow of fork, branch, commit, and pull request.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.