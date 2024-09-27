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

#pragma once

#include <asio.hpp>
#include "spdlog/spdlog.h"

#include <cstdlib>
#include <memory>
#include <queue>
#include <sstream>

#include <ISocket.hpp>

using asio::ip::tcp;
namespace io = asio;

/**
 * @class TCPSocket
 * @brief Implementation Class used to control a TCP socket.
 * 
 */
class TCPSocket : public std::enable_shared_from_this<TCPSocket> , public ISocket {
    public:

        explicit TCPSocket(io::ip::tcp::socket&& socket);
        ~TCPSocket();

        using pointer = std::shared_ptr<TCPSocket>;  /**< 'pointer' is used as a type alias for std::shared_ptr<TCPSocket>. */

        /**
         * @brief This function creates a new TCPSocket object from an existing io::ip::tcp::socket object, and returns a smart pointer to the newly created object.
         * 
         * The function creates a new TCPSocket object by calling its constructor with std::move(socket) as the argument,
         * which moves the ownership of the socket object into the TCPSocket object being constructed.
         * 
         * @param socket existing io::ip::tcp::socket object.
         * @return smart pointer to to the newly created object of class 'TCPSocket'.
         */
        static pointer Create(io::ip::tcp::socket&& socket) {
            return pointer(new TCPSocket(std::move(socket)));
        }

        /**
         * @brief This function provides read-only access to the private _username member variable of the class.
         * 
         * @return const reference to a std::string object named _username.
         */
        inline const std::string& GetUsername() const { return _username; }

        /**
         * @brief Returns a reference to the underlying TCP socket object.
         *
         * This function returns a reference to the `tcp::socket` object that is used
         * internally by the class to send and receive data over the network. Clients
         * can use this function to access the socket directly if they need to perform
         * any low-level socket operations.
         *
         * @return A reference to the `tcp::socket` object.
         */
        tcp::socket& Socket() {
            return _socket;
        }

        bool open(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) override;
        bool close() override;
        bool write() override;
        bool write(std::string message);
        bool read() override;

        /**
        @brief Post a message to be sent on the TCP socket.
        @param message The message to be sent on the TCP socket.
        */
        void Post(const std::string& message);

        /**
        @brief Callback method called upon completion of a read operation on the TCP socket.
        @param ec The error code returned by the read operation.
        @param bytesTransferred The number of bytes transferred in the read operation.
        */
        void onRead(asio::error_code ec, size_t bytesTranferred);

        /**
        @brief Callback method called upon completion of a write operation on the TCP socket.
        @param ec The error code returned by the write operation.
        @param bytesTransferred The number of bytes transferred in the write operation.
        */
        void onWrite(asio::error_code ec, size_t bytesTransferred);
        
    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        std::string outGoingMessage_;  /**< String containing the message to be send. */
        io::streambuf _streamBuf {65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
