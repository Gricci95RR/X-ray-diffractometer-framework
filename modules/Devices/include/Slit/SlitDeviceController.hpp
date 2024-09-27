/**
 * @file SlitDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'IMultiStepperDeviceController'.
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

#include "IMultiStepperDeviceController.hpp"
#include "Slit/System.hpp"
#include "Slit/SlitStatus.hpp"

namespace slit {

/**
 * @class SlitDeviceController
 * @brief Implementation Class of the interface 'IMultiStepperDeviceController'.
 * 
 */
class SlitDeviceController : public IMultiStepperDeviceController {
 public:
  explicit SlitDeviceController(
    std::filesystem::path logFilePath,
    std::shared_ptr<IMotor> clientMotor1Linear,
    std::shared_ptr<IMotor> clientMotor2Rotational,
    std::shared_ptr<scanning::IScanning> clientScanning1Linear,
    std::shared_ptr<scanning::IScanning> clientScanning2Rotational,
    std::shared_ptr<sensors::ISensors> clientSensors,
    std::shared_ptr<IConfiguration> clientConfiguration,
    std::shared_ptr<IPostProcessing> clientPostProcessing);
  ~SlitDeviceController();
  bool start() override;
  bool disconnectStp() override;
  bool goHome() override;
  float getPositionStepperLinear() override;
  float getPositionStepperRotational() override;
  bool logger(std::string filename) override;
  bool moveToPositionStepper1Linear(float position);
  bool moveToPositionStepper2Rotational(float position);
  bool scanStepper1Linear(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) override;
  bool scanStepper2Rotational(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) override;
  bool stop() override;
  std::string getFsmState() override;
  /**
   * @brief This method returns the current status of the Slit device.
   * 
   * @details first checks the current state of the state machine associated with the device using Boost.SML library.
   * Depending on the current state of the state machine, the method sets the corresponding SlitStatus.
   * If the state of the state machine is not recognized or defined, the method sets the status to "NotInitialized".
   * 
   * @return SlitStatus 
   */
  SlitStatus getStatus();
  bool alignSourceWithSensor() override;
  bool alignMonochromator(bool searchCenter) override;
  bool searchMonochromatorBraggPeak() override;
  bool alignSlit(bool alignDevice) override;

 private:
  std::filesystem::path logFilePath_;  /**< Path to the log file directory. */
  SlitStatus slitStatus_;  /**< Object of type 'SlitStatus'. */
  std::shared_ptr<Actions> actionsPtr_;  /**< Shared pointer to the Action Class. */
  std::shared_ptr<scanning::IScanning> scanningPtr1Linear_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<scanning::IScanning> scanningPtr2Rotational_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  boost::sml::sm<System> sMachine_;  /**<  State machine object. */
};

}  // namespace slit
