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

#include "TCPServer.hpp"
#include <iostream>

using asio::ip::tcp;
TCPServer::TCPServer(IPV ipv, int port) : _ipVersion(ipv), _port(port),
    _acceptor(_ioContext, tcp::endpoint(_ipVersion == IPV::V4 ? tcp::v4() : tcp::v6(), _port)) {
    spdlog::info("cTor TCPServer\n");
}

TCPServer::~TCPServer() {
    spdlog::info("dTor TCPServer\n");
    _socket->close();
}

bool TCPServer::open() {
    spdlog::info("method open of class TCPServer\n");
    try {
        this->acceptConnection();
        _ioContext.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool TCPServer::close() {
    spdlog::info("method close of class TCPServer\n");
    try {
      _socket->close();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void TCPServer::Broadcast(const std::string &message) {
    spdlog::info("method Broadcast of class TCPServer\n");
    for (auto& connection : _connections) {
        connection->write(message);
    }
}

void TCPServer::acceptConnection() {
    spdlog::info("method acceptConnection of class TCPServer\n");
    _socket.emplace(_ioContext);
    _acceptor.async_accept(*_socket, [this](const asio::error_code& error){
        auto connection = TCPSocket::Create(std::move(*_socket));  // create new socket
        if (OnJoin) {
            OnJoin(connection);  // call function obj OnJoin
        }
        _connections.insert(connection);
        if (!error) {
            connection->open(  // open connection with socket
                [this](const std::string& message) { if (OnClientMessage) OnClientMessage(message); },  // call function obj OnClientMessage
                [&, weak =std::weak_ptr(connection)] {
                    if (auto shared = weak.lock(); shared && _connections.erase(shared)) {
                        if (OnLeave) OnLeave(shared);  // call function obj OnLeave
                    }
                }
            );
        }
        this->acceptConnection();
    });
}
