# UI Management Server Module

## Table of Contents
1. [Overview](#overview)
2. [Interface Description](#interface-description)
   - [Class: IUIManagementServer](#class-iuimanagementserver)
     - [Public Methods](#public-methods)
3. [Dependencies](#dependencies)
4. [Licensing](#licensing)
5. [Contact](#contact)

## Overview

This module provides the interface for the UI Management Server class, enabling communication with the GUI. The UI Management Server facilitates the control and monitoring of various devices through a TCP server. It interprets incoming messages from the GUI, executes corresponding actions on the devices, and sends feedback about the states of the devices back to the GUI.

## Interface Description

### Class: IUIManagementServer

#### Public Methods

1. `open()`: Opens a new TCP server for communication.
   - Returns: `true` if the server is successfully started, `false` otherwise.

2. `close()`: Closes the TCP server.
   - Returns: `true` if the server is successfully closed, `false` otherwise.

3. `initServer()`: Initializes the TCP server for communication.
   - Returns: `true` if the server is successfully initialized, `false` otherwise.

4. `interpreter()`: Interprets the incoming message from the GUI.
   - Details: This method deserializes the received message into a JSON format and then processes the roots, branches, and leafs of the JSON data. Based on the leaf values, corresponding methods of the device controllers are executed. It also sends a JSON file containing the current states of the devices back to the client.

5. `sendFSMStatus()`: Sends a message to the client with the states of the Finite State Machines (FSMs) controlling the devices and the current positions of the devices.

## Dependencies

This module has the following dependencies:

- `spdlog`: A fast and lightweight logging library for C++.
- `json.hpp`: A JSON library for modern C++.
- `ISocket.hpp`: Interface for controlling TCP sockets.
- `TCPServer.hpp`: TCP server implementation.
- `IDevicesFactory.hpp`: Interface for creating and managing device controllers.

## Licensing

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.