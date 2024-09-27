/**
 * @file XRaySensorDeviceController.cpp
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

#include "XRaySensorDeviceController.hpp"

namespace xRaySensor {

XRaySensorDeviceController::XRaySensorDeviceController(std::filesystem::path logFilePath,
                                                       std::shared_ptr<IMotor> clientStepper,
                                                       std::shared_ptr<scanning::IScanning> clientScanning,
                                                       std::shared_ptr<IConfiguration> clientConfiguration):
    logFilePath_(logFilePath),
    actionsPtr_(std::make_shared<Actions>(clientStepper, clientScanning)),
    scanningPtr_(clientScanning),
    sMachine_(System(actionsPtr_)),
    clientConfiguration_(clientConfiguration) {
    spdlog::info("cTor XRaySensorDeviceController\n");
}


XRaySensorDeviceController::~XRaySensorDeviceController() {
    spdlog::info("dTor XRaySensorDeviceController\n");
}

bool XRaySensorDeviceController::start() {
     using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::disconnectStp() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }
    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::goHome() {
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

float XRaySensorDeviceController::getPositionStepper() {
    return actionsPtr_->getStepperPosition();
}

bool XRaySensorDeviceController::moveToPositionStepper(float position) {
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

bool XRaySensorDeviceController::scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        scanningPtr_->setupAlignmentParameters(stepSize, range, durationAcquisition, filename, eraseCsvContent, showPlot);
        sMachine_.process_event(eventStartScanStepper{});
        sMachine_.process_event(eventStartScanStepper{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}


bool XRaySensorDeviceController::alignSourceWithSensor(bool searchCenter) {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepperPosition(0);  // middle point
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::setupMonochromatorBraggPeakSearch() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepperPosition(0);
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::setupForCrystalBraggPeakSearch() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>) || sMachine_.is(state<systemNotInitialized>)) {
        actionsPtr_->setStepperPosition(clientConfiguration_->readFloatFromConfigurationFile(clientConfiguration_->getConfigFilename(),
                                                                                             clientConfiguration_->getPath(),
                                                                                             "X-RAY_SENSOR",
                                                                                             "2THETA"));
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
        sMachine_.process_event(eventMoveStepperToRelativePosition{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool XRaySensorDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string XRaySensorDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("XRaySensorDeviceController FSM state is: {}\n", temp);
    return temp;
}

XRaySensorStatus XRaySensorDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        xRaySensorStatus_ = XRaySensorStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        xRaySensorStatus_ = XRaySensorStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        xRaySensorStatus_ = XRaySensorStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        xRaySensorStatus_ = XRaySensorStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        xRaySensorStatus_ = XRaySensorStatus::Error;
    } else {
        xRaySensorStatus_ = XRaySensorStatus::NotInitialized;
    }
    return xRaySensorStatus_;
}

}  // namespace xRaySensor


