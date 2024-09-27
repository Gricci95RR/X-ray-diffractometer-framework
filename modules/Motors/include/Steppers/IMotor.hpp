/**
 * @file IMotor.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class interface used to control the stepper motors.
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

/**
 * @class IMotor
 * @brief Class interface used to control the stepper motors.
 * 
 */
class IMotor {
 public:
    /**
     * @brief Destroy the IMotor object.
     * 
     */
    ~IMotor() = default;

    /**
     * @brief Convert radians to user units.
     * @param rad radians to convert.
     * @return user units.
     */
    virtual float rad_to_user_units(float rad) = 0;

    /**
     * @brief This method establishes a connection with the motor using the IP address and motor index.
     * 
     * Stepper motor Ip Address controller is stored in the config.ini file in ../ConfigurationFiles directory.
     * 
     * @return 1 if connection failed.
     * @return 0 if the connection did not fail.
     */
    virtual int connect() = 0;

    /**
     * @brief Get IP Address of the stepper motor controller.
     * @return IP Address of type std::string.
     */
    virtual std::string get_ipAddress() = 0;

    /**
     * @brief Get stepper motor index.
     * @return Motor index of type std::string.
     */
    virtual std::string get_motorIndex() = 0;

    /**
     * @brief Calibrate stepper motor.
     * 
     * Defines how many steps per user unit the stepper motor must do.
     * ratio = User Units / Steps -> E.G.: 1/800 = 0.00125 -> means that 800 controller steps are equal to 1 user-defined unit.
     * 
     * @return 1 if calibration failed.
     * @return 0 if the calibration did not fail.
     */
    virtual int calibrate() = 0;

    /**
     * @brief This function initiates a home search for the stepper motor.
     * @return 1 if an error occured.
     * @return 0 if the movement to home position was executed correctly.
     */
    virtual int go_home() = 0;

    /**
     * @brief This function allows to execute a movement of a calibrated stepper motor.
     * @param position defines of how many user unit must move (incremental displacement value).
     * @return 1 if an error occured.
     * @return 0 if an error did not occur during the motion and the motor stopped moving.
     */
    virtual int moveCalibratedMotor(float position) = 0;

    /**
     * @brief This function prints current (relative) position in Radians of the stepper motor.
     * @return current relative position in Radians.
     */
    virtual float getPositionRad() = 0;

     /**
     * @brief This function prints current (relative) position in User Units (UU) of the stepper motor.
     * 
     * Note: in the config.ini file in ../ConfigurationFiles directory are listed the units for every device.
     * 
     * @return current relative position in User Units.
     */
    virtual float getPositionUserUnits() = 0;

    /**
     * @brief This function exectutes a soft stop of the motor.
     * @return 1 if the stop was not executed correctly.
     * @return 0 if the stop was executed correctly.
     */
    virtual int softStop() = 0;

    /**
     * @brief Disconnect the device.
     * @return 1 if an error occured.
     * @return 0 if the device was disconnected.
     */
    virtual int disconnect() = 0;
};
