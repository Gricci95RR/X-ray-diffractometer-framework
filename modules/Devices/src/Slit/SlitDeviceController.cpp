/**
 * @file SlitDeviceController.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'IMultiStepperDeviceController'.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <string>

#include "SlitDeviceController.hpp"

namespace slit {

SlitDeviceController::SlitDeviceController(std::filesystem::path logFilePath,
                                           std::shared_ptr<IMotor> clientStepper1Linear,
                                           std::shared_ptr<IMotor> clientStepper2Rotational,
                                           std::shared_ptr<scanning::IScanning> clientScanning1Linear,
                                           std::shared_ptr<scanning::IScanning> clientScanning2Rotational,
                                           std::shared_ptr<sensors::ISensors> clientSensors,
                                           std::shared_ptr<IConfiguration> clientConfiguration,
                                           std::shared_ptr<IPostProcessing> clientPostProcessing):
    logFilePath_(logFilePath),
    actionsPtr_(std::make_shared<Actions>(clientStepper1Linear,
                                          clientStepper2Rotational,
                                          clientScanning1Linear,
                                          clientScanning2Rotational,
                                          clientSensors,
                                          clientConfiguration,
                                          clientPostProcessing)),
    scanningPtr1Linear_(clientScanning1Linear),
    scanningPtr2Rotational_(clientScanning2Rotational),
    clientConfiguration_(clientConfiguration),
    sMachine_(System(actionsPtr_)) {
    spdlog::info("cTor SlitDeviceController\n");
}


SlitDeviceController::~SlitDeviceController() {
    spdlog::info("dTor SlitDeviceController\n");
}

bool SlitDeviceController::start() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }

    return true;
}

bool SlitDeviceController::disconnectStp() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }
    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::goHome() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventSetupHome{});
        sMachine_.process_event(eventSetupHome{});
    }
    if (!sMachine_.is(state<systemHome>)) {
        return false;
    }
    return true;
}

float SlitDeviceController::getPositionStepperLinear() {
    return actionsPtr_->getStepper1LinearPosition();
}

float SlitDeviceController::getPositionStepperRotational() {
    return actionsPtr_->getStepper2RotationalPosition();
}

bool SlitDeviceController::moveToPositionStepper1Linear(float position) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        actionsPtr_->setStepper1LinearPosition(position);
        sMachine_.process_event(eventMoveStepper1LinearToRelativePosition{});
        sMachine_.process_event(eventMoveStepper1LinearToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::moveToPositionStepper2Rotational(float position) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        actionsPtr_->setStepper2RotationalPosition(position);
        sMachine_.process_event(eventMoveStepper2RotationalToRelativePosition{});
        sMachine_.process_event(eventMoveStepper2RotationalToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::scanStepper1Linear(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        scanningPtr1Linear_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanStepper1Linear{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventStartScanStepper1Linear{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::scanStepper2Rotational(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        scanningPtr2Rotational_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanStepper2Rotational{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventStartScanStepper2Rotational{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::alignSourceWithSensor() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepper1LinearPosition(0);
        actionsPtr_->setStepper2RotationalPosition(0);
        sMachine_.process_event(eventMoveBothMotors{});
        sMachine_.process_event(eventMoveBothMotors{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::alignMonochromator(bool searchCenter) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepper1LinearPosition(0);
        actionsPtr_->setStepper2RotationalPosition(0);
        sMachine_.process_event(eventMoveBothMotors{});
        sMachine_.process_event(eventMoveBothMotors{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::searchMonochromatorBraggPeak() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepper1LinearPosition(0);
        actionsPtr_->setStepper2RotationalPosition(0);
        sMachine_.process_event(eventMoveBothMotors{});
        sMachine_.process_event(eventMoveBothMotors{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::alignSlit(bool alignDevice) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        /* Initial Positions */
        actionsPtr_->setStepper2RotationalPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                        clientConfiguration_->getPath(),
                                                                                                        "Rotational_Alignment_SLIT_STAGE_ROTATIONAL",
                                                                                                        "START_POSITION"));
        actionsPtr_->setStepper1LinearPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                        clientConfiguration_->getPath(),
                                                                                                        "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                        "START_POSITION"));
        /* Setup Alignment */
        actionsPtr_->setAlignDevice(alignDevice);
        scanningPtr1Linear_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                          "STEP_SIZE"),
                                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                           clientConfiguration_->getPath(),
                                                                                                            "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                            "RANGE"),
                                                      clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                         clientConfiguration_->getPath(),
                                                                                                         "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                         "DURATION_ACQUISITION"),
                                                      clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                            "DATA_LOG_FILENAME"),
                                                      clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "Linear_Alignment_SLIT_STAGE_LINEAR",
                                                                                                            "ERASE_CSV_CONTENT"),
                                                       false);
        sMachine_.process_event(eventAlignSlit{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventAlignSlit{});  // event that allows to start the alignment
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool SlitDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string SlitDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("SlitDeviceController FSM state is: {}\n", temp);
    return temp;
}

SlitStatus SlitDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        slitStatus_ = SlitStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        slitStatus_ = SlitStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        slitStatus_ = SlitStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        slitStatus_ = SlitStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        slitStatus_ = SlitStatus::Error;
    } else {
        slitStatus_ = SlitStatus::NotInitialized;
    }
    return slitStatus_;
}

}  // namespace slit

