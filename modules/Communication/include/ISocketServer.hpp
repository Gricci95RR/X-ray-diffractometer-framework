/**
 * @file ISocketServer.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief Interface Class used to control TCP server.
 * @version 0.1
 * @date 2023-03-08
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <optional>
#include <chrono>
#include <memory>
#include <string>

#include "ISocket.hpp"

/**
 * @class ISocketServer
 * @brief Interface Class used to control a TCP server.
 * 
 */
class ISocketServer {
    public: 
        /**
         * @brief Destroy the ISocketServer object.
         * 
         */
        ~ISocketServer() = default;

        /**
         * @brief Opens the server.
         * @return boolean value indicating if the server was successfully opened.
         */
        virtual bool open() = 0;

        /**
         * @brief Closes the server.
         * @return boolean value indicating if the server was successfully closed.
         */
        virtual bool close() = 0;

        /**
        * @brief Start accepting new TCP connections on the acceptor.
        * 
        * This function is called recursively after each new connection is accepted. If an error occurs during
        * the async_accept() operation, the error code is passed as a parameter to the
        * callback function. If there is no error, a new TCPSocket is created for the
        * accepted connection and its open() function is called to establish the
        * connection with the client. Before opening the connection, the OnJoin()
        * function object is called with the new connection as a parameter. When a
        * client leaves the server, the OnLeave() function object is called with the
        * connection as a parameter. The OnClientMessage() function object is called
        * whenever a message is received from the client. This function is called
        * automatically by the open() function and runs indefinitely until the server
        * is stopped.
        */
        virtual void acceptConnection() = 0;
};
