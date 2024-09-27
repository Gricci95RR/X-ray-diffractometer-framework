/**
 * @file ISocket.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief Interface Class used to control TCP sockets.
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

using MessageHandler = std::function<void(std::string)>;
using ErrorHandler = std::function<void()>;

/**
 * @class ISocket
 * @brief Interface Class used to control TCP sockets.
 * 
 */
class ISocket {
    public:
        /**
         * @brief Destroy the ISocket object.
         * 
         */
        ~ISocket() = default;

        /**
         * @brief Method to open a TCP socket.
         * @return true if the socket was correctly open.
         * @return false otherwise.
         */
        virtual bool open(MessageHandler&& messageHandler, ErrorHandler&& errorHandler) = 0;

        /**
         * @brief Close the socket.
         * @return true if the socket was correctly closed.
         * @return false otherwise.
         */
        virtual bool close() = 0;

        /**
         * @brief Writes first element of a queue of strings on the socket.
         * @return true if the message has been sent correctly.
         * @return false otherwise.
         */
        virtual bool write() = 0;

        /**
         * @brief Writes std::string on the socket.
         * 
         * The new string is added to a queue and than sent.
         * @param message string to send.
         * @return true if the message has been sent correctly.
         * @return false otherwise.
         */
        virtual bool write(std::string message) = 0;

        /**
         * @brief Read from the socket.
         * @return true if the message has been read correctly.
         * @return false otherwise.
         */
        virtual bool read() = 0;
};
