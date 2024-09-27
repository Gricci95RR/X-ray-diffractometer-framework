/**
 * @file AutocollimatorDeviceController.hpp
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
#include "Autocollimator/System.hpp"
#include "Autocollimator/AutocollimatorStatus.hpp"

namespace autocollimator {

/**
 * @class AutocollimatorDeviceController
 * @brief Implementation Class of the interface 'ISingleStepperDeviceController'.
 * 
 * This Class is used to lauch the execution of the events of the FSM.
 * 
 */
class AutocollimatorDeviceController : public ISingleStepperDeviceController {
 public:
  /**
  * @brief Construct a new Autocollimator Device Controller object.
  * 
  * @param logFilePath path to the log file where to save the output logs.
  * @param clientMotor shared pointer of class IHXP.
  * @param clientScanning shared pointer of class IScanning.
  */
  explicit AutocollimatorDeviceController(std::filesystem::path logFilePath,
                                          std::shared_ptr<IMotor> clientMotor,
                                          std::shared_ptr<scanning::IScanning> clientScanning);

  /**
   * @brief Destroy the Autocollimator Device Controller object.
   * 
   */
  ~AutocollimatorDeviceController();
  bool start() override;
  bool disconnectStp() override;
  bool goHome() override;
  float getPositionStepper() override;
  bool logger(std::string filename) override;
  bool moveToPositionStepper(float position) override;
  bool scanStepper(float stepSize,
                   float range,
                   int durationAcquisition,
                   std::string filename,
                   bool eraseCsvContent,
                   bool showPlot) override;
  bool stop() override;
  std::string getFsmState() override;
  /**
   * @brief This method returns the current status of the Autocollimator device.
   * 
   * @details first checks the current state of the state machine associated with the device using Boost.SML library.
   * Depending on the current state of the state machine, the method sets the corresponding AutocollimatorStatus.
   * If the state of the state machine is not recognized or defined, the method sets the status to "NotInitialized".
   * 
   * @return AutocollimatorStatus 
   */
  AutocollimatorStatus getStatus();

  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * The event will move the device to position 0 deg.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool alignSourceWithSensor(bool searchCenter) override;

  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * The event will move the device to position 0 deg.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool setupMonochromatorBraggPeakSearch() override;

  /**
   * @brief Method used to process the event to go to the FSM state 'systemConnected'.
   * 
   * The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
   * The event will move the device to position 0 deg.
   * 
   * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
   * @return true otherwise.
   */
  bool setupForCrystalBraggPeakSearch() override;

 private:
  std::filesystem::path logFilePath_;  /**< Path to the file where the log file is saved. */
  AutocollimatorStatus autocollimatorStatus_; /**< Object of type 'AutocollimatorStatus'. */
  std::shared_ptr<Actions> actionsPtr_;  /**< Shared pointer to Class Actions. */
  std::shared_ptr<scanning::IScanning> scanningPtr_;  /**< Shared pointer to Class IScanning. */
  boost::sml::sm<System> sMachine_;  /**<  State machine object. */
};

}  // namespace autocollimator
