/**
 * @file Crystal/Actions.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Crystal device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "Crystal/Actions.hpp"

namespace crystal {

Actions::Actions(std::shared_ptr<IHXP> clientHxp,
                 std::shared_ptr<IMotor> clientStepper,
                 std::shared_ptr<IScanning> clientScanningHXP,
                 std::shared_ptr<IScanning> clientScanningStepper,
                 std::shared_ptr<sensors::ISensors> clientSensors,
                 std::shared_ptr<IConfiguration> clientConfiguration,
                 std::shared_ptr<IPostProcessing> clientPostProcessing):
                 clientHxp_(clientHxp),
                 clientStepper_(clientStepper),
                 clientScanningHXP_(clientScanningHXP),
                 clientScanningStepper_(clientScanningStepper),
                 clientSensors_(clientSensors),
                 clientConfiguration_(clientConfiguration),
                 clientPostProcessing_(clientPostProcessing),
                 crystalWidth_(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                    clientConfiguration_->getPath(),
                                                                                    "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                    "CRYSTAL_WIDTH")),
                 stepperPosition_(0) {
    spdlog::info("cTor Actions Crystal\n");
    pathToCrystalAlinmentResultsDirectory_ = (clientConfiguration_->getPathToLogFilesDirectory() / crystalAlignmentResultsDirectoryName_).string();
    hxpAlignmentCoord_.CoordX = 0;
}

Actions::~Actions() {
    spdlog::info("dTor Actions Crystal\n");
}

bool Actions::connect() {
    spdlog::info("Method connect Crystal of class Actions\n");
    int result_connect_hxp = clientHxp_->connect(10, "HEXAPOD");
    int result_connect_stepper = clientStepper_->connect();
    int result_calibrate = clientStepper_->calibrate();
    if (result_connect_stepper == 0 && result_calibrate == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::disconnect() {
    spdlog::info("Method disconnect Crystal of class Actions\n");
    int result_disconnect_stepper = clientStepper_->disconnect();
    clientHxp_->disconnect();
    if (result_disconnect_stepper == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::goHome() {
    spdlog::info("Method goHome Crystal of class Actions\n");
    int result_goHome_hxp = clientHxp_->goHome();
    int result_goHome_stepper = clientStepper_->go_home();
    if (result_goHome_hxp == 0 && result_goHome_stepper == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::setHxpPositionAbsolute() {
    spdlog::info("Method setHxpPositionAbsolute Crystal of class Actions\n");
    int result_setPositionAbsolute = clientHxp_->setPositionAbsolute(clientHxp_->getCoordinateX(),
                                                                     clientHxp_->getCoordinateY(),
                                                                     clientHxp_->getCoordinateZ(),
                                                                     clientHxp_->getCoordinateU(),
                                                                     clientHxp_->getCoordinateV(),
                                                                     clientHxp_->getCoordinateW());
    if (result_setPositionAbsolute == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveCalibratedMotor() {
    spdlog::info("Method moveCalibratedMotor Crystal of class Actions\n");
    int result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(stepperPosition_);
    if (result_moveCalibratedMotor == 0) {
        return true;
    } else {
        return false;
    }
}

bool Actions::moveBothMotors() {
    bool result_movement_hxp = this->setHxpPositionAbsolute();
    bool result_movement_stepper_rotational = this->moveCalibratedMotor();
    if (result_movement_hxp == true && result_movement_stepper_rotational == true) {
        return true;
    } else {
        return false;
    }
}

bool Actions::searchXAxisAlignmentCrystal() {
    spdlog::info("Method searchXAxisAlignmentCrystal Crystal of class Actions\n");
    /* Initial Movement */
    clientScanningHXP_->hxpAxisToScan_ = 1;
    this->moveBothMotors();
    bool result_scan = clientScanningHXP_->scan();  // X Scan
    if (!result_scan) {
        return false;
    }
    /* Setup .csvs result */
    std::string resultAlignmentCrystalXAxisFileNamePath = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                          + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                  clientConfiguration_->getPath(),
                                                                                                                  "XAxis_Alignment_CRYSTAL_STAGE",
                                                                                                                  "FILENAME_TO_X_ALIGNMENT_POSITION");
    clientSensors_->flushCsv(resultAlignmentCrystalXAxisFileNamePath);  // Erase content
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");
    std::filesystem::path pathToAlignmentScript = pathToScriptDirectory_ / scriptName;
    const std::string pathToAlignmentScript_string = pathToAlignmentScript.string();
    /* Execute Alignment script */
    std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                clientConfiguration_->getPath(),
                                                                                                                "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                                "DATA_LOG_FILENAME");
    clientPostProcessing_->executeScript2(pathToAlignmentScript_string,
                                          dataLogFileName,
                                          resultAlignmentCrystalXAxisFileNamePath);
    /* Read new X-Axis positions and update .ini file */
    hxpAlignmentCoord_.CoordX = clientSensors_->readCsvResult(resultAlignmentCrystalXAxisFileNamePath);
    int result_writeXAxisAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(hxpAlignmentCoord_.CoordX),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X",
                                                                                          clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath());
    if (result_writeXAxisAlignmentPosition == 0) {
        return false;
    }
    clientHxp_->setCoordinateX(hxpAlignmentCoord_.CoordX);
    /*Final movement to alignment position*/
    return this->setHxpPositionAbsolute();
}

bool Actions::searchZAxisAlignmentCrystal() {
    spdlog::info("Method searchZAxisAlignmentCrystal Crystal of class Actions\n");
    /* Initial Movement */
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    bool result_scan = clientScanningHXP_->scan();  // Z Scan
    if (!result_scan) {
        return false;
    }
    /* Setup .csvs result */
    std::string resultAlignmentCrystalZAxisFileNamePath = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                          + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                  clientConfiguration_->getPath(),
                                                                                                                  "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                                  "FILENAME_TO_Z_ALIGNMENT_POSITION");
    clientSensors_->flushCsv(resultAlignmentCrystalZAxisFileNamePath);  // Erase content
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");
    std::filesystem::path pathToAlignmentScript = pathToScriptDirectory_ / scriptName;
    const std::string pathToAlignmentScript_string = pathToAlignmentScript.string();
    /* Execute Alignment script */
    std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                clientConfiguration_->getPath(),
                                                                                                                "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                                "DATA_LOG_FILENAME");
    clientPostProcessing_->executeScript2(pathToAlignmentScript_string,
                                          dataLogFileName,
                                          resultAlignmentCrystalZAxisFileNamePath);
    /* Read new position and update .ini file */
    hxpAlignmentCoord_.CoordZ = clientSensors_->readCsvResult(resultAlignmentCrystalZAxisFileNamePath);
    int result_writeZAxisAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(hxpAlignmentCoord_.CoordZ),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_Z",
                                                                                          clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath());

    if (result_writeZAxisAlignmentPosition == 0) {
        return false;
    }
    clientHxp_->setCoordinateZ(hxpAlignmentCoord_.CoordZ);
    /*Final movement to alignment position*/
    return this->setHxpPositionAbsolute();
}

bool Actions::searchYAxisAlignmentCrystal() {
    spdlog::info("Method searchYAxisAlignmentCrystal Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextWAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME_OFFSET");
    std::filesystem::path pathAlignmentScript = pathToScriptDirectory_ / scriptName;
    const std::string pathToAlignmentOffsetScript_string = pathAlignmentScript.string();
    /* Setup .csvs result */
    const std::string pathToSlopes = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_Y_SLOPES");
    clientSensors_->flushCsv(pathToSlopes);  // Erase content
    const std::string pathToResultWaxis = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_W_ALIGNMENT_POSITION");
    clientSensors_->flushCsv(pathToResultWaxis);  // Erase content 
    const std::string pathToResultXaxis = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_X_ALIGNMENT_POSITION");
    clientSensors_->flushCsv(pathToResultXaxis);  // Erase content                                                                                                 
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_W");
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                  "RANGE_W");  // Range W Scan
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextWAxisPosition = initialPositionWAxis + i;
        /* Movement to initial position */
        clientHxp_->setCoordinateX(-10);
        clientHxp_->setCoordinateW(nextWAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionW(), nextWAxisPosition)) {
            return false;
        }
        /* Align X axis */
        clientScanningHXP_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        this->searchXAxisAlignmentCrystal();
        /* Scan Y axis */
        clientScanningHXP_->hxpAxisToScan_ = 2;
        clientScanningHXP_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE_Y"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "RANGE_Y"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME_Y_SCAN"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        bool result_scan = clientScanningHXP_->scanRelative();  // Y Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_WCoordinate = std::to_string(clientHxp_->getCoordinateW());
        const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                                  "DATA_LOG_FILENAME_Y_SCAN");
        clientPostProcessing_->executeScript7(pathToAlignmentOffsetScript_string,
                                              dataLogFileName,
                                              pathToSlopes,
                                              std::to_string(clientHxp_->getCoordinateW()),
                                              std::to_string(clientHxp_->getCoordinateX()),
                                              std::to_string(clientHxp_->getCoordinateY()),
                                              pathToResultWaxis,
                                              pathToResultXaxis
                                              );
        clientHxp_->setCoordinateY(initialPositionYAxis);  // return to initial position
    }
    /* Read new X and W Axes positions and update .ini file */
    clientHxp_->setCoordinateW(clientSensors_->readCsvResult(pathToResultWaxis));
    clientHxp_->setCoordinateX(clientSensors_->readCsvResult(pathToResultXaxis));
    int result_writeWAxisAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(hxpAlignmentCoord_.CoordW),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_W",
                                                                                          clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath());
    int result_writeXAxisAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(hxpAlignmentCoord_.CoordX),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X",
                                                                                          clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath());
    if (result_writeWAxisAlignmentPosition == 0) {
        return false;
    }
    if (result_writeXAxisAlignmentPosition == 0) {
        return false;
    }
    /* Final movement to alignment position */
    return this->setHxpPositionAbsolute();
}

bool Actions::searchYWAxesAlignmentCrystal() {
    spdlog::info("Method searchYWAxesAlignmentCrystal Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextYAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");
    std::filesystem::path pathAlignmentScript = pathToScriptDirectory_ / scriptName;
    const std::string pathToYWAxesAlignmentScript_string = pathAlignmentScript.string();
    /* Setup .csvs result */
    const std::string pathToResultYAlignment = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_Y_ALIGNMENT_POSITION");
    const std::string pathToResultStdDevSlopes = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                 + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                         clientConfiguration_->getPath(),
                                                                                                         "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                         "FILENAME_TO_STANDARD_DEVIATION_RESULT");
    const std::string pathToSlopesYWAlignment = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                        clientConfiguration_->getPath(),
                                                                                                        "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                        "FILENAME_TO_SLOPESFILE");
    clientSensors_->flushCsv(pathToResultYAlignment);  // Erase content
    clientSensors_->flushCsv(pathToResultStdDevSlopes);  // Erase content
    clientSensors_->flushCsv(pathToSlopesYWAlignment);  // Erase content
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_SCAN_HXP_Y");
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                  "RANGE_SCAN_HXP_Y");  // Range Y Scan
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextYAxisPosition = initialPositionYAxis + i;
        clientHxp_->setCoordinateY(nextYAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionY(), nextYAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                  "DATA_LOG_FILENAME");
        clientPostProcessing_->executeScript5(pathToYWAxesAlignmentScript_string,
                                              string_actual_YCoordinate,
                                              dataLogFileName,
                                              pathToResultYAlignment,
                                              pathToResultStdDevSlopes,
                                              pathToSlopesYWAlignment);
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read new Y and W Axes positions and update .ini file */
    hxpAlignmentCoord_.CoordY = clientSensors_->readCsvResult(pathToResultYAlignment);
    clientHxp_->setCoordinateY(hxpAlignmentCoord_.CoordY);
    int result_writeYAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(hxpAlignmentCoord_.CoordY),
                                                                                      "CRYSTAL_HXP",
                                                                                      "ALIGNMENT_POSITION_HXP_Y",
                                                                                      clientConfiguration_->getConfigFilename(),
                                                                                      clientConfiguration_->getPath());

    if (result_writeYAlignmentPosition == 0) {
        return false;
    }

    float stdDevSlopes = clientSensors_->readCsvResult(pathToResultStdDevSlopes);  // std deviation of slopes
    float thresholdSTD = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                              clientConfiguration_->getPath(),
                                                                              "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                              "THRESHOLD_STD_DEVIATION");
    if (stdDevSlopes > thresholdSTD) {
        spdlog::error("Alignment failed. Standard Deviation of slope differences exceeded threshold.\n");
        return false;
    }
    /* Final movement to alignment position */
    return this->setHxpPositionAbsolute();
}

bool Actions::searchBraggPeakCrystal() {
    spdlog::info("Method searchBraggPeakCrystal Crystal of Class Action\n");
    /* Initial Movement */
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    bool result_scan = clientScanningHXP_->scan();  // W Scan
    if (!result_scan) {
        return false;
    }
    // Setup .csv result
    const std::string pathToResultCrystalBraggPeakSearch = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                           + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                   clientConfiguration_->getPath(),
                                                                                                             "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                             "FILENAME_TO_BRAGG_ANGLE");
    clientSensors_->flushCsv(pathToResultCrystalBraggPeakSearch);  // Erase content
    // Bragg Peak search Script variables
    const std::filesystem::path SearchCrystalBraggPeakScriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                                 clientConfiguration_->getPath(),
                                                                                                                                 "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                                                 "SCRIPT_NAME");
    std::filesystem::path pathToCrystalBraggPeakScript = pathToScriptDirectory_ / SearchCrystalBraggPeakScriptName;
    const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                              clientConfiguration_->getPath(),
                                                                                              "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                              "DATA_LOG_FILENAME");
    clientPostProcessing_->executeScript2(pathToCrystalBraggPeakScript.string(),
                                          dataLogFileName,
                                          pathToResultCrystalBraggPeakSearch);
    /* Read new rotational positions and update .ini file */
    float crytalBraggPeakAngle = clientSensors_->readCsvResult(pathToResultCrystalBraggPeakSearch);  // Read Crystal crystal X-Axis center position from .csv file after scans
    int result_writeBraggPeakAngle = clientConfiguration_->writeConfigurationFile(std::to_string(crytalBraggPeakAngle),
                                                                                  "CRYSTAL_HXP",
                                                                                  "ALIGNMENT_POSITION_HXP_W",
                                                                                  clientConfiguration_->getConfigFilename(),
                                                                                  clientConfiguration_->getPath());
    if (result_writeBraggPeakAngle == 0) {
        return false;
    }
    /* Final Movement */
    clientHxp_->setCoordinateW(crytalBraggPeakAngle);
    bool result_movement2 = this->moveBothMotors();
    if (!result_movement2) {
        return false;
    }
    return true;
}

bool Actions::searchYAxisFineAlignment() {
    spdlog::info("Method searchYWAxesAlignmentCrystal Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextYAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    /* Script Fully Opened Beam */
    std::filesystem::path scriptNameFobComputation = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                   clientConfiguration_->getPath(),
                                                                                                                   "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                                   "SCRIPT_NAME_FULLY_OPENED_BEAM");

    std::filesystem::path pathComputeFullyOpenedBeamScript = pathToScriptDirectory_ / scriptNameFobComputation;
    const std::string pathToComputeFullyOpenedBeamScript_string = pathComputeFullyOpenedBeamScript.string();
    /* Script Y alignment */
    std::filesystem::path scriptNameYinouComputation = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                     clientConfiguration_->getPath(),
                                                                                                                     "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                                     "SCRIPT_NAME");

    std::filesystem::path pathAlignmentScript = pathToScriptDirectory_ / scriptNameYinouComputation;
    const std::string pathToYAxisFineAlignmentScript_string = pathAlignmentScript.string();
    /* Setup .csvs result */
    const std::string pathToResultYAlignment = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_Y_ALIGNMENT_POSITION");
    const std::string pathToYinFineAlignment = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                               + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                       "FILENAME_TO_Yin");
    const std::string pathToYoutFineAlignment = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                        clientConfiguration_->getPath(),
                                                                                                        "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                        "FILENAME_TO_Yout");
    const std::string pathToResultFullyOpenedBeam = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                    + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                            "FILENAME_TO_FULLY_OPENED_BEAM");                                                                                                 
    clientSensors_->flushCsv(pathToResultYAlignment);  // Erase content
    clientSensors_->flushCsv(pathToYinFineAlignment);  // Erase content
    clientSensors_->flushCsv(pathToYoutFineAlignment);  // Erase content
    clientSensors_->flushCsv(pathToResultFullyOpenedBeam);  // Erase content
    /* --- First scan to find Fully opened beam --- */
    bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
    /* Execute Alignment script (Compute Fully Opened Beam) */
    const std::string dataLogFileName = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                              clientConfiguration_->getPath(),
                                                                                              "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                              "DATA_LOG_FILENAME");
    clientPostProcessing_->executeScript2(pathToComputeFullyOpenedBeamScript_string,
                                          dataLogFileName,
                                          pathToResultFullyOpenedBeam);
    /* Read Fully Opened Beam Value */
    float fullyOpenedBeamValue = clientSensors_->readCsvResult(pathToResultFullyOpenedBeam);
    std::string fullyOpenedBeamValue_string = std::to_string(fullyOpenedBeamValue);
    /* Go to Yin position */
    initialPositionYAxis = initialPositionYAxis - clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                       "CRYSTAL_WIDTH") / 2;
    clientHxp_->setCoordinateY(initialPositionYAxis);
    clientHxp_->setCoordinateW(initialPositionWAxis);
    this->setHxpPositionAbsolute();
    /* --- [Yin Computation] W Scans in Y0 - (Crystal W / 2) --- */
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_SCAN_HXP_Y");  // Step Size
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                  "RANGE_SCAN_HXP_Y");  // Range
    float num_steps_yaxis = (stopOffset / stepSizeOffeset) + 1;
    std::string num_steps_yaxis_string = std::to_string(num_steps_yaxis);
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextYAxisPosition = initialPositionYAxis + i;
        clientHxp_->setCoordinateY(nextYAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionY(), nextYAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        clientPostProcessing_->executeScript6(pathToYAxisFineAlignmentScript_string,
                                              string_actual_YCoordinate,
                                              dataLogFileName,
                                              pathToResultYAlignment,
                                              pathToYinFineAlignment,
                                              num_steps_yaxis_string,
                                              fullyOpenedBeamValue_string);
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read Yin Position */
    float yInPosition = clientSensors_->readCsvResult(pathToResultYAlignment);
    clientSensors_->flushCsv(pathToResultYAlignment);  // Erase content
    /* --- [Yout Computation] W Scans in Y0 + (Crystal W / 2) --- */
    clientHxp_->setCoordinateY(initialPositionYAxis + clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                           clientConfiguration_->getPath(),
                                                                                                           "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                           "CRYSTAL_WIDTH"));
    clientHxp_->setCoordinateW(initialPositionWAxis);
    result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    initialPositionYAxis = clientHxp_->getCoordinateY();
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextYAxisPosition = initialPositionYAxis - i;
        clientHxp_->setCoordinateY(nextYAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionY(), nextYAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        clientPostProcessing_->executeScript6(pathToYAxisFineAlignmentScript_string,
                                              string_actual_YCoordinate,
                                              dataLogFileName,
                                              pathToResultYAlignment,
                                              pathToYoutFineAlignment,
                                              num_steps_yaxis_string,
                                              fullyOpenedBeamValue_string);
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read Yout Position */
    float yOutPosition = clientSensors_->readCsvResult(pathToResultYAlignment);
    /* --- Compute new Y position and update .ini file --- */
    float yPosFineAlignment = (yInPosition + yOutPosition) / 2;
    spdlog::debug("yPos - FineAlignment: {}.\n", yPosFineAlignment);
    clientHxp_->setCoordinateY(yPosFineAlignment);
    int result_writeYAlignmentPosition = clientConfiguration_->writeConfigurationFile(std::to_string(yPosFineAlignment),
                                                                                      "CRYSTAL_HXP",
                                                                                      "ALIGNMENT_POSITION_HXP_Y",
                                                                                      clientConfiguration_->getConfigFilename(),
                                                                                      clientConfiguration_->getPath());

    if (result_writeYAlignmentPosition == 0) {
        return false;
    }
    /* Final movement to alignment position */
    return this->setHxpPositionAbsolute();
}

bool Actions::compensateAndReAlign(float compensation) {
    spdlog::info("Method compensateAndReAlign of class Actions Crystal - compensation of: {}.\n", compensation);
    /* Compensate */
    clientHxp_->setCoordinateY(compensation);
    this->setStepperPosition(0);
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    /* Re-Align X Axis */
    clientScanningHXP_->hxpAxisToScan_ = 1;
    clientScanningHXP_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                      clientConfiguration_->getPath(),
                                                                                                      "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                      "STEP_SIZE"),
                                                 clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                      clientConfiguration_->getPath(),
                                                                                                      "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                      "RANGE"),
                                                 clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                    "DURATION_ACQUISITION"),
                                                 clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                       clientConfiguration_->getPath(),
                                                                                                       "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                       "DATA_LOG_FILENAME"),
                                                 clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                                     "ERASE_CSV_CONTENT"),
                                                  false);
    bool result_x_alignment = this->searchXAxisAlignmentCrystal();
    if (!result_x_alignment) {
        return false;
    }
    /* Re-Align Y Axis */
    clientScanningHXP_->hxpAxisToScan_ = 6;
    /*--- Alignment Parameters ---*/
    clientScanningHXP_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                      clientConfiguration_->getPath(),
                                                                                                      "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                      "STEP_SIZE"),
                                                clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                     "RANGE"),
                                                clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                   clientConfiguration_->getPath(),
                                                                                                   "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                   "DURATION_ACQUISITION"),
                                                clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                      clientConfiguration_->getPath(),
                                                                                                      "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                      "DATA_LOG_FILENAME"),
                                                clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                                    "ERASE_CSV_CONTENT"),
                                                false);
    bool result_y_alignment = this->searchYAxisFineAlignment();
    if (!result_y_alignment) {
        return false;
    }
    /* Re-Check Alignment Y Axis In flipped orientation */
    this->setStepperPosition(180);
    result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    return this->checkAlignmentFlippedOrientation();
}

bool Actions::checkAlignmentFlippedOrientation() {
    float initial_y_alignment_pos = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                         clientConfiguration_->getPath(),
                                                                                         "CRYSTAL_HXP",
                                                                                         "ALIGNMENT_POSITION_HXP_Y");  // Read initial y axis alignment position
    clientHxp_->setCoordinateW(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") -
                               clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
    bool result_Y_fine_alignment = this->searchYAxisFineAlignment();
    if (!result_Y_fine_alignment) {
        return false;
    }
    float flipped_y_alignment_pos = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                         clientConfiguration_->getPath(),
                                                                                         "CRYSTAL_HXP",
                                                                                         "ALIGNMENT_POSITION_HXP_Y");  // Read initial y axis alignment position
    float thresholdDifference = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                     clientConfiguration_->getPath(),
                                                                                     "checkAlignmentInFlippedOrientation_CRYSTAL_STAGE",
                                                                                     "THRESHOLD");
    float differenceYPositions = flipped_y_alignment_pos - initial_y_alignment_pos;
    spdlog::debug("differenceYPositions: {}\n", differenceYPositions);
    if (differenceYPositions > thresholdDifference) {
        bool result_compensateAndReAlign = this->compensateAndReAlign(differenceYPositions);
        if (!result_compensateAndReAlign) {
            return false;
        }
        return false;
    }
    return true;
}

bool Actions::bendingAngleMeasurement() {
    spdlog::info("Method bendingAngleMeasurement Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextYAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "Bending_Angle_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");

    std::filesystem::path pathToScriptName = pathToScriptMeasurementsDirectory_ / scriptName;
    const std::string pathToBendingAngleMeasurementScript_string = pathToScriptName.string();
    /* Setup .csvs result */
    const std::string pathToResultBendingAngle = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                 + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                         clientConfiguration_->getPath(),
                                                                                                         "Bending_Angle_CRYSTAL_STAGE",
                                                                                                         "FILENAME_TO_BENDING_ANGLE");
    const std::string pathToPeaks = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                    + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                            clientConfiguration_->getPath(),
                                                                                            "Bending_Angle_CRYSTAL_STAGE",
                                                                                            "FILENAME_TO_PEAKS");
    clientSensors_->flushCsv(pathToResultBendingAngle);  // Erase content
    clientSensors_->flushCsv(pathToPeaks);               // Erase content
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "Bending_Angle_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_SCAN_HXP_Y");  // Step Size
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "Bending_Angle_CRYSTAL_STAGE",
                                                                                  "RANGE_SCAN_HXP_Y");  // Range
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextYAxisPosition = initialPositionYAxis + i;
        clientHxp_->setCoordinateY(nextYAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionY(), nextYAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        const std::string dataLogFilename = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "Bending_Angle_CRYSTAL_STAGE",
                                                                                                  "DATA_LOG_FILENAME");
        clientPostProcessing_->executeScript6(pathToBendingAngleMeasurementScript_string,
                                              string_actual_YCoordinate,
                                              dataLogFilename,
                                              pathToResultBendingAngle,
                                              pathToPeaks,
                                              std::to_string(stopOffset),
                                              std::to_string(crystalWidth_));
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read Bending Angle and update .ini file */
    float bendingAngle = clientSensors_->readCsvResult(pathToResultBendingAngle);
    int result_BendingAngle = clientConfiguration_->writeConfigurationFile(std::to_string(bendingAngle),
                                                                           "CRYSTAL_MEASUREMENTS",
                                                                           "BENDING_ANGLE",
                                                                           clientConfiguration_->getConfigFilename(),
                                                                           clientConfiguration_->getPath());
    if (result_BendingAngle == 0) {
        return false;
    }
    return true;
}

bool Actions::miscutAngleMeasurement() {
    spdlog::info("Method miscutAngleMeasurement Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextYAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");

    std::filesystem::path pathToScriptName = pathToScriptMeasurementsDirectory_ / scriptName;
    const std::string pathToMiscutAngleMeasurementScript_string = pathToScriptName.string();
    /* Setup .csvs result */
    const std::string pathToResultMiscutAngle = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                        clientConfiguration_->getPath(),
                                                                                                        "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                        "FILENAME_TO_MISCUT_ANGLE");
    const std::string pathToPeaks = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                    + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                            clientConfiguration_->getPath(),
                                                                                            "Miscut_Angle_CRYSTAL_STAGE",
                                                                                            "FILENAME_TO_PEAKS");
    clientSensors_->flushCsv(pathToResultMiscutAngle);  // Erase content
    clientSensors_->flushCsv(pathToPeaks);               // Erase content
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "Miscut_Angle_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_SCAN_HXP_Y");  // Step Size
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "Miscut_Angle_CRYSTAL_STAGE",
                                                                                  "RANGE_SCAN_HXP_Y");  // Range
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextYAxisPosition = initialPositionYAxis + i;
        clientHxp_->setCoordinateY(nextYAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionY(), nextYAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        std::string dataLogPeaksFilename_bending =  pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                    + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "Bending_Angle_CRYSTAL_STAGE",
                                                                                                            "FILENAME_TO_PEAKS");
        const std::string dataLogFilename = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                  "DATA_LOG_FILENAME");
        clientPostProcessing_->executeScript7(pathToMiscutAngleMeasurementScript_string,
                                              string_actual_YCoordinate,
                                              dataLogFilename,
                                              pathToResultMiscutAngle,
                                              pathToPeaks,
                                              std::to_string(stopOffset),
                                              std::to_string(crystalWidth_),
                                              dataLogPeaksFilename_bending);
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read Miscut Angle and update .ini file */
    float miscutAngle = clientSensors_->readCsvResult(pathToResultMiscutAngle);
    int result_MiscutAngle = clientConfiguration_->writeConfigurationFile(std::to_string(miscutAngle),
                                                                           "CRYSTAL_MEASUREMENTS",
                                                                           "MISCUT_ANGLE",
                                                                           clientConfiguration_->getConfigFilename(),
                                                                           clientConfiguration_->getPath());
    if (result_MiscutAngle == 0) {
        return false;
    }
    return true;
}

bool Actions::torsionAngleMeasurement() {
    spdlog::info("Method torsionAngleMeasurement Crystal of class Actions\n");
    /* Initial Movement */
    double initialPositionYAxis = clientHxp_->getCoordinateY();
    double initialPositionZAxis = clientHxp_->getCoordinateZ();
    double initialPositionWAxis = clientHxp_->getCoordinateW();
    double nextZAxisPosition;
    bool result_movement = this->moveBothMotors();
    if (!result_movement) {
        return false;
    }
    std::filesystem::path scriptName = clientConfiguration_->readFileSystemPathFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                     clientConfiguration_->getPath(),
                                                                                                     "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                     "SCRIPT_NAME");

    std::filesystem::path pathToScriptName = pathToScriptMeasurementsDirectory_ / scriptName;
    const std::string pathToTorsionAngleMeasurementScript_string = pathToScriptName.string();
    /* Setup .csvs result */
    const std::string pathToResultTorsionAngle = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                                 + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                         clientConfiguration_->getPath(),
                                                                                                         "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                         "FILENAME_TO_TORSION_ANGLE");
    const std::string pathToPeaks = pathToCrystalAlinmentResultsDirectory_ + "\\"
                                    + clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                            clientConfiguration_->getPath(),
                                                                                            "Torsion_Angle_CRYSTAL_STAGE",
                                                                                            "FILENAME_TO_PEAKS");
    clientSensors_->flushCsv(pathToResultTorsionAngle);  // Erase content
    clientSensors_->flushCsv(pathToPeaks);               // Erase content
    const float stepSizeOffeset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                       clientConfiguration_->getPath(),
                                                                                       "Torsion_Angle_CRYSTAL_STAGE",
                                                                                       "STEP_SIZE_SCAN_HXP_Z");  // Step Size
    const float stopOffset = clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                  clientConfiguration_->getPath(),
                                                                                  "Torsion_Angle_CRYSTAL_STAGE",
                                                                                  "RANGE_SCAN_HXP_Z");  // Range
    for (double i = 0; i <= stopOffset; i = i + stepSizeOffeset) {
        nextZAxisPosition = initialPositionZAxis + i;
        clientHxp_->setCoordinateZ(nextZAxisPosition);
        result_movement = this->setHxpPositionAbsolute();
        if (!result_movement && !clientScanningHXP_->checkReachingPosition(clientHxp_->getPositionZ(), nextZAxisPosition)) {
            return false;
        }
        bool result_scan = clientScanningHXP_->scan();  // W Scan
        if (!result_scan) {
            return false;
        }
        /* Execute Alignment script */
        std::string string_actual_ZCoordinate = std::to_string(clientHxp_->getCoordinateZ());
        std::string string_actual_YCoordinate = std::to_string(clientHxp_->getCoordinateY());
        const std::string dataLogFilename = clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                  clientConfiguration_->getPath(),
                                                                                                  "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                  "DATA_LOG_FILENAME");
        clientPostProcessing_->executeScript7(pathToTorsionAngleMeasurementScript_string,
                                              string_actual_ZCoordinate,
                                              dataLogFilename,
                                              pathToResultTorsionAngle,
                                              pathToPeaks,
                                              std::to_string(stopOffset),
                                              std::to_string(crystalWidth_),
                                              string_actual_YCoordinate);
        clientHxp_->setCoordinateW(initialPositionWAxis);  // return to initial position
    }
    /* Read Torsion Angle and update .ini file */
    float torsionAngle = clientSensors_->readCsvResult(pathToResultTorsionAngle);
    int result_TorsionAngle = clientConfiguration_->writeConfigurationFile(std::to_string(torsionAngle),
                                                                           "CRYSTAL_MEASUREMENTS",
                                                                           "TORSION_ANGLE",
                                                                           clientConfiguration_->getConfigFilename(),
                                                                           clientConfiguration_->getPath());
    if (result_TorsionAngle == 0) {
        return false;
    }
    return true;
}

bool Actions::startScanStepper() {
    return clientScanningStepper_->scan();
}

bool Actions::stop() {
    spdlog::info("Method stop Crystal of class Actions\n");
    bool result_stop1_HXP = clientScanningHXP_->stop();
    bool result_stop2_stepper = clientScanningStepper_->stop();
    if (result_stop1_HXP == true && result_stop2_stepper == true) {
        return true;
    } else {
        return false;
    }
}

bool Actions::startScanHxp() {
    return clientScanningHXP_->scan();
}

float Actions::getStepperPosition() {
    return clientStepper_->getPositionUserUnits();
}

void Actions::setStepperPosition(float stepperPosition) {
    stepperPosition_ = stepperPosition;
}

}  // namespace crystal
