/**
 * @file XIMC.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Implementation of "IMotor" used to control the hexapod robot.
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <ximc.h>
#include "spdlog/spdlog.h"

#include <iostream>
#include <string>

#include "XIMC.hpp"

XIMC::XIMC(std::string ipAddress, std::string motorIndex, float ratio):ipAddress_(ipAddress), motorIndex_(motorIndex), ratio_(ratio) {
    spdlog::info("cTor XIMC\n");
    set_bindy_key("C:/Crystal Controller Library/Standa Controller/XIMC_Software_package-2018.11.21-win32_win64/win32/keyfile.sqlite");
}

XIMC::~XIMC() {
    spdlog::info("dTor XIMC\n");
}

std::string XIMC::get_ipAddress() {
    return ipAddress_;
}

std::string XIMC::get_motorIndex() {
    return motorIndex_;
}

float XIMC::rad_to_user_units(float rad) {
    spdlog::info("Method rad_to_user_units of Class XIMC\n");
    float deg;
    float user_unit;
    deg = rad*(180/3.14);
    user_unit = deg/1.8;
    return user_unit;
}

int XIMC::connect() {
    spdlog::info("Method connect of Class XIMC\n");
    spdlog::debug("Start connection to motor: 0x{}\n", get_motorIndex());
    std::string deviceName;
    deviceName =  "xi-net://" + get_ipAddress() + "/"  + get_motorIndex();
    std::string deviceName_temp_str = deviceName;
    char *deviceName_temp_ch = deviceName_temp_str.data();  // Conversion string to char *
    spdlog::debug("deviceName: {}\n", deviceName_temp_ch);
    device_t device = open_device(deviceName_temp_ch);
    if (device == device_undefined) {  // if device == -1 => device is undefined => couldnt open device
        spdlog::error("Error openning device\n");
        return 1;
    } else {
        spdlog::debug("Device {} opened.\n", deviceName);
        device_ = device;
        return 0;
    }
}

int XIMC::calibrate() {
    spdlog::info("Start calibration of motor: 0x{}\n", get_motorIndex());
    engine_settings_calb_t engine_settings_calb;
    calibration_t calibration;
    result_t result;
    calibration.A = ratio_;
    calibration.MicrostepMode = MICROSTEP_MODE_FULL;
    if (get_engine_settings_calb(device_, &engine_settings_calb, &calibration) != result_ok) {
        spdlog::error("Error getting engine calibration settings.\n");
        return 1;
    } else {
        spdlog::debug("Device Calibrated.\n");
        calibration_ = calibration;
        return 0;
    }
}

int XIMC::moveCalibratedMotor(float position) {
    spdlog::info("Method moveCalibratedMotor of Class XIMC.\n");
    result_t moveResult = command_move_calb(device_, position, &calibration_);
    result_t result2 = command_wait_for_stop(this->device_, 20);  // wait for stop
    if (moveResult != result_ok && result2 == result_ok) {
        spdlog::error("Failed to move: {}", moveResult);
        return 1;
    } else {
        spdlog::info("Moving Stepper Motor to position {}.\n", position);
        return 0;
    }
}

float XIMC::getPositionRad() {
    //  spdlog::info("Method getPositionRad of Class XIMC.\n");
    get_position_calb_t position_calb_t;
    get_position_calb(device_, &position_calb_t,  &calibration_);
    float posInUserUnits = position_calb_t.Position;
    float posInRad = posInUserUnits * 1.8 * (3.14/180);
    // spdlog::debug("Position: {} [rad], Position Encoder: {} \n", posInRad, position_calb_t.EncPosition);
    return posInRad;
}

float XIMC::getPositionUserUnits() {
    //  spdlog::info("Method getPositionUserUnits of Class XIMC.\n");
    get_position_calb_t position_calb_t;
    get_position_calb(device_, &position_calb_t,  &calibration_);
    float posInUserUnits = position_calb_t.Position;
    // spdlog::debug("Position: {} [User Units], Position Encoder: {} \n", posInUserUnits, position_calb_t.EncPosition);
    return posInUserUnits;
}

int XIMC::go_home() {
    spdlog::info("Method go_home of Class XIMC.\n");
    result_t result1 = command_homezero(this->device_);
    result_t result2 = command_wait_for_stop(this->device_, 20);  // wait for stop
    if (result1 == result_ok && result2 == result_ok) {
        spdlog::info("Homing Stepper Motor.\n");
        return 0;
    } else {
        spdlog::error("Home position has not been reached by stepper motor!\n");
        spdlog::debug("result_command_homezero: {}, result_command_wait_for_stop: {} \n", result1, result2);
        return 1;
    }
}

int XIMC::softStop() {
    spdlog::info("Method softStop of Class XIMC.\n");
    result_t result1 = command_sstp(this->device_);
    if (result1 == result_ok) {
        return 0;
    } else {
        return 1;
    }
}

int XIMC::disconnect() {
    spdlog::info("Method disconnect of Class XIMC.\n");
    result_t closeResult = close_device(&device_);
    if (closeResult != result_ok) {
        spdlog::error("Error closing device: {} \n", closeResult);
        return 1;
    } else {
        spdlog::debug("Device closed\n");
        return 0;
    }
}
