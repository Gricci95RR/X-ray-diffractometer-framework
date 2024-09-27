/**
 * @file Monochromator/Actions.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Monochromator device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */
#include "Monochromator/Actions.hpp"

namespace monochromator {

Actions::Actions(std::shared_ptr<IMotor> clientStepper1Linear,
                 std::shared_ptr<IMotor> clientStepper2Rotational,
                 std::shared_ptr<IScanning> clientScanning1Linear,
                 std::shared_ptr<IScanning> clientScanning2Rotational,
                 std::shared_ptr<ISensors> clientSensors,
                 std::shared_ptr<IConfiguration> clientConfiguration,
                 std::shared_ptr<IPostProcessing> clientPostProcessing) :
                 clientStepper1Linear_(clientStepper1Linear),
                 clientStepper2Rotational_(clientStepper2Rotational),
                 clientScanning1Linear_(clientScanning1Linear),
                 clientScanning2Rotational_(clientScanning2Rotational),
                 clientSensors_(clientSensors),
                 clientConfiguration_(clientConfiguration),
                 clientPostProcessing_(clientPostProcessing),
                 stepper1LinearPosition_(0),
                 stepper2RotationalPosition_(0),
                 centerLinearPosition_(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                            clientConfiguration_->getPath(),
                                                                                            "MONOCHROMATOR_STAGE_LINEAR",
                                                                                            "ALIGNMENT_POSITION")),
                 centerRotationalPosition_(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                                clientConfiguration_->getPath(),
                                                                                                "MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                "ALIGNMENT_POSITION")) {
    spdlog::info("cTor Actions Monochromator\n");
    pathToMonochromatorAlinmentResultsDirectory_ = (clientConfiguration_->getPathToLogFilesDirectory() / monochromatorAlignmentResultsDirectoryName_).string();
}

Actions::~Actions() {
    spdlog::info("dTor Actions Monochromator\n");
}

bool Actions::connect() {
    spdlog::info("Method connect Monochromator of Class Action\n");
    int result_connect_stepper1Linear = clientStepper1Linear_->connect();
    int result_connect_stepper2Rotational = clientStepper2Rotational_->connect();
    int result_calibrate_stepper1Linear = clientStepper1Linear_->calibrate();
    int result_calibrate_stepper2Rotational = clientStepper2Rotational_->calibrate();
    if (result_connect_stepper1Linear == 0 && result_connect_stepper2Rotational == 0 && result_calibrate_stepper1Linear == 0 && result_calibrate_stepper2Rotational == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::disconnect() {
    spdlog::info("Method disconnect Monochromator of Class Action\n");
    int result_disconnect_stepper1Linear = clientStepper1Linear_->disconnect();
    int result_disconnect_stepper2Rotational = clientStepper2Rotational_->disconnect();
    if (result_disconnect_stepper1Linear == 0 && result_disconnect_stepper2Rotational == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome Monochromator of Class Action\n");
    int result_goHome_stepper1Linear = clientStepper1Linear_->go_home();
    int result_goHome_stepper2Rotational = clientStepper2Rotational_->go_home();
    if (result_goHome_stepper1Linear == 0 && result_goHome_stepper2Rotational == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor1Linear() {
    spdlog::info("Method moveCalibratedMotor1Linear Monochromator of Class Action\n");
    int result_moveCalibratedMotor = clientStepper1Linear_->moveCalibratedMotor(stepper1LinearPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor2Rotational() {
    spdlog::info("Method moveCalibratedMotor2Rotational Monochromator of Class Action\n");
    int result_moveCalibratedMotor = clientStepper2Rotational_->moveCalibratedMotor(stepper2RotationalPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanStepper1Linear() {
    spdlog::info("Method startScanStepper1Linear Monochromator of Class Action\n");
    return clientScanning1Linear_->scan();
}

bool Actions::startScanStepper2Rotational() {
    spdlog::info("Method startScanStepper2Rotational Monochromator of Class Action\n");
    return clientScanning2Rotational_->scan();
}

bool Actions::stop() {
    spdlog::info("Method Stop Monochromator of class Actions\n");
    bool result_stop1 = clientScanning1Linear_->stop();
    bool result_stop2 = clientScanning2Rotational_->stop();
    if (result_stop1 == true && result_stop2 == true) {
        return true;
    } else {
        return false;
    }
}

float Actions::getStepper1LinearPosition() {
    return clientStepper1Linear_->getPositionUserUnits();
}

void Actions::setStepper1LinearPosition(float stepperPosition) {
    stepper1LinearPosition_ = stepperPosition;
}

float Actions::getStepper2RotationalPosition() {
    return clientStepper2Rotational_->getPositionUserUnits();
}

void Actions::setStepper2RotationalPosition(float stepperPosition) {
    stepper2RotationalPosition_ = stepperPosition;
}

bool Actions::moveBothMotors() {
    bool result_movement_stepper_linear = this->moveCalibratedMotor1Linear();
    bool result_movement_stepper_rotational = this->moveCalibratedMotor2Rotational();
    if (result_movement_stepper_linear == true && result_movement_stepper_rotational == true) {
        return true;
    } else {
        return false;
    }
}

bool Actions::alignMonochromator() {
    if (searchCenter_) {
        this->moveCalibratedMotor1Linear();  // Move linear motor to predefined position
        bool result_movement;
        /* Setup Result csvs */
        const std::string pathToResultXAxisFileName = pathToMonochromatorAlinmentResultsDirectory_ + "\\"
                                                      + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                              clientConfiguration_->getPath(),
                                                                                                              "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                              "FILENAME_TO_X_ALIGNMENT_POSITION");
        const std::string pathToResultOmegaAxisFileName = pathToMonochromatorAlinmentResultsDirectory_ + "\\"
                                                          + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                clientConfiguration_->getPath(),
                                                                                                                "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                "FILENAME_TO_OMEGA_ALIGNMENT_POSITION");
        const std::string pathToResultSlopesXOmegaAxesFileName = pathToMonochromatorAlinmentResultsDirectory_ + "\\"
                                                                 + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                         clientConfiguration_->getPath(),
                                                                                                                         "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                         "FILENAME_TO_SLOPES");
        clientSensors_->flushCsv(pathToResultXAxisFileName);
        clientSensors_->flushCsv(pathToResultOmegaAxisFileName);
        clientSensors_->flushCsv(pathToResultSlopesXOmegaAxesFileName);
        // Linear Alignment Script variables
        std::filesystem::path SearchMonochromatorLinearAlignmentScriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                                           clientConfiguration_->getPath(),
                                                                                                                                           "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                                                           "SCRIPT_NAME");
        std::filesystem::path pathToMonochromatorLinearAlignmentScript = pathToScriptDirectory_ / SearchMonochromatorLinearAlignmentScriptName;
        const std::string pathToMonochromatorLinearAlignmentScript_string = pathToMonochromatorLinearAlignmentScript.string();
        const std::string dataLogFilename_linearAlignment = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                  clientConfiguration_->getPath(),
                                                                                                                  "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                                  "DATA_LOG_FILENAME");
        // Rotational Alignment Script variables
        std::filesystem::path SearchMonochromatorRotationalAlignmentScriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                                               clientConfiguration_->getPath(),
                                                                                                                                               "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                                               "SCRIPT_NAME");
        std::filesystem::path pathToMonochromatorRotationalAlignmentScript = pathToScriptDirectory_ / SearchMonochromatorRotationalAlignmentScriptName;
        const std::string pathToMonochromatorRotationalAlignmentScript_string = pathToMonochromatorRotationalAlignmentScript.string();
        const std::string dataLogFilename_rotationalAlignment = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                      clientConfiguration_->getPath(),
                                                                                                                      "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                                      "DATA_LOG_FILENAME");
        /* X-Scan to find Monochromator Center */
        bool result_scan = clientScanning1Linear_->scan();
        if (!result_scan) {
            return false;
        }
        clientPostProcessing_->executeScript2(pathToMonochromatorLinearAlignmentScript_string,
                                              dataLogFilename_linearAlignment,
                                              pathToResultXAxisFileName);
        centerLinearPosition_ = clientSensors_->readCsvResult(pathToResultXAxisFileName);  // Read from .csv file
        /* X-Omega Scan */
        const float startOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                       "START_OFFSET");
        const float range = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                 clientConfiguration_->getPath(),
                                                                                 "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                 "RANGE");
        const float stepSize = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                    clientConfiguration_->getPath(),
                                                                                    "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                    "STEP_SIZE");
        for (float i = startOffset; i < range; i = i + stepSize) {
            stepper1LinearPosition_ = centerLinearPosition_ + i;
            result_movement = this->moveCalibratedMotor1Linear();
            if (!result_movement && !clientScanning1Linear_->checkReachingPosition(clientStepper1Linear_->getPositionUserUnits(), stepper1LinearPosition_)) {
                    return false;
            }
            clientScanning2Rotational_->scan();  // Omega Scan

            std::string string_stepper1LinearPosition = std::to_string(stepper1LinearPosition_);
            clientPostProcessing_->executeScript5(pathToMonochromatorRotationalAlignmentScript_string,
                                                  string_stepper1LinearPosition,
                                                  dataLogFilename_rotationalAlignment,
                                                  pathToResultSlopesXOmegaAxesFileName,
                                                  pathToResultXAxisFileName,
                                                  pathToResultOmegaAxisFileName);
            // Rotational Axis in 0 position for next scan
            stepper2RotationalPosition_ = 0.0;
            result_movement = this->moveCalibratedMotor2Rotational();
            if (!result_movement) {
                    return false;
            }
        }  // end for
        /* Read new linear and rotational positions and update .ini file */
        centerLinearPosition_ = clientSensors_->readCsvResult(pathToResultXAxisFileName);  // Read Monochromator crystal X-Axis center position from .csv file after scans
        int result_writeLinearAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(centerLinearPosition_),
                                                                                               "MONOCHROMATOR_STAGE_LINEAR",
                                                                                               "ALIGNMENT_POSITION",
                                                                                               clientConfiguration_->getConfigFilename(),
                                                                                               clientConfiguration_->getPath());
        if (result_writeLinearAlignmentPosition == 0) {
            return false;
        }

        centerRotationalPosition_ = clientSensors_->readCsvResult(pathToResultOmegaAxisFileName);  // Read Monochromator crystal Omega-Axis alignment position from .csv file after scans
        int result_writeRotationalAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(centerRotationalPosition_),
                                                                                                   "MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                   "ALIGNMENT_POSITION",
                                                                                                   clientConfiguration_->getConfigFilename(),
                                                                                                   clientConfiguration_->getPath());
        if (result_writeRotationalAlignmentPosition == 0) {
            return false;
        }
    }
    /*Move to linear and rotational positons*/
    stepper1LinearPosition_ = centerLinearPosition_;
    stepper2RotationalPosition_ = centerRotationalPosition_;
    bool result_movement1 = this->moveCalibratedMotor1Linear();
    bool result_movement2 = this->moveCalibratedMotor2Rotational();
    if (!result_movement1 && !result_movement2) {
        return false;
    }
    return true;
}

bool Actions::searchMonochromatorBraggPeak() {
    spdlog::info("Method searchMonochromatorBraggPeak Monochromator of Class Action\n");
    bool result_movement = this->moveCalibratedMotor2Rotational();  // Move linear motor to predefined position
    if (!result_movement) {
        return false;
    }
    bool result_scan = clientScanning2Rotational_->scan();  // Omega Scan
    if (!result_scan) {
        return false;
    }
    // Setup .csv result
    std::string pathToResultMonochromatorBraggPeakSearch = pathToMonochromatorAlinmentResultsDirectory_ + "\\"
                                                           + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                   clientConfiguration_->getPath(),
                                                                                                                   "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                   "FILENAME_TO_ALIGNMENT_POSITION");
    clientSensors_->flushCsv(pathToResultMonochromatorBraggPeakSearch);  // Erase content
    // Bragg Peak search Script variables
    std::filesystem::path SearchMonochromatorBraggPeakScriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                                 clientConfiguration_->getPath(),
                                                                                                                                 "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                                 "SCRIPT_NAME");
    std::filesystem::path pathToMonochromatorBraggPeakScript = pathToScriptDirectory_ / SearchMonochromatorBraggPeakScriptName;
    const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                              clientConfiguration_->getPath(),
                                                                                              "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                              "DATA_LOG_FILENAME");
    clientPostProcessing_->executeScript2(pathToMonochromatorBraggPeakScript.string(),
                                          dataLogFileName,
                                          pathToResultMonochromatorBraggPeakSearch);
    /* Read new rotational positions and update .ini file */
    float monochromatorBraggPeakAngle = clientSensors_->readCsvResult(pathToResultMonochromatorBraggPeakSearch);  // Read Monochromator crystal X-Axis center position from .csv file after scans
    int result_writeBraggPeakAngle = clientConfiguration_->writeConfigurationFile(std::to_string(monochromatorBraggPeakAngle),
                                                                                  "MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                  "BRAGG_PEAK_ANGLE",
                                                                                  clientConfiguration_->getConfigFilename(),
                                                                                  clientConfiguration_->getPath());

    if (result_writeBraggPeakAngle == 0) {
        return false;
    }
    /* Final Movement */
    stepper2RotationalPosition_ = monochromatorBraggPeakAngle;
    bool result_movement2 = this->moveCalibratedMotor2Rotational();
    if (!result_movement2) {
        return false;
    }
    return true;
}

bool Actions::getSearchCenter() {
    return searchCenter_;
}

void Actions::setSearchCenter(bool searchCenter) {
    searchCenter_ = searchCenter;
}

float Actions::getCenterPosition() {
    return centerLinearPosition_;
}

void Actions::setCenterPosition(float centerPosition) {
    centerLinearPosition_ = centerPosition;
}

}  // namespace monochromator
