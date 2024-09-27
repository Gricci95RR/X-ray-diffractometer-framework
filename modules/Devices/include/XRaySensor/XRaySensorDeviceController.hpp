/**
 * @file XRaySensorDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'ISingleStepperDeviceController'.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <sml.hpp>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/rotating_file_sink.h"

#include <string>
#include <cassert>
#include <memory>
#include <filesystem>

#include "ISingleStepperDeviceController.hpp"
#include "XRaySensor/System.hpp"
#include "XRaySensor/XRaySensorStatus.hpp"

namespace xRaySensor {

/**
 * @class XRaySensorDeviceController
 * @brief Implementation Class of the interface 'ISingleStepperDeviceController'.
 * 
 */
class XRaySensorDeviceController : public ISingleStepperDeviceController {
 public:
  explicit XRaySensorDeviceController(
    std::filesystem::path logFilePath,
    std::shared_ptr<IMotor> clientMotor,
    std::shared_ptr<scanning::IScanning> clientScanning,
    std::shared_ptr<IConfiguration> clientConfiguration);
  ~XRaySensorDeviceController();
  bool start() override;
  bool disconnectStp() override;
  bool goHome() override;
  float getPositionStepper() override;
  bool logger(std::string filename) override;
  bool moveToPositionStepper(float position) override;
  bool scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) override;
  bool stop() override;
  std::string getFsmState() override;
  /**
   * @brief This method returns the current status of the XRaySensor device.
   * 
   * @details first checks the current state of the state machine associated with the device using Boost.SML library.
   * Depending on the current state of the state machine, the method sets the corresponding XRaySensorStatus.
   * If the state of the state machine is not recognized or defined, the method sets the status to "NotInitialized".
   * 
   * @return XRaySensorStatus 
   */
  XRaySensorStatus getStatus();
  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * 
   * @details The method set the 'stepperPosition_' to position 0 and executes the event to move the stepper to 
   * the target position.
   * 
   * @param searchCenter parameter ignored.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool alignSourceWithSensor(bool searchCenter) override;
  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * 
   * @details The method set the 'stepperPosition_' to position 0 and executes the event to move the stepper to 
   * the target position.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool setupMonochromatorBraggPeakSearch() override;
  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * 
   * @details The method set the 'stepperPosition_' to the position read in the configuration file
   * and executes the event to move the stepper to the target position.
   * 
   * @param searchCenter parameter ignored.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool setupForCrystalBraggPeakSearch() override;

 private:
  std::filesystem::path logFilePath_;  /**< Path to the log file directory. */
  XRaySensorStatus xRaySensorStatus_;  /**< Object of type 'XRaySensorStatus'*/
  std::shared_ptr<Actions> actionsPtr_;  /**< Shared pointer to Actions Class. */
  std::shared_ptr<scanning::IScanning> scanningPtr_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  boost::sml::sm<System> sMachine_;  /**<  State machine object. */
};

}  // namespace xRaySensor
