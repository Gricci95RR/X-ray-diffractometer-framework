/**
 * @file Slit/Actions.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Slit device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */


#ifndef MODULES_DEVICES_INCLUDE_SLIT_ACTIONS_HPP_
#define MODULES_DEVICES_INCLUDE_SLIT_ACTIONS_HPP_

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

namespace slit {

/**
 * @class Actions
 * @brief Class that allows to define the behavior of the Slit device in response to specific events during state transitions of the FSM.
 * 
 * The methods of this class encapsulate the action that needs to be performed when executing a specific FSM event.
 */
class Actions {
 public:
  Actions() = delete;
  /**
   * @brief Construct a new Actions object.
   * 
   * @param clientStepper1Linear shared pointer to IMotor Class.
   * @param clientStepper2Rotational shared pointer to IMotor Class.
   * @param clientScanning1Linear shared pointer to IScanning Class.
   * @param clientScanning2Rotational shared pointer to IScanning Class.
   * @param clientSensors shared pointer to ISensors Class.
   * @param clientConfiguration shared pointer to IConfiguration Class.
   * @param clientPostProcessing shared pointer to IPostProcessing Class.
   */
  explicit Actions(std::shared_ptr<IMotor> clientStepper1Linear,
                   std::shared_ptr<IMotor> clientStepper2Rotational,
                   std::shared_ptr<scanning::IScanning> clientScanning1Linear,
                   std::shared_ptr<scanning::IScanning> clientScanning2Rotational,
                   std::shared_ptr<sensors::ISensors> clientSensors,
                   std::shared_ptr<IConfiguration> clientConfiguration,
                   std::shared_ptr<IPostProcessing> clientPostProcessing);
  /**
   * @brief Destroy the Actions object.
   * 
   */
  ~Actions();
  /**
   * @brief Method used to connect and calibrate the stepper motors.
   * 
   * @return true if connection was successfull.
   * @return false if connection failed.
   */
  bool connect();
  /**
   * @brief Method used to disconnect the stepper motors.
   * 
   * @return true if the disconnection was successfull.
   * @return false if the disconnection failed.
   */
  bool disconnect();
  /**
   * @brief Method used to home the stepper motors.
   * 
   * @return true if the movement was successfull.
   * @return false if the movement failed.
   */
  bool goHome();
  /**
   * @brief This function is used to move the linear axis of the device a target position.
   * 
   * @note The target position is stored in the private parameter 'stepper1LinearPosition_'.
   * 
   * @return true if the movement has been executer correctly.
   * @return false otherwisw.
   */
  bool moveCalibratedMotor1Linear();
  /**
   * @brief This function is used to move the rotational axis of the device a target position.
   * 
   * @note The target position is stored in the private parameter 'stepper2RotationalPosition_'.
   * 
   * @return true if the movement has been executer correctly.
   * @return false otherwisw.
   */
  bool moveCalibratedMotor2Rotational();
  /**
   * @brief Method used to start a scan of the stepper motor that controls the linear stage of the device.
   * 
   * This method calls the method 'scan' of class IScanning.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanStepper1Linear();
  /**
   * @brief Method used to start a scan of the stepper motor that controls the rotational stage of the device.
   * 
   * This method calls the method 'scan' of class IScanning.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanStepper2Rotational();
  /**
   * @brief Method used to stop a scan of the two stepper motors.
   * 
   * @return true if the stop was successfull.
   * @return false otherwise.
   */
  bool stop();
  /**
   * @brief Get the position (in mm) of the stepper motor that controls the linear motion of the device.
   * 
   * The method calls the funtion 'getPositionUserUnits' of Class XIMC.
   * 
   * @return float 
   */
  float getStepper1LinearPosition();
  /**
   * @brief Setter function of the parameter 'stepper1LinearPosition_'.
   * 
   * @param stepperPosition float representing the value of the parameter 'stepper1LinearPosition_'.
   */
  void setStepper1LinearPosition(float stepperPosition);
  /**
   * @brief Get the position (in deg) of the stepper motor that controls the rotational motion of the device.
   * 
   * The method calls the funtion 'getPositionUserUnits' of Class XIMC.
   * 
   * @return float 
   */
  float getStepper2RotationalPosition();
  /**
   * @brief Setter function of the parameter 'stepper2RotationalPosition_'.
   * 
   * @param stepperPosition float representing the value of the parameter 'stepper2RotationalPosition_'.
   */
  void setStepper2RotationalPosition(float stepperPosition);
  /**
   * @brief Method that moves the linear and rotational stage of the device to the target positions.
   * 
   * @note The target positions are stored in the parameters 'stepper1LinearPosition_' and
   * 'stepper2RotationalPosition_' and must be set before calling this method.
   * 
   * @return true if the movement has been executed.
   * @return false otherwise.
   */
  bool moveBothMotors();
  /**
   * @brief This function is used to align the slit device on the rotational and linear axis.
   * 
   * @details
   * 1. Check if the 'alignDevice_' parameter is true (if false go to 7.);
   * 2. Move motors to initial positions;
   * 3. Set up csv files;
   * 4. Read rotational axis scan parameters;
   * 5. For every step of the rotational axis perform a scan on the linear axis;
   * 6. Read alignment positions and write to configuration file.
   * 7. Move devices to alignement postions.
   * 
   * @return true if the alignment has been executed.
   * @return false otherwis.
   */
  bool alignDevice();
  /**
   * @brief Get the 'alignDevice_' parameter.
   * 
   * @return 'alignDevice_' boolean parameter.
   */
  bool getAlignDevice();
  /**
   * @brief Set the 'alignDevice_' parameter.
   * 
   * @param alignDevice boolean value.
   */
  void setAlignDevice(bool alignDevice);

 private:
  std::shared_ptr<IMotor> clientStepper1Linear_;
  std::shared_ptr<IMotor> clientStepper2Rotational_;
  std::shared_ptr<scanning::IScanning> clientScanning1Linear_;
  std::shared_ptr<scanning::IScanning> clientScanning2Rotational_;
  std::shared_ptr<sensors::ISensors> clientSensors_;
  std::shared_ptr<IConfiguration> clientConfiguration_;
  std::shared_ptr<IPostProcessing> clientPostProcessing_;
  float stepper1LinearPosition_;
  float stepper2RotationalPosition_;
  bool alignDevice_;
  float centerLinearPosition_;
  float centerRotationalPosition_;
  std::filesystem::path pathToProjDirectory_ = clientSensors_->getPathToProjDirectory();
  std::filesystem::path pathToLogFilesDirectory_ = pathToProjDirectory_ / "LogFiles";
  std::filesystem::path pathToScriptDirectory = pathToProjDirectory_ / "modules\\Devices\\scripts\\Slit";
  std::filesystem::path slitAlignmentResultsDirectoryName_ = "SlitAlignmentResults";
  std::string pathToSlitAlinmentResultsDirectory_;
};

}  // namespace slit

#endif  // MODULES_DEVICES_INCLUDE_SLIT_ACTIONS_HPP_
