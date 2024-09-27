/**
 * @file UIManagementServer.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief This file implements the management server class that allows the communication with the GUI.
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

#include "IUIManagementServer.hpp"
#include "Autocollimator/AutocollimatorDeviceController.hpp"
#include "Crystal/CrystalDeviceController.hpp"
#include "Monochromator/MonochromatorDeviceController.hpp"
#include "XRaySensor/XRaySensorDeviceController.hpp"
#include "XRaySource/XRaySourceDeviceController.hpp"

/**
 * @class UIManagementServer
 * @brief This class allows the communication with the GUI. 
 * 
 */
class UIManagementServer : public IUIManagementServer {
 public:
  /**
   * @brief Construct a new UIManagementServer object.
   * @details
   * 1. Creation of the objects.
   * 2. Initialization of the json to be sent to the client.
   * 3. Definitions of Server Class obj functions.
   * @param port The port number to listen on.
   * @param devicesFactory shared pointer to devicesFactory Class.
   */
  UIManagementServer(int port,
                     std::shared_ptr<IDevicesFactory> devicesFactory);
  /**
   * @brief Destroy the UIManagementServer object.
   * 
   */
  ~UIManagementServer();
  /**
   * @brief Open a new TCP server.
   * 
   * @return true if the server is started.
   * @return false otherwise.
   */
  bool open() override;
  /**
   * @brief Close the TCP server.
   * 
   * @return true if the server is closed.
   * @return false otherwise.
   */
  bool close() override;
  /**
   * @brief Initialize a new TCP server.
   * 
   * @return true if the server is started.
   * @return false otherwise.
   */
  bool initServer() override;
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
  void interpreter() override;
  /**
   * @brief Method used to send a string to the client.
   * @details The message sent informs the client about the states of the FSMs
   * controlling the devices and the positions of the devices.
   * 
   */
  void sendFSMStatus() override;
  /**
   * @brief Handle motor controls based on the JSON configuration file.
   * @details This function processes the JSON configuration file for motor controls
   *          and performs the necessary actions on the motors accordingly.
   * 
   * @param jsonFile The JSON configuration file containing motor control instructions.
   */
  void handleMotorControls(const nlohmann::json& jsonFile);
  /**
   * @brief Handle device scans based on the JSON configuration file.
   * @details This function processes the JSON configuration file for device scans
   *          and initiates the corresponding scans based on the provided parameters.
   * 
   * @param jsonFile The JSON configuration file containing device scan instructions.
   */
  void handleDeviceScans(const nlohmann::json& jsonFile);
  /**
   * @brief Handle beam alignments based on the JSON configuration file.
   * @details This function processes the JSON configuration file for beam alignments
   *          and performs the necessary beam alignment tasks based on the instructions.
   * 
   * @param jsonFile The JSON configuration file containing beam alignment instructions.
   */
  void handleBeamAlignments(const nlohmann::json& jsonFile);
  /**
   * @brief Handle crystal alignments based on the JSON configuration file.
   * @details This function processes the JSON configuration file for crystal alignments
   *          and performs the necessary crystal alignment tasks based on the instructions.
   * 
   * @param jsonFile The JSON configuration file containing crystal alignment instructions.
   */
  void handleCrystalAlignments(const nlohmann::json& jsonFile);
  /**
   * @brief Handle crystal measurements based on the JSON configuration file.
   * @details This function processes the JSON configuration file for crystal measurements
   *          and performs the necessary crystal measurement tasks based on the instructions.
   * 
   * @param jsonFile The JSON configuration file containing crystal measurement instructions.
   */
  void handleCrystalMeasurements(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the xray source motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleXRaySourceRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the monochromator (rotational stage) motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleMonochromatorRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the monochromator (linear stage) motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleMonochromatorLinear(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the slit (rotational stage) motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleSlitRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the slit (linear stage) motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleSlitLinear(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the crystal (rotational stage) motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the hexapod motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleHexapod(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the autocollimator motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleAutocollimatorRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the x-ray detector motion.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleXRayDetectorRotational(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the xray source.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleXRaySourceRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the monochromator (rotational stage).
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleMonochromatorRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the monochromator (linear stage).
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleMonochromatorLinearScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the slit (rotational stage).
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleSlitRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the slit (linear stage).
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleSlitLinearScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of rotational stage of the crystal.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of one of the axes of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleHexapodScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the autocollimator.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleAutocollimatorRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle a scan of the xray detector.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleXRayDetectorRotationalScan(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the alignment of the x-ray source.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleXRaySourceRotationalAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the alignment of the monochromator.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleMonochromatorAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the alignment of the slit.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleSlitAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle X-Axis alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalXAxisAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Y-Axis alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalYAxisAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Y-Axis alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalYAxisFineAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Y/W-Axes alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalYWAxesAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle X-Axis alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalZAxisAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the search of the Bragg Peak of the crystal.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalBraggPeakSearch(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle the check of the alignment in flipped orientation of the crystal.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalCheckAlignmentFlippedOrientation(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle X-Axis alignment of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalXAxisFineAlignment(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Bending Angle measurement.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalBendingAngleMeasurement(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Miscut Angle measurement.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalMiscutAngleMeasurement(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to handle Torsion Angle measurement.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void handleCrystalTorsionAngleMeasurement(const nlohmann::json& jsonFile);
  /**
   * @brief Method used to update Json file to send to GUI with positions of the axes of the hexapod.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void updateJsonToSendWithPositionsHXPAxes();
  /**
   * @brief Method used to update Json file to send to GUI with states of the FSMs.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void updateJsonToSendWithFSMsStates();
  /**
   * @brief Method used to update Json file to send to GUI with positions of the stepper motors.
   * 
   * @param jsonFile The JSON configuration file containing instructions.
   * 
   */
  void updateJsonToSendWithStepperPositions();

 private:
  std::shared_ptr<ISocket> socket_;  /**< Shared pointer of class ISocket. */
  std::shared_ptr<TCPServer> server_;  /**< Shared pointer of class TCPServer. */
  std::string message_;  /**< Message received from the client. */
  std::shared_ptr<IDevicesFactory> devicesFactory_;  /**< Shared pointer of class IDevicesFactory.*/
  std::shared_ptr<ISingleStepperDeviceController> client_XRaySource;  /**< Shared pointer of class ISingleStepperDeviceController.*/
  std::shared_ptr<ICrystalDeviceController> client_Crystal;  /**< Shared pointer of class ICrystalDeviceController.*/
  std::shared_ptr<IMultiStepperDeviceController> client_Monochromator;  /**< Shared pointer of class IMultiStepperDeviceController.*/
  std::shared_ptr<IMultiStepperDeviceController> client_Slit;  /**< Shared pointer of class IMultiStepperDeviceController.*/
  std::shared_ptr<ISingleStepperDeviceController> client_XraySensor;  /**< Shared pointer of class ISingleStepperDeviceController.*/
  nlohmann::json jsonToSend_;  /**< Message to send to the client. */
  std::thread t1_;  /**< Object to detach a new thread. */
  std::thread t_status_;  /**< Object to detach a new thread for status update. */
};
