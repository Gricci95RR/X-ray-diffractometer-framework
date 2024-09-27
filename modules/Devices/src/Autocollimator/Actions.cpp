/**
 * @file Autocollimator/Actions.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Autocollimator in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "Autocollimator/Actions.hpp"

namespace autocollimator {

Actions::Actions(std::shared_ptr<IMotor> clientStepper,
                 std::shared_ptr<IScanning> clientScanning) :
                 clientStepper_(clientStepper),
                 clientScanning_(clientScanning),
                 stepperPosition_(0) {
    spdlog::info("cTor Actions Autocollimator\n");
}

Actions::~Actions() {
    spdlog::info("dTor Actions Autocollimator\n");
}

bool Actions::connect() {
    spdlog::info("Method connect Autocollimator of class Actions\n");
    int result_connect_stepper = clientStepper_->connect();
    int result_calibrate = clientStepper_->calibrate();
    if (result_connect_stepper == 0 && result_calibrate == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::disconnect() {
    spdlog::info("Method disconnect Autocollimator of class Actions\n");
    int result_disconnect_stepper = clientStepper_->disconnect();
    if (result_disconnect_stepper == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome Autocollimator of class Actions\n");
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
    spdlog::info("Method moveCalibratedMotor Autocollimator of class Actions\n");
    int result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(stepperPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanStepper() {
    spdlog::info("Method startScanStepper Autocollimator of class Actions\n");
    bool result_scan = clientScanning_->scan();
    spdlog::debug("Result startScanStepper Autocollimator of class Action: {}\n", result_scan);
    return result_scan;
}

bool Actions::stop() {
    spdlog::info("Method Stop Autocollimator of class Actions\n");
    return clientScanning_->stop();
}

float Actions::getStepperPosition() {
    return clientStepper_->getPositionUserUnits();
}

void Actions::setStepperPosition(float stepperPosition) {
    stepperPosition_ = stepperPosition;
}

}  // namespace autocollimator
