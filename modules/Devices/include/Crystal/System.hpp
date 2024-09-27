/**
 * @file Crystal/System.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the Class that defines the state machine of the Crystal Device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <spdlog/spdlog.h>
#include <sml.hpp>

#include <string>
#include <memory>

#include "Crystal/Actions.hpp"
#include "Crystal/Events.hpp"

namespace crystal {

// States
class systemNotInitialized;
class systemConnected;
class systemHome;
class systemInMotion;
class stop;
class systemError;

/**
 * @class System
 * @brief Class that defines the state machine transitions and events of the Crystal Device.
 * 
 */
class System{
 private:
  std::shared_ptr<Actions> actions_;

 public:
  explicit System(std::shared_ptr<Actions> actions) noexcept:
  actions_(actions) {}
  ~System() {}
  auto operator()() const noexcept {
    using namespace boost::sml;  // NOLINT
    /**
     * Initial state: *initial_state
     * Transition DSL: src_state + event [ guard ] / action = dst_state
     */
    return make_transition_table(
    // From SystemNotInitialized
    *state<systemNotInitialized> + event<eventInitialize> [([this] { return actions_->connect(); })] = state<systemConnected>,
    state<systemNotInitialized> + event<eventInitialize> = state<systemError>,
    // From SystemConnected
    state<systemConnected> + event<eventSetupHome> [([this] { return true; })] = state<systemInMotion>,
    state<systemConnected> + event<eventDisconnect> [([this] { return actions_->disconnect(); })] = state<systemNotInitialized>,
    state<systemConnected> + event<eventDisconnect> [([this] { return !actions_->disconnect(); })] = state<systemError>,
    state<systemConnected> + event<eventMoveHxpToAbsolutePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemConnected> + event<eventMoveStepperToRelativePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemConnected> + event<eventStartScanStepper> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventStartScanHxp> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventXAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventZAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventYAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventYWAxesAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventSearchBraggPeakCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventYAxisFineAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventCheckAlignmentInFlippedOrientation> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventBendingAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventMiscutAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventTorsionAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    // From systemInMotion
    state<systemInMotion> + event<eventStartScanStepper> [([this] { return actions_->startScanStepper(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventStartScanStepper> [([this] { return !actions_->startScanStepper(); })] = state<systemError>,
    state<systemInMotion> + event<eventStartScanHxp> [([this] { return actions_->startScanHxp(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventStartScanHxp> [([this] { return !actions_->startScanHxp(); })] = state<systemError>,
    state<systemInMotion> + event<eventXAxisAlignmentCrystal> [([this] { return actions_->searchXAxisAlignmentCrystal(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventXAxisAlignmentCrystal> [([this] { return !actions_->searchXAxisAlignmentCrystal(); })] = state<systemError>,
    state<systemInMotion> + event<eventZAxisAlignmentCrystal> [([this] { return actions_->searchZAxisAlignmentCrystal(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventZAxisAlignmentCrystal> [([this] { return !actions_->searchZAxisAlignmentCrystal(); })] = state<systemError>,
    state<systemInMotion> + event<eventYAxisAlignmentCrystal> [([this] { return actions_->searchYAxisAlignmentCrystal(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventYAxisAlignmentCrystal> [([this] { return !actions_->searchYAxisAlignmentCrystal(); })] = state<systemError>,
    state<systemInMotion> + event<eventYWAxesAlignmentCrystal> [([this] { return actions_->searchYWAxesAlignmentCrystal(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventYWAxesAlignmentCrystal> [([this] { return !actions_->searchYWAxesAlignmentCrystal(); })] = state<systemError>,
    state<systemInMotion> + event<eventSearchBraggPeakCrystal> [([this] { return actions_->searchBraggPeakCrystal(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventSearchBraggPeakCrystal> [([this] { return !actions_->searchBraggPeakCrystal(); })] = state<systemError>,
    state<systemInMotion> + event<eventYAxisFineAlignmentCrystal> [([this] { return actions_->searchYAxisFineAlignment(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventYAxisFineAlignmentCrystal> [([this] { return !actions_->searchYAxisFineAlignment(); })] = state<systemError>,
    state<systemInMotion> + event<eventCheckAlignmentInFlippedOrientation> [([this] { return actions_->checkAlignmentFlippedOrientation(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventCheckAlignmentInFlippedOrientation> [([this] { return !actions_->checkAlignmentFlippedOrientation(); })] = state<systemError>,
    state<systemInMotion> + event<eventBendingAngleMeasurement> [([this] { return actions_->bendingAngleMeasurement(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventBendingAngleMeasurement> [([this] { return !actions_->bendingAngleMeasurement(); })] = state<systemError>,
    state<systemInMotion> + event<eventMiscutAngleMeasurement> [([this] { return actions_->miscutAngleMeasurement(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventMiscutAngleMeasurement> [([this] { return !actions_->miscutAngleMeasurement(); })] = state<systemError>,
    state<systemInMotion> + event<eventTorsionAngleMeasurement> [([this] { return actions_->torsionAngleMeasurement(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventTorsionAngleMeasurement> [([this] { return !actions_->torsionAngleMeasurement(); })] = state<systemError>,
    state<systemInMotion> + event<eventSetupHome> [([this] { return actions_->goHome(); })] = state<systemHome>,
    state<systemInMotion> + event<eventSetupHome> [([this] { return !actions_->goHome(); })] = state<systemError>,
    state<systemInMotion> + event<eventMoveHxpToAbsolutePosition> [([this] { return actions_->setHxpPositionAbsolute(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventMoveHxpToAbsolutePosition> [([this] { return !actions_->setHxpPositionAbsolute(); })] = state<systemError>,
    state<systemInMotion> + event<eventMoveStepperToRelativePosition> [([this] { return actions_->moveCalibratedMotor(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventMoveStepperToRelativePosition> [([this] { return !actions_->moveCalibratedMotor(); })] = state<systemError>,
    state<systemInMotion> + event<eventStopMovement> [([this] { return actions_->stop(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventStopMovement> [([this] { return !actions_->stop(); })] = state<systemError>,
    // From System Home
    state<systemHome> + event<eventSetupHome> [([this] { return true; })] = state<systemInMotion>,
    state<systemHome> + event<eventDisconnect> [([this] { return actions_->disconnect(); })] = state<systemNotInitialized>,
    state<systemHome> + event<eventDisconnect> [([this] { return !actions_->disconnect(); })] = state<systemError>,
    state<systemHome> + event<eventMoveHxpToAbsolutePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemHome> + event<eventMoveStepperToRelativePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemHome> + event<eventStartScanStepper> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventStartScanHxp> [([this] { return true; })] = state<systemInMotion>,  // Scan OK
    state<systemHome> + event<eventXAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventZAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemConnected> + event<eventYAxisAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventYWAxesAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventSearchBraggPeakCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventYAxisFineAlignmentCrystal> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventCheckAlignmentInFlippedOrientation> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventBendingAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventMiscutAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    state<systemHome> + event<eventTorsionAngleMeasurement> [([this] { return true; })]  = state<systemInMotion>,
    // From System Error
    state<systemError> + event<eventInitialize> [([this] { return actions_->connect(); })] = state<systemConnected>,
    state<systemError> + event<eventInitialize> [([this] { return !actions_->connect(); })] = state<systemError>,
    state<systemError> + event<eventDisconnect> / [this] { actions_->disconnect(); } = state<systemNotInitialized>);  // end transition table
  }
};

}  // namespace crystal
