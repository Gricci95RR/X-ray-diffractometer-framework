/**
 * @file TCPServer.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief Implementation Class used to control TCP server.
 * @version 0.1
 * @date 2023-03-08
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <asio.hpp>
#include "spdlog/spdlog.h"

#include <functional>
#include <optional>
#include <unordered_set>
#include <sstream>

#include <ISocketServer.hpp>
#include <TCPSocket.hpp>

namespace io = asio;

/**
 * @enum IPV
 * @brief Enumeration used to define the type of IP that is used to establish the TCP connection.
 * 
 */
enum class IPV {
    V4, /**< for IPV::V4. */
    V6  /**< for IPV::V6. */
};

/**
 * @class TCPServer
 * @brief Implementation Class used to control a TCP server.
 * 
 */
class TCPServer : public ISocketServer{
    /**
    * @typedef OnJoinHandler.
    * @brief A function type alias for handling the event when a client joins the server.
    *
    * This function type takes a shared pointer to a TCPSocket as its parameter.
    * The function should handle the actions to be taken when a client joins the server.
    * 
    */
    using OnJoinHandler = std::function<void(TCPSocket::pointer)>;

    /**
    * @typedef OnLeaveHandler.
    * @brief A function type alias for handling the event when a client leaves the server.
    * 
    * This function type takes a shared pointer to a TCPSocket as its parameter.
    * The function should handle the actions to be taken when a client leaves the server.
    * 
    */
    using OnLeaveHandler = std::function<void(TCPSocket::pointer)>;

    /**
    * @typedef OnClientMessageHandler
    * @brief A function type alias for handling messages received from clients.
    * 
    * This function type takes a string parameter.
    * The function should handle the actions to be taken when a message is received from a client.
    * 
    */
    using OnClientMessageHandler = std::function<void(std::string)>;

public:
    /**
     * @brief Constructs a new TCPServer object.
     *
     * This constructor creates a new `TCPServer` object and initializes its
     * internal `_acceptor` object with the specified IP version and port number.
     *
     * @param ipv The IP version to use (IPv4 or IPv6).
     * @param port The port number to listen on.
     */
    explicit TCPServer(IPV ipv, int port);
    ~TCPServer();
    bool open() override;
    bool close() override;
    void acceptConnection() override;

    /**
    * @brief Broadcasts a message to all connected clients.
    * 
    * This method sends the given message to all clients currently connected to the server.
    * It iterates through each connection in the _connections vector and calls the write() method
    * to send the message to each client.
    * 
    * @param message The message to broadcast to all clients.
    */
    void Broadcast(const std::string& message);

public:
    OnJoinHandler OnJoin;  /**< Variable of type 'OnJoinHandler' */
    OnLeaveHandler OnLeave;  /**< Variable of type 'OnLeaveHandler' */
    OnClientMessageHandler OnClientMessage;  /**< Variable of type 'OnClientMessageHandler' */

private:
    IPV _ipVersion;
    int _port;

    io::io_context _ioContext;  /**< Instance of type 'io::io_context'. */
    io::ip::tcp::acceptor  _acceptor;  /**< Instance of 'io::ip::tcp::acceptor'. */
    std::optional<io::ip::tcp::socket> _socket; /**< Member that contains an 'io::ip::tcp::socket' object. */
    std::unordered_set<TCPSocket::pointer> _connections {};
    std::vector<std::shared_ptr<ISocket> > connectedClients_;
};
