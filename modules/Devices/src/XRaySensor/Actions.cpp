/**
 * @file XRaySensor/Actions.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the XRaySensor device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "XRaySensor/Actions.hpp"

namespace xRaySensor {

Actions::Actions(std::shared_ptr<IMotor> clientStepper, std::shared_ptr<IScanning> clientScanning) :
                 clientStepper_(clientStepper),
                 clientScanning_(clientScanning),
                 stepperPosition_(0) {
    spdlog::info("cTor Actions XRaySensor\n");
}

Actions::~Actions() {
    spdlog::info("dTor Actions XRaySensor\n");
}

bool Actions::connect() {
    spdlog::info("Method connect XRaySensor of class Actions\n");
    int result_connect_stepper = clientStepper_->connect();
    int result_calibrate = clientStepper_->calibrate();
    if (result_connect_stepper == 0 && result_calibrate == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::disconnect() {
    spdlog::info("Method disconnect XRaySensor of class Actions\n");
    int result_disconnect_stepper = clientStepper_->disconnect();
    if (result_disconnect_stepper == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome XRaySensor of class Actions\n");
    int result_goHome_stepper = clientStepper_->go_home();
    float position = clientStepper_->getPositionUserUnits();
    if (result_goHome_stepper == 0) {
        stepperPosition_ = 0;
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor() {
    spdlog::info("Method moveCalibratedMotor XRaySensor of class Actions\n");
    int result_moveCalibratedMotor = 1;
    result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(stepperPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanStepper() {
    spdlog::info("Method startScan XRaySensor of class Actions\n");
    return clientScanning_->scan();
}

bool Actions::stop() {
    spdlog::info("Method stop XRaySensor of class Actions\n");
    return clientScanning_->stop();
}

float Actions::getStepperPosition() {
    return clientStepper_->getPositionUserUnits();
}

void Actions::setStepperPosition(float stepperPosition) {
    stepperPosition_ = stepperPosition;
}

}  // namespace xRaySensor
