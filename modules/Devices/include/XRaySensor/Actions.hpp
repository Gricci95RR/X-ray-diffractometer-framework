/**
 * @file XRaySensor/Actions.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the XRaySensor device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_XRAYSENSOR_ACTIONS_HPP_
#define MODULES_DEVICES_INCLUDE_XRAYSENSOR_ACTIONS_HPP_

#include <spdlog/spdlog.h>

#include <memory>

#include "IMotor.hpp"
#include "IScanning.hpp"
#include "IConfiguration.hpp"
#include "IPostProcessing.hpp"

using namespace scanning;  // NOLINT

namespace xRaySensor {

/**
 * @class Actions
 * @brief Class that allows to define the behavior of the X-Ray Sensor device in response to specific events during state transitions of the FSM.
 * 
 * The methods of this class encapsulate the action that needs to be performed when executing a specific FSM event.
 */
class Actions {
 public:
  Actions() = delete;
  /**
   * @brief Construct a new Actions object.
   * 
   * @param clientStepper shared pointer to IMotor Class.
   * @param clientScanning shared pointer to IScanning Class.
   */
  explicit Actions(std::shared_ptr<IMotor> clientStepper,
                   std::shared_ptr<IScanning> clientScanning);
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
   * @return true if disconnection  was successfull.
   * @return false if disconnection  failed.
   */
  bool disconnect();
  /**
   * @brief Method used to home the stepper motor that controls the movement of the XRaySensor.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool goHome();
  /**
   * @brief Method used to move to a target position the XRaySensor.
   * 
   * @note The target position where the motor will be moved is stored in the private parameter 'stepperPosition_'
   * and must be set before calling this method.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool moveCalibratedMotor();
  /**
   * @brief Method used to start a scan of the stepper motor.
   * 
   * This method calls the method 'scan' of class IScanning.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanStepper();
  /**
   * @brief Method used to stop the motion of the stepper motor.
   * 
   * @return true if the stop was successfull.
   * @return false otherwise.
   */
  bool stop();
  /**
   * @brief Getter function of the private parameter 'stepperPosition_'.
   * 
   * @return float representing the value of the private parameter 'stepperPosition_'.
   */
  float getStepperPosition();
  /**
   * @brief Setter function of the private parameter 'stepperPosition_'.
   * 
   * @param stepperPosition float representing to be set to the private parameter 'stepperPosition_'.
   */
  void setStepperPosition(float stepperPosition);

 private:
  std::shared_ptr<IMotor> clientStepper_;  /**< Shared pointer to IMotor Class. */
  std::shared_ptr<scanning::IScanning> clientScanning_;  /**< Shared pointer to IScanning Class. */
  float stepperPosition_;  /**< Parameter that stores the target position of the stepper motor. */
};

}  // namespace xRaySensor

#endif  // MODULES_DEVICES_INCLUDE_XRAYSENSOR_ACTIONS_HPP_
