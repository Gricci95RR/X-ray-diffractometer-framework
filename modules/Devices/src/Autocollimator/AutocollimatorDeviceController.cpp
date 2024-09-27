/**
 * @file AutocollimatorDeviceController.cpp
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

#include "AutocollimatorDeviceController.hpp"

namespace autocollimator {

AutocollimatorDeviceController::AutocollimatorDeviceController(std::filesystem::path logFilePath,
                                                               std::shared_ptr<IMotor> clientStepper,
                                                               std::shared_ptr<scanning::IScanning> clientScanning):
    logFilePath_(logFilePath),
    actionsPtr_(std::make_shared<Actions>(clientStepper, clientScanning)),
    scanningPtr_(clientScanning),
    sMachine_(System(actionsPtr_)) {
    spdlog::info("cTor AutocollimatorDeviceController\n");
}

AutocollimatorDeviceController::~AutocollimatorDeviceController() {
    spdlog::info("dTor AutocollimatorDeviceController\n");
}

bool AutocollimatorDeviceController::start() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>) || sMachine_.is(state<systemError>)) {
        sMachine_.process_event(eventInitialize{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool AutocollimatorDeviceController::disconnectStp() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventDisconnect{});
    }
    if (!sMachine_.is(state<systemNotInitialized>)) {
        return false;
    }
    return true;
}

bool AutocollimatorDeviceController::goHome() {
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

float AutocollimatorDeviceController::getPositionStepper() {
    return actionsPtr_->getStepperPosition();
}

bool AutocollimatorDeviceController::moveToPositionStepper(float position) {
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

bool AutocollimatorDeviceController::scanStepper(float stepSize,
                                                 float range,
                                                 int durationAcquisition,
                                                 std::string filename,
                                                 bool eraseCsvContent,
                                                 bool showPlot) {
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

bool AutocollimatorDeviceController::stop() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemConnected>) || sMachine_.is(state<systemHome>) || sMachine_.is(state<systemInMotion>)) {
        sMachine_.process_event(eventStopMovement{});
    }
    if (!sMachine_.is(state<systemConnected>)) {
        return false;
    }
    return true;
}

bool AutocollimatorDeviceController::alignSourceWithSensor(bool searchCenter) {
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

bool AutocollimatorDeviceController::setupMonochromatorBraggPeakSearch() {
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

bool AutocollimatorDeviceController::setupForCrystalBraggPeakSearch() {
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

bool AutocollimatorDeviceController::logger(std::string filename) {
    auto logger = spdlog::rotating_logger_mt(filename, logFilePath_.string(), max_size, max_files);
    spdlog::set_default_logger(logger);
    return true;
}

std::string AutocollimatorDeviceController::getFsmState() {
    static const char *EnumCrystalStatusStrings[] = { "Not Defined", "Not Initialized", "Connected", "Home", "In Motion", "Error" };
    std::string temp = EnumCrystalStatusStrings[static_cast<int>(this->getStatus())];
    spdlog::debug("AutocollimatorDeviceController FSM state is: {}\n", temp);
    return temp;
}

AutocollimatorStatus AutocollimatorDeviceController::getStatus() {
    using namespace boost::sml;  // NOLINT
    if (sMachine_.is(state<systemNotInitialized>)) {
        autocollimatorStatus_ = AutocollimatorStatus::NotInitialized;
    } else if (sMachine_.is(state<systemConnected>)) {
        autocollimatorStatus_ = AutocollimatorStatus::Connected;
    } else if (sMachine_.is(state<systemInMotion>)) {
        autocollimatorStatus_ = AutocollimatorStatus::InMotion;
    } else if (sMachine_.is(state<systemHome>)) {
        autocollimatorStatus_ = AutocollimatorStatus::Home;
    } else if (sMachine_.is(state<systemError>)) {
        autocollimatorStatus_ = AutocollimatorStatus::Error;
    } else {
        autocollimatorStatus_ = AutocollimatorStatus::NotInitialized;
    }
    return autocollimatorStatus_;
}

}  // namespace autocollimator
