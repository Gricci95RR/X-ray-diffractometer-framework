/**
 * @file XIMC.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Implementation of "IMotor" used to control the stepper motors.
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <string>

#include "IMotor.hpp"

#include <ximc.h>

/**
 * @class XIMC
 * @brief Implementation of Class IMotor used to control stepper motors with the use of the "ximc.h" library.
 * 
 */
class XIMC: public IMotor {
 private:
  std::string ipAddress_;  /**< Stepper motors controller IP Address. Value stored in ../ConfigurationFiles/config.ini*/
  std::string motorIndex_;  /**< Stepper motor index. Value stored in ../ConfigurationFiles/config.ini*/
  float ratio_;  /**< Stepper motor ratio (steps per user unit that the stepper motor must do). Value stored in ../ConfigurationFiles/config.ini*/
  device_t device_;
  calibration_t calibration_;

 public:
  /**
   * @brief Construct a new XIMC object.
   * 
   * @param ipAddress to assign to the private variable ipAddress_.
   * @param motorIndex to assign to the private variable motorIndex_.
   * @param ratio to assign to the private variable ratio_.
   */
  XIMC(std::string ipAddress, std::string motorIndex, float ratio);
  /**
   * @brief Destroy the XIMC object.
   * 
   */
  ~XIMC();
  float rad_to_user_units(float rad) override;
  int connect() override;
  std::string get_ipAddress() override;
  std::string get_motorIndex() override;
  int calibrate() override;
  int go_home() override;
  int moveCalibratedMotor(float position) override;
  float getPositionRad() override;
  float getPositionUserUnits() override;
  int softStop() override;
  int disconnect() override;
};
