/**
 * @file CrystalDeviceController.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'ICrystalDeviceController'.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <string>

#include "CrystalDeviceController.hpp"

namespace crystal {

CrystalDeviceController::CrystalDeviceController(std::filesystem::path logFilePath,
                                                 std::shared_ptr<IHXP> clientHXP,
                                                 std::shared_ptr<IMotor> clientStepper,
                                                 std::shared_ptr<scanning::IScanning> clientScanningHXP,
                                                 std::shared_ptr<scanning::IScanning> clientScanningStepper,
                                                 std::shared_ptr<sensors::ISensors> clientSensors,
                                                 std::shared_ptr<IConfiguration> clientConfiguration,
                                                 std::shared_ptr<IPostProcessing> clientPostProcessing):
    logFilePath_(logFilePath),
    actionsPtr_(std::make_shared<Actions>(clientHXP,
                                          clientStepper,
                                          clientScanningHXP,
                                          clientScanningStepper,
                                          clientSensors,
                                          clientConfiguration,
                                          clientPostProcessing)),
    clientScanningHxp_(clientScanningHXP),
    clientScanningStepper_(clientScanningStepper),
    clientHxp_(clientHXP),
    clientStepper_(clientStepper),
    clientConfiguration_(clientConfiguration),
    sMachine_(System(actionsPtr_)) {
    spdlog::info("cTor CrystalDeviceController\n");
}


CrystalDeviceController::~CrystalDeviceController() {
    spdlog::info("dTor CrystalDeviceController\n");
}

bool CrystalDeviceController::start() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::disconnect() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }
    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::goHome() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemHome>)) {
        sMachine_.process_event(eventSetupHome{});
        sMachine_.process_event(eventSetupHome{});
    }
    if (!sMachine_.is(state<systemHome>)) {
        return false;
    }
    return true;
}

float CrystalDeviceController::getPositionStepper() {
    return actionsPtr_->getStepperPosition();
}

double CrystalDeviceController::getPositionX() {
    return clientHxp_->getCoordinateX();
}

double CrystalDeviceController::getPositionY() {
    return clientHxp_->getCoordinateY();
}

double CrystalDeviceController::getPositionZ() {
    return clientHxp_->getCoordinateZ();
}

double CrystalDeviceController::getPositionU() {
    return clientHxp_->getCoordinateU();
}

double CrystalDeviceController::getPositionV() {
    return clientHxp_->getCoordinateV();
}

double CrystalDeviceController::getPositionW() {
    return clientHxp_->getCoordinateW();
}

bool CrystalDeviceController::moveToAbsPosition(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        clientHxp_->setHxpCoordinates(CoordX, CoordY, CoordZ, CoordU, CoordV, CoordW);
        sMachine_.process_event(eventMoveHxpToAbsolutePosition{});
        sMachine_.process_event(eventMoveHxpToAbsolutePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::scanHxp(int axis, float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        clientScanningHxp_->hxpAxisToScan_ = axis;
        clientScanningHxp_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanHxp{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventStartScanHxp{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        clientScanningStepper_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanStepper{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventStartScanStepper{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::alignSourceWithSensor() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        clientHxp_->setHxpCoordinates(17, 0, 0, 0, 0, 0);
        sMachine_.process_event(eventMoveHxpToAbsolutePosition{});
        sMachine_.process_event(eventMoveHxpToAbsolutePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::moveToPositionStepper(float position) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        actionsPtr_->setStepperPosition(position);
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::xAxisAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Positions Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_W"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::X;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
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
        sMachine_.process_event(eventXAxisAlignmentCrystal{});
        sMachine_.process_event(eventXAxisAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::yAxisAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_W")
                                                                                           );
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "yAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::Y;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
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
        sMachine_.process_event(eventYAxisAlignmentCrystal{});
        sMachine_.process_event(eventYAxisAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::zAxisAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_W"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::Z;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "zAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventZAxisAlignmentCrystal{});
        sMachine_.process_event(eventZAxisAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::yWAxesAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_SCAN_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_W"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "yWAxis_Alignment_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventYWAxesAlignmentCrystal{});
        sMachine_.process_event(eventYWAxesAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::xAxisFineAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                          "START_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::X;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
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
        sMachine_.process_event(eventXAxisAlignmentCrystal{});
        sMachine_.process_event(eventXAxisAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::braggPeakSearchCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_W"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Braggs_Peak_Search_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventSearchBraggPeakCrystal{});
        sMachine_.process_event(eventSearchBraggPeakCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::yAxisFineAlignmentCrystal() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") -
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
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
        sMachine_.process_event(eventYAxisFineAlignmentCrystal{});
        sMachine_.process_event(eventYAxisFineAlignmentCrystal{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::checkAlignmentInFlippedOrientation() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") -
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "yAxis_Fine_Alignment_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
        actionsPtr_->setStepperPosition(180);
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
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
        sMachine_.process_event(eventCheckAlignmentInFlippedOrientation{});
        sMachine_.process_event(eventCheckAlignmentInFlippedOrientation{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::bendingAngleMeasurement() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") +
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Bending_Angle_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bending_Angle_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bending_Angle_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bending_Angle_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bending_Angle_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bending_Angle_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventBendingAngleMeasurement{});
        sMachine_.process_event(eventBendingAngleMeasurement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::miscutAngleMeasurement(bool repeateBendingAngleMeas) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        if (repeateBendingAngleMeas) {
            this->bendingAngleMeasurement();
        }
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") +
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Miscut_Angle_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "Miscut_Angle_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Miscut_Angle_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventMiscutAngleMeasurement{});
        sMachine_.process_event(eventMiscutAngleMeasurement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::torsionAngleMeasurement() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        /*--- Initial Position Setup ---*/
        clientHxp_->setHxpCoordinates(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                          clientConfiguration_->getPath(),
                                                                                          "CRYSTAL_HXP",
                                                                                          "ALIGNMENT_POSITION_HXP_X"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Torsion_Angle_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Y"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Torsion_Angle_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_Z"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_U"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "xAxis_Alignment_CRYSTAL_STAGE",
                                                                                           "START_POSITION_HXP_V"),
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "CRYSTAL_HXP",
                                                                                           "ALIGNMENT_POSITION_HXP_W") +
                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                           clientConfiguration_->getPath(),
                                                                                           "Torsion_Angle_CRYSTAL_STAGE",
                                                                                           "OFFSET_FROM_BRAGG_ANGLE"));
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "Torsion_Angle_CRYSTAL_STAGE",
                                                                                             "START_POSITION_STEPPER"));
        /*--- Axis to Scan ---*/
        clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::W;
        /*--- Alignment Parameters ---*/
        clientScanningHxp_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                          "STEP_SIZE"),
                                                     clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                          "RANGE"),
                                                     clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                          "DURATION_ACQUISITION"),
                                                     clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                          "DATA_LOG_FILENAME"),
                                                     clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Torsion_Angle_CRYSTAL_STAGE",
                                                                                                          "ERASE_CSV_CONTENT"),
                                                     false);
        sMachine_.process_event(eventTorsionAngleMeasurement{});
        sMachine_.process_event(eventTorsionAngleMeasurement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool CrystalDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string CrystalDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("CrystalDeviceController FSM state is: {}\n", temp);
    return temp;
}

CrystalStatus CrystalDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        crystalStatus_ = CrystalStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        crystalStatus_ = CrystalStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        crystalStatus_ = CrystalStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        crystalStatus_ = CrystalStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        crystalStatus_ = CrystalStatus::Error;
    } else {
        crystalStatus_ = CrystalStatus::NotInitialized;
    }
    return crystalStatus_;
}

}  // namespace crystal


