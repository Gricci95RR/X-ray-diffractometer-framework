/**
 * @file IUIManagementServer.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief This file creates the interface of the management server class that allows the communication with the GUI.
 * @version 0.1
 * @date 2023
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <spdlog/spdlog.h>
#include <json.hpp>

#include <string>
#include <thread>
#include <cassert>
#include <memory>
#include <iostream>

#include "ISocket.hpp"
#include "TCPServer.hpp"
#include "IDevicesFactory.hpp"

/**
 * @class IUIManagementServer
 * @brief This interface allows the communication with the GUI. 
 * 
 */
class IUIManagementServer {
 public:
  /**
   * @brief Destroy the IUIManagementServer object.
   * 
   */
  ~IUIManagementServer() = default;
  /**
   * @brief Open a new TCP server.
   * 
   * @return true if the server is started.
   * @return false otherwise.
   */
  virtual bool open() = 0;
  /**
   * @brief Close the TCP server.
   * 
   * @return true if the server is closed.
   * @return false otherwise.
   */
  virtual bool close() = 0;
  /**
   * @brief Initialize a new TCP server.
   * 
   * @return true if the server is started.
   * @return false otherwise.
   */
  virtual bool initServer() = 0;
  /**
   * @brief This method interprets the message stored in the variable 'message_'.
   * @details 
   * 1. The method deserialize the string 'message' in a new 'jsonFile' variable
   *    of type nlohmann::json.
   * 2. The method checks the roots of the json file.
   * 3. The method checks the branches of the json file.
   * 4. The method checks the leafs of the json file and executes methods
   *    of the device controllers based on the value of the leaf.
   * 5. A json file is sent to the client.
   */
  virtual void interpreter() = 0;
  /**
   * @brief Method used to send a string to the client.
   * @details The message sent informs the client about the states of the FSMs
   * controlling the devices and the positions of the devices.
   * 
   */
  virtual void sendFSMStatus() = 0;
};
