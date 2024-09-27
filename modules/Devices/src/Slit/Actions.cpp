/* © Copyright CERN 2022.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Gianmarco Ricci CERN BE/CEM/MRO 2022
 *         
 *  ===============================================================================================
 */

#include "Slit/Actions.hpp"

namespace slit {

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
                                                                                            "SLIT_STAGE_LINEAR",
                                                                                            "ALIGNMENT_POSITION")),
                 centerRotationalPosition_(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                                clientConfiguration_->getPath(),
                                                                                                "SLIT_STAGE_ROTATIONAL",
                                                                                                "ALIGNMENT_POSITION")) {
    spdlog::info("cTor Actions Slit\n");
    pathToSlitAlinmentResultsDirectory_ = (clientConfiguration_->getPathToLogFilesDirectory() / slitAlignmentResultsDirectoryName_).string();
}

Actions::~Actions() {
    spdlog::info("dTor Actions Slit\n");
}

bool Actions::connect() {
    spdlog::info("Method connect Slit of Class Actions\n");
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
    spdlog::info("Method disconnect Slit of Class Actions\n");
    int result_disconnect_stepper1Linear = clientStepper1Linear_->disconnect();
    int result_disconnect_stepper2Rotational = clientStepper2Rotational_->disconnect();
    if (result_disconnect_stepper1Linear == 0 && result_disconnect_stepper2Rotational == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome Slit of Class Actions\n");
    int result_goHome_stepper1Linear = clientStepper1Linear_->go_home();
    int result_goHome_stepper2Rotational = clientStepper2Rotational_->go_home();
    if (result_goHome_stepper1Linear == 0 && result_goHome_stepper2Rotational == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor1Linear() {
    spdlog::info("Method moveCalibratedMotor1Linear Slit of Class Actions\n");
    int result_moveCalibratedMotor = clientStepper1Linear_->moveCalibratedMotor(stepper1LinearPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor2Rotational() {
    spdlog::info("Method moveCalibratedMotor2Rotational Slit of Class Actions\n");
    int result_moveCalibratedMotor = clientStepper2Rotational_->moveCalibratedMotor(stepper2RotationalPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanStepper1Linear() {
    spdlog::info("Method startScanStepper1Linear Slit of Class Actions\n");
    return clientScanning1Linear_->scan();
}

bool Actions::startScanStepper2Rotational() {
    spdlog::info("Method startScanStepper2Rotational Slit of Class Actions\n");
    return clientScanning2Rotational_->scan();
}

bool Actions::stop() {
    spdlog::info("Method Stop Slit of class Actions\n");
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
    spdlog::info("Method moveBothMotors of class Actions\n");
    bool result_movement_stepper_linear = this->moveCalibratedMotor1Linear();
    bool result_movement_stepper_rotational = this->moveCalibratedMotor2Rotational();
    if (result_movement_stepper_linear == true && result_movement_stepper_rotational == true) {
        return true;
    } else {
        return false;
    }
}

bool Actions::alignDevice() {
    spdlog::info("Method alignDevice of class Actions\n");
    if (alignDevice_) {
        /* Script variables */
        float initialPosition = stepper1LinearPosition_;
        float initialPositionRot = stepper2RotationalPosition_;
        bool result_movement = this->moveBothMotors();
        if (!result_movement) {
            return false;
        }
        std::filesystem::path slitLinearAlignmentScriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                            clientConfiguration_->getPath(),
                                                                                                                            "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                                            "SCRIPT_NAME");

        std::filesystem::path pathToSlitLinearAlignmentScript = pathToScriptDirectory / slitLinearAlignmentScriptName;
        const std::string pathToSlitLinearAlignmentScript_string = pathToSlitLinearAlignmentScript.string();
        /* Setup .csvs results */
        const std::string pathToResultXAxisPositionSlitFileName = pathToSlitAlinmentResultsDirectory_ + "\\"
                                                                  + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                          clientConfiguration_->getPath(),
                                                                                                                          "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                                          "FILENAME_LINEAR_ALIGNMENT_POSITION");
        clientSensors_->flushCsv(pathToResultXAxisPositionSlitFileName);  // Erase content
        const std::string pathToResultRotationalAxisPositionSlitFileName = pathToSlitAlinmentResultsDirectory_ + "\\"
                                                                           + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                                   clientConfiguration_->getPath(),
                                                                                                                                   "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                                                                   "FILENAME_ROTATIONAL_ALIGNMENT_POSITION");
        clientSensors_->flushCsv(pathToResultRotationalAxisPositionSlitFileName);  // Erase content
        const std::string pathToResultSlitAlignmentFileName = pathToSlitAlinmentResultsDirectory_ + "\\"
                                                              + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                      clientConfiguration_->getPath(),
                                                                                                                      "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                                                      "FILENAME_FWHM");
        clientSensors_->flushCsv(pathToResultSlitAlignmentFileName);  // Erase content
        const float startOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                      clientConfiguration_->getPath(),
                                                                                      "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                      "START_OFFSET");  // Offset to the current linear position
        const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                      clientConfiguration_->getPath(),
                                                                                      "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                      "STOP_OFFSET");  // Range
        const float stepSize = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                    clientConfiguration_->getPath(),
                                                                                    "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                    "STEP_SIZE");  // Step Size
        for (float i = startOffset; i <= stopOffset; i = i + stepSize) {
            stepper2RotationalPosition_ = initialPositionRot + i;
            result_movement = this->moveCalibratedMotor2Rotational();
            if (!result_movement && !clientScanning1Linear_->checkReachingPosition(clientStepper2Rotational_->getPositionUserUnits(), stepper2RotationalPosition_)) {
                    return false;
            }
            clientScanning1Linear_->scan();  // Linear axis scan
            std::string string_stepper2RotationalPosition = std::to_string(clientStepper2Rotational_->getPositionUserUnits());
            const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                      clientConfiguration_->getPath(),
                                                                                                      "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                      "DATA_LOG_FILENAME");
            clientPostProcessing_->executeScript5(pathToSlitLinearAlignmentScript_string,
                                                  string_stepper2RotationalPosition,
                                                  dataLogFileName,
                                                  pathToResultXAxisPositionSlitFileName,
                                                  pathToResultRotationalAxisPositionSlitFileName,
                                                  pathToResultSlitAlignmentFileName);
            // Return to initial position
            stepper1LinearPosition_ = initialPosition;
            this->moveCalibratedMotor1Linear();
        }
        /* Read new rotational positions and update .ini file */
        centerLinearPosition_ = clientSensors_->readCsvResult(pathToResultXAxisPositionSlitFileName);
        int result_writeLinearAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(centerLinearPosition_),
                                                                                               "SLIT_STAGE_LINEAR",
                                                                                               "ALIGNMENT_POSITION",
                                                                                               clientConfiguration_->getConfigFilename(),
                                                                                               clientConfiguration_->getPath());
        if (result_writeLinearAlignmentPosition == 0) {
            return false;
        }

        centerRotationalPosition_ = clientSensors_->readCsvResult(pathToResultRotationalAxisPositionSlitFileName);
        int result_writeRotationalAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(centerRotationalPosition_),
                                                                                                   "SLIT_STAGE_ROTATIONAL",
                                                                                                   "ALIGNMENT_POSITION",
                                                                                                   clientConfiguration_->getConfigFilename(),
                                                                                                   clientConfiguration_->getPath());
        if (result_writeRotationalAlignmentPosition == 0) {
            return false;
        }
    }
    /* Final Movement */
    spdlog::debug("Moving to slit alignment positions\n");
    stepper1LinearPosition_ = centerLinearPosition_;
    stepper2RotationalPosition_ = centerRotationalPosition_;
    return this->moveBothMotors();
}

bool Actions::getAlignDevice() {
    return alignDevice_;
}

void Actions::setAlignDevice(bool alignDevice) {
    alignDevice_ = alignDevice;
}

}  // namespace slit
