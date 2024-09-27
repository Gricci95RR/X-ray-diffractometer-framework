/**
 * @file XRaySource/Actions.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the XRaySource device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "XRaySource/Actions.hpp"

namespace xRaySource {

Actions::Actions(std::shared_ptr<IMotor> clientStepper,
                 std::shared_ptr<IScanning> clientScanning,
                 std::shared_ptr<ISensors> clientSensors,
                 std::shared_ptr<IConfiguration> clientConfiguration,
                 std::shared_ptr<IPostProcessing> clientPostProcessing) :
                 clientStepper_(clientStepper),
                 clientScanning_(clientScanning),
                 clientSensors_(clientSensors),
                 clientConfiguration_(clientConfiguration),
                 clientPostProcessing_(clientPostProcessing),
                 stepperPosition_(0),
                 centerPosition_(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                      clientConfiguration_->getPath(),
                                                                                      "X-RAY_SOURCE",
                                                                                      "ALIGNMENT_POSITION")) {
    spdlog::info("cTor Actions XRaySource\n");
    pathToXRaySourceAlinmentResultsDirectory_ = (clientConfiguration_->getPathToLogFilesDirectory() / xRaySourceAlignmentResultsDirectoryName_).string();
}

Actions::~Actions() {
    spdlog::info("dTor Actions XRaySource\n");
}

bool Actions::connect() {
    spdlog::info("Method connect XRaySource of class Actions\n");
    int result_connect_stepper = clientStepper_->connect();
    int result_calibrate = clientStepper_->calibrate();
    if (result_connect_stepper == 0 && result_calibrate == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::disconnect() {
    spdlog::info("Method disconnect XRaySource of class Actions\n");
    int result_disconnect_stepper = clientStepper_->disconnect();
    if (result_disconnect_stepper == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome XRaySource of class Actions\n");
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
    spdlog::info("Method moveCalibratedMotor XRaySource of class Actions\n");
    int result_moveCalibratedMotor = 1;
    result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(stepperPosition_);
    spdlog::debug("Moving motor to position: {}\n", stepperPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanStepper() {
    spdlog::info("Method startScan XRaySource of class Actions\n");
    return clientScanning_->scan();
}

bool Actions::stop() {
    spdlog::info("Method stop XRaySource of class Actions\n");
    return clientScanning_->stop();
}

float Actions::getStepperPosition() {
    return clientStepper_->getPositionUserUnits();
}

void Actions::setStepperPosition(float stepperPosition) {
    stepperPosition_ = stepperPosition;
}

bool Actions::alignSourceWithSensor() {
    spdlog::info("Method alignSourceWithSensor XRaySource of class Actions\n");
    if (searchCenter_) {
        float centerPosition = 0;
        this->goHome();
        bool result_scan = clientScanning_->scan();
        if (!result_scan) {
            return false;
        }
        /* Setup .csv result */
        std::string pathToResultAlignmentScanSourceFileName = pathToXRaySourceAlinmentResultsDirectory_ + "\\"
                                                              + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                      clientConfiguration_->getPath(),
                                                                                                                      "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                                      "FILENAME_TO_ALIGNMENT_POSITION");
        clientSensors_->flushCsv(pathToResultAlignmentScanSourceFileName);  // Erase content
        std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                         clientConfiguration_->getPath(),
                                                                                                         "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                         "SCRIPT_NAME");
        std::filesystem::path pathToAlignmentScript = pathToScriptDirectory_ / scriptName;
        const std::string pathToAlignmentScript_string = pathToAlignmentScript.string();
        /*Execute Script*/
        const std::string dataLogFilename = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                  "DATA_LOG_FILENAME");
        clientPostProcessing_->executeScript2(pathToAlignmentScript_string,
                                              dataLogFilename,
                                              pathToResultAlignmentScanSourceFileName);
        /*Read Alignment Position and update .ini file*/
        centerPosition_ = clientSensors_->readCsvResult(pathToResultAlignmentScanSourceFileName);
        int result_writeAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(centerPosition_),
                                                                                         "X-RAY_SOURCE",
                                                                                         "ALIGNMENT_POSITION",
                                                                                         clientConfiguration_->getConfigFilename(),
                                                                                         clientConfiguration_->getPath());  // update .ini file
        if (result_writeAlignmentPosition == 0) {
            return false;
        }
    }
    /* Final movement to alignment position */
    stepperPosition_ = centerPosition_;
    return this->moveCalibratedMotor();
}

bool Actions::getSearchCenter() {
    return searchCenter_;
}

void Actions::setSearchCenter(bool searchCenter) {
    searchCenter_ = searchCenter;
}

float Actions::getCenterPosition() {
    return centerPosition_;
}

void Actions::setCenterPosition(float centerPosition) {
    centerPosition_ = centerPosition;
}


}  // namespace xRaySource
