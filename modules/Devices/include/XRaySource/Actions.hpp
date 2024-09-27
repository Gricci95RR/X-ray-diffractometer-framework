/**
 * @file XRaySource/Actions.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the XRaySource device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_XRAYSOURCE_ACTIONS_HPP_
#define MODULES_DEVICES_INCLUDE_XRAYSOURCE_ACTIONS_HPP_

#include <spdlog/spdlog.h>

#include <memory>
#include <filesystem>

#include "IMotor.hpp"
#include "IScanning.hpp"
#include "ISensors.hpp"
#include "IConfiguration.hpp"
#include "IPostProcessing.hpp"

using namespace scanning;  // NOLINT
using namespace sensors;  // NOLINT

namespace xRaySource {

/**
 * @class Actions
 * @brief Class that allows to define the behavior of the X-Ray Source device in response to specific events during state transitions of the FSM.
 * 
 * The methods of this class encapsulate the action that needs to be performed when executing a specific FSM event.
 */
class Actions {
 public:
  Actions() = delete;
  /**
   * @brief Construct a new Actions object.
   * 
   * @param clientStepper shared pointer to Class IMotor.
   * @param clientScanning shared pointer to Class IScanning.
   * @param clientSensors shared pointer to Class ISensors.
   * @param clientConfiguration shared pointer to Class IConfiguration.
   * @param clientPostProcessing shared pointer to Class IPostProcessing.
   */
  explicit Actions(std::shared_ptr<IMotor> clientStepper,
                   std::shared_ptr<scanning::IScanning> clientScanning,
                   std::shared_ptr<sensors::ISensors> clientSensors,
                   std::shared_ptr<IConfiguration> clientConfiguration,
                   std::shared_ptr<IPostProcessing> clientPostProcessing);
  /**
   * @brief Destroy the Actions object.
   * 
   */
  ~Actions();
  /**
   * @brief Method used to connect and calibrate the stepper motor that controls the movement of the device.
   * 
   * @return true if connection was successfull.
   * @return false if connection failed.
   */
  bool connect();
  /**
   * @brief Method used to disconnect the stepper motor that controls the movement of the device.
   * 
   * @return true if the disconnection was successfull.
   * @return false if the disconnection failed.
   */
  bool disconnect();
  /**
   * @brief Method used to home the stepper motor that controls the movement of the device.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool goHome();
  /**
   * @brief Method used to move to a specific position the stepper motor that controls the movement of the device.
   * 
   * @note The target position where the motor will be moved is stored in the private parameter 'stepperPosition_'
   * and must be set before calling this method.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool moveCalibratedMotor();
  /**
   * @brief Method used to start a scan of the device.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanStepper();
  /**
   * @brief Method used to stop the motiron of the device.
   * 
   * @return true if the stop was successfull.
   * @return false otherwise.
   */
  bool stop();
  /**
   * @brief Getter funtion of the stepper motor position private parameter ('stepperPosition_').
   * 
   * @return float representing the value of the stepper motor position.
   */
  float getStepperPosition();
  /**
   * @brief Setter funtion of the stepper motor position private parameter ('stepperPosition_').
   * 
   * @param stepperPosition 
   */
  void setStepperPosition(float stepperPosition);
  bool alignSourceWithSensor();
  bool searchBraggsPeakMonochromator();
  void setSearchCenter(bool searchCenter);
  bool getSearchCenter();
  void setCenterPosition(float centerPosition);
  float getCenterPosition();

 private:
  std::shared_ptr<IMotor> clientStepper_;  /**< Shared pointer to IMotor Class. */
  std::shared_ptr<scanning::IScanning> clientScanning_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<sensors::ISensors> clientSensors_;  /**< Shared pointer to ISensors Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  std::shared_ptr<IPostProcessing> clientPostProcessing_;  /**< Shared pointer to IPostProcessing Class. */
  float stepperPosition_;  /**< Parameter that stores the target position of the stepper motor. */
  bool searchCenter_;   /**< Parameter that controls whether to compute the alignment position or not. */
  float centerPosition_;  /**< Parameter that stores the alignment position fo the device. */
  std::filesystem::path pathToProjDirectory_ = clientSensors_->getPathToProjDirectory();
  std::filesystem::path pathToLogFilesDirectory_ = pathToProjDirectory_ / "LogFiles";  /**< Path to the directory where the log files are stored. */
  std::filesystem::path pathToScriptDirectory_ = pathToProjDirectory_ / "modules\\Devices\\scripts\\XRaySource";  /**< Path to the directory where the python scripts are stored. */
  std::filesystem::path xRaySourceAlignmentResultsDirectoryName_ = "XRaySourceAlignmentResults";
  std::string pathToXRaySourceAlinmentResultsDirectory_;  /**< Path to the directory where the alignment positions are stored in the .csv files. */
};

}  // namespace xRaySource

#endif  // MODULES_DEVICES_INCLUDE_XRAYSOURCE_ACTIONS_HPP_
