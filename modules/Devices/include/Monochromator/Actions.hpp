/**
 * @file Monochromator/Actions.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Monochromator device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_MONOCHROMATOR_ACTIONS_HPP_
#define MODULES_DEVICES_INCLUDE_MONOCHROMATOR_ACTIONS_HPP_

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

namespace monochromator {

/**
 * @class Actions
 * @brief Class that allows to define the behavior of the Monochromator device in response to specific events during state transitions of the FSM.
 * 
 * The methods of this class encapsulate the action that needs to be performed when executing a specific FSM event.
 */
class Actions {
 public:
  Actions() = delete;
  explicit Actions(std::shared_ptr<IMotor> clientStepper1Linear,
                   std::shared_ptr<IMotor> clientStepper2Rotational,
                   std::shared_ptr<scanning::IScanning> clientScanning1Linear,
                   std::shared_ptr<scanning::IScanning> clientScanning2Rotational,
                   std::shared_ptr<sensors::ISensors> clientSensors,
                   std::shared_ptr<IConfiguration> clientConfiguration,
                   std::shared_ptr<IPostProcessing> clientPostProcessing);
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
   * @return false otherwise.
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
   * @brief Method used to align the Monochromator.
   * 
   * @details
   * 1) If the 'searchCenter_' parameter is true the linear axis is moved to the target position;
   *    If false go to 6).
   * 2) The .csv files are set up;
   * 3) A scan to find the alignment postion of the linear axis is performed 
   *   (to determine the position where the crystal surface interrupts the beam);
   * 4) A double axis scan (linear and rotational) is performed to search for the rotational
   *    axis alignment position (this allows to fine tune the orizontal postion of the monochromator crystal);
   * 5) The new alignment positions are written in the configuration file;
   * 6) The device is moved to the alignment position read from the configuration file;
   * 
   * @return true if the alignment has been executed.
   * @return false otherwise.
   */
  bool alignMonochromator();
  /**
   * @brief Method used to search the Bragg's peak of the monochromator crystal.
   * 
   * @details
   * 1) The rotational axis of the monochromator is moved to the target position;
   * 2) A rotational axis scan is performed;
   * 3) The .csv files are set up;
   * 4) The Bragg's peak position in computed by means of a python script;
   * 5) The Bragg's peak position is saved in the configuration file;
   * 6) The rotational axis is moved to the Bragg's peak position;
   * 
   * @return true if the alignment has been executed.
   * @return false otherwise.
   */
  bool searchMonochromatorBraggPeak();
  /**
   * @brief Setter funtion of 'searchCenter_' parameter.
   * 
   * @param searchCenter boolean value.
   */
  void setSearchCenter(bool searchCenter);
  /**
   * @brief Getter funtion of 'searchCenter_' parameter.
   * 
   * @return boolean value representing the value of the private parameter.
   */
  bool getSearchCenter();
  /**
   * @brief Setter function of 'centerLinearPosition_' parameter.
   * 
   * @param centerPosition float value representing the value of 'centerLinearPosition_'.
   */
  void setCenterPosition(float centerPosition);
  /**
   * @brief Getter function of 'centerLinearPosition_' parameter.
   * 
   * @return float value representing the value of 'centerLinearPosition_'.
   */
  float getCenterPosition();

 private:
  std::shared_ptr<IMotor> clientStepper1Linear_;  /**< Shared pointer to IMotor Class. This pointer controls the motion of the linear stage of the device. */
  std::shared_ptr<IMotor> clientStepper2Rotational_;  /**< Shared pointer to IMotor Class. This pointer controls the motion of the rotational stage of the device. */
  std::shared_ptr<scanning::IScanning> clientScanning1Linear_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<scanning::IScanning> clientScanning2Rotational_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<sensors::ISensors> clientSensors_;  /**< Shared pointer to ISensors Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  std::shared_ptr<IPostProcessing> clientPostProcessing_;  /**< Shared pointer to IPostProcessing Class. */
  float stepper1LinearPosition_;  /**< Parameter that stores the target position of the linear axis of the device. */
  float stepper2RotationalPosition_;  /**< Parameter that stores the target position of the rotational axis of the device. */
  bool searchCenter_;  /**< Parameter that controls whether to compute the alignment position or not. */
  float centerLinearPosition_;  /**< Parameter that stores the alignment position of the linear axis of the device. */
  float centerRotationalPosition_;  /**< Parameter that stores the alignment position of the rotational axis of the device. */
  std::filesystem::path pathToProjDirectory_ = clientSensors_->getPathToProjDirectory();
  std::filesystem::path pathToLogFilesDirectory_ = pathToProjDirectory_ / "LogFiles";  /**< Path to the log file directory. */
  std::filesystem::path pathToScriptDirectory_ = pathToProjDirectory_ / "modules\\Devices\\scripts\\Monochromator";  /**< Path to the directory where the python scripts are stored. */
  std::filesystem::path monochromatorAlignmentResultsDirectoryName_ = "MonochromatorAlignmentResults";
  std::string pathToMonochromatorAlinmentResultsDirectory_;  /**< Path to the directory where the alignment positions are stored in the .csv files. */
};

}  // namespace monochromator

#endif  // MODULES_DEVICES_INCLUDE_MONOCHROMATOR_ACTIONS_HPP_
