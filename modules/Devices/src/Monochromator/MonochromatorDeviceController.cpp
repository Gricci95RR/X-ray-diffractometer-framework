/* © Copyright CERN 2022.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Gianmarco Ricci CERN BE/CEM/MRO 2022
 *         
 *  ===============================================================================================
 */

#include <string>

#include "MonochromatorDeviceController.hpp"

namespace monochromator {

MonochromatorDeviceController::MonochromatorDeviceController(std::filesystem::path logFilePath,
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
    spdlog::info("cTor MonochromatorDeviceController\n");
}

MonochromatorDeviceController::~MonochromatorDeviceController() {
    spdlog::info("dTor MonochromatorDeviceController\n");
}

bool MonochromatorDeviceController::start() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }

    return true;
}

bool MonochromatorDeviceController::disconnectStp() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }
    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::goHome() {
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

float MonochromatorDeviceController::getPositionStepperLinear() {
    return actionsPtr_->getStepper1LinearPosition();
}

float MonochromatorDeviceController::getPositionStepperRotational() {
    return actionsPtr_->getStepper2RotationalPosition();
}

bool MonochromatorDeviceController::moveToPositionStepper1Linear(float position) {
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

bool MonochromatorDeviceController::moveToPositionStepper2Rotational(float position) {
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

bool MonochromatorDeviceController::scanStepper1Linear(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
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

bool MonochromatorDeviceController::scanStepper2Rotational(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
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

bool MonochromatorDeviceController::alignSourceWithSensor() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepper2RotationalPosition(0);
        actionsPtr_->setStepper1LinearPosition(0);
        sMachine_.process_event(eventAlignSourceWithSensor{});
        sMachine_.process_event(eventAlignSourceWithSensor{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::alignMonochromator(bool searchCenter) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setSearchCenter(searchCenter);
        /* Initial Position */
        actionsPtr_->setStepper1LinearPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                    "START_POSITION"));

        /*--- Alignment Parameters ---*/
        scanningPtr1Linear_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                          "STEP_SIZE"),
                                                      clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                            "RANGE"),
                                                      clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                            "DURATION_ACQUISITION"),
                                                      clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                            "DATA_LOG_FILENAME"),
                                                      clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                            clientConfiguration_->getPath(),
                                                                                                            "1st_Linear_Alignment_MONOCHROMATOR_STAGE_LINEAR",
                                                                                                            "ERASE_CSV_CONTENT"),
                                                      false);

        scanningPtr2Rotational_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                          "STEP_SIZE"),
                                                          clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                               clientConfiguration_->getPath(),
                                                                                                               "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                               "RANGE"),
                                                          clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                             clientConfiguration_->getPath(),
                                                                                                             "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                             "DURATION_ACQUISITION"),
                                                          clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                clientConfiguration_->getPath(),
                                                                                                                "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                "DATA_LOG_FILENAME"),
                                                          clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                              clientConfiguration_->getPath(),
                                                                                                              "2nd_Linear_Alignment_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                              "ERASE_CSV_CONTENT"),
                                                      false);

        sMachine_.process_event(eventAlignMonochromator{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventAlignMonochromator{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::searchMonochromatorBraggPeak() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepper2RotationalPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                    "THETA"));
        scanningPtr2Rotational_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                          clientConfiguration_->getPath(),
                                                                                                          "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                          "STEP_SIZE"),
                                                          clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                               clientConfiguration_->getPath(),
                                                                                                               "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                               "RANGE"),
                                                          clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                             clientConfiguration_->getPath(),
                                                                                                             "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                             "DURATION_ACQUISITION"),
                                                          clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                                clientConfiguration_->getPath(),
                                                                                                                "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                                "DATA_LOG_FILENAME"),
                                                          clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                              clientConfiguration_->getPath(),
                                                                                                              "Bragg_Peak_Search_MONOCHROMATOR_STAGE_ROTATIONAL",
                                                                                                              "ERASE_CSV_CONTENT"),
                                                          false);
        sMachine_.process_event(eventSearchMonochromatorBraggPeak{});
        sMachine_.process_event(eventSearchMonochromatorBraggPeak{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::alignSlit(bool searchCenter) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        //  Do nothing
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool MonochromatorDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string MonochromatorDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("MonochromatorDeviceController FSM state is: {}\n", temp);
    return temp;
}

MonochromatorStatus MonochromatorDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        monochromatorStatus_ = MonochromatorStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        monochromatorStatus_ = MonochromatorStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        monochromatorStatus_ = MonochromatorStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        monochromatorStatus_ = MonochromatorStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        monochromatorStatus_ = MonochromatorStatus::Error;
    } else {
        monochromatorStatus_ = MonochromatorStatus::NotInitialized;
    }
    return monochromatorStatus_;
}

}  // namespace monochromator

