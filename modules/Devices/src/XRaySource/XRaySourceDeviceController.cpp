/**
 * @file XRaySourceDeviceController.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'ISingleStepperDeviceController'.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <string>

#include "XRaySourceDeviceController.hpp"

namespace xRaySource {

XRaySourceDeviceController::XRaySourceDeviceController(std::filesystem::path logFilePath,
                                                       std::shared_ptr<IMotor> clientStepper,
                                                       std::shared_ptr<scanning::IScanning> clientScanning,
                                                       std::shared_ptr<sensors::ISensors> clientSensors,
                                                       std::shared_ptr<IConfiguration> clientConfiguration,
                                                       std::shared_ptr<IPostProcessing> clientPostProcessing) :
    logFilePath_(logFilePath),
    actionsPtr_(std::make_shared<Actions>(clientStepper, clientScanning, clientSensors, clientConfiguration, clientPostProcessing)),
    scanningPtr_(clientScanning),
    clientConfiguration_(clientConfiguration),
    sMachine_(System(actionsPtr_)) {
    spdlog::info("cTor XRaySourceDeviceController\n");
}


XRaySourceDeviceController::~XRaySourceDeviceController() {
    spdlog::info("dTor XRaySourceDeviceController\n");
}

bool XRaySourceDeviceController::start() {
     using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::disconnectStp() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }

    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::goHome() {
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

float XRaySourceDeviceController::getPositionStepper() {
    return actionsPtr_->getStepperPosition();
}

bool XRaySourceDeviceController::moveToPositionStepper(float position) {
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

bool XRaySourceDeviceController::scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        scanningPtr_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanStepper{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventStartScanStepper{});  // event that allows to start the scan
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::alignSourceWithSensor(bool searchCenter) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setSearchCenter(searchCenter);
        scanningPtr_->setupAlignmentParameters(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                    "STEP_SIZE"),
                                               clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                    "RANGE"),
                                               clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                    "DURATION_ACQUISITION"),
                                               clientConfiguration_->readStringFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                    "DATA_LOG_FILENAME"),
                                               clientConfiguration_->readBoolFromConfigurationFile(clientConfiguration_->getAlignmentSettingsConfigFilename(),
                                                                                                    clientConfiguration_->getPath(),
                                                                                                    "XRAY_SOURCE_STAGE_ROTATIONAL",
                                                                                                    "ERASE_CSV_CONTENT"),
                                               false);
        sMachine_.process_event(eventAlignSourceWithSensor{});  // event that allows transition to temp state "systemInMotion"
        sMachine_.process_event(eventAlignSourceWithSensor{});  // event that allows to start the alignment
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::setupMonochromatorBraggPeakSearch() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "X-RAY_SOURCE",
                                                                                             "2THETA"));
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::setupForCrystalBraggPeakSearch() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "X-RAY_SOURCE",
                                                                                             "2THETA"));
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySourceDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string XRaySourceDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("XRaySourceDeviceController FSM state is: {}\n", temp);
    return temp;
}

XRaySourceStatus XRaySourceDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        xRaySourceStatus_ = XRaySourceStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        xRaySourceStatus_ = XRaySourceStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        xRaySourceStatus_ = XRaySourceStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        xRaySourceStatus_ = XRaySourceStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        xRaySourceStatus_ = XRaySourceStatus::Error;
    } else {
        xRaySourceStatus_ = XRaySourceStatus::NotInitialized;
    }
    return xRaySourceStatus_;
}

}  // namespace xRaySource
