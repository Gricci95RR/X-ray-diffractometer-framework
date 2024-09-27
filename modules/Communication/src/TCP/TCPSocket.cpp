/**
 * @file TCPSocket.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief Implementation Class used to control TCP sockets.
 * @version 0.1
 * @date 2023-03-08
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "TCPSocket.hpp"
#include <iostream>

TCPSocket::TCPSocket(io::ip::tcp::socket&& socket) : _socket(std::move(socket)) {
    spdlog::info("cTor TCPSocket\n");
    asio::error_code ec;

    std::stringstream name;
    name << _socket.remote_endpoint();

    _username = name.str();
}

TCPSocket::~TCPSocket() {
    spdlog::info("dTor TCPSocket\n");
}

bool TCPSocket::open(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) {
    spdlog::info("method open of Class TCPSocket\n");
    _messageHandler = std::move(messageHandler);
    _errorHandler = std::move(errorHandler);
    this->read();
    return true;
}

bool TCPSocket::close() {
    try {
      _socket.close();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

void TCPSocket::Post(const std::string &message) {
    bool queueIdle = _outgoingMessages.empty();
    _outgoingMessages.push(message);

    if (queueIdle) {
        this->read();
    }
}

bool TCPSocket::read() {
    spdlog::info("method read of Class TCPSocket\n");
    io::async_read_until(_socket, _streamBuf, "\n", [this]
    (asio::error_code ec, size_t bytesTransferred) {
        this->onRead(ec, bytesTransferred);
    });
    return true;
}

void TCPSocket::onRead(asio::error_code ec, size_t bytesTranferred) {
    if (ec) {
        _socket.close(ec);
        _errorHandler();
        return;
    }
    std::stringstream message;
    //message << _username << ": " << std::istream(&_streamBuf).rdbuf();
    message << std::istream(&_streamBuf).rdbuf();
    _streamBuf.consume(bytesTranferred);
    _messageHandler(message.str());
    this->read();
}

bool TCPSocket::write() {
    spdlog::info("method write of Class TCPSocket\n");
    io::async_write(_socket, io::buffer(_outgoingMessages.front()), [this]
            (asio::error_code ec, size_t bytesTransferred) {
        onWrite(ec, bytesTransferred);
    });
    return true;
}

void TCPSocket::onWrite(asio::error_code ec, size_t bytesTransferred) {
    if (ec) {
        _socket.close(ec);
        _errorHandler();
        return;
    }
    _outgoingMessages.pop();
    if (!_outgoingMessages.empty()) {
        this->write();
    }
}

bool TCPSocket::write(std::string message) {
    io::write(_socket, io::buffer(message));
    return true;
}
