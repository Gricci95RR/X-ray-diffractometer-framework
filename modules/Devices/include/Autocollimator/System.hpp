/**
 * @file Autocollimator/System.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the Class that defines the state machine of the Autocollimator Device.
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

#include "Autocollimator/Actions.hpp"
#include "Autocollimator/Events.hpp"

namespace autocollimator {

// States
class systemNotInitialized;
class systemConnected;
class systemHome;
class stop;
class systemInMotion;
class systemError;

/**
 * @class System
 * @brief Class that defines the state machine transitions and events of the Autocollimator Device.
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
    state<systemConnected> + event<eventMoveStepperToRelativePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemConnected> + event<eventStartScanStepper> [([this] { return true; })]  = state<systemInMotion>,
    // From systemInMotion
    state<systemInMotion> + event<eventStartScanStepper> [([this] { return actions_->startScanStepper(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventStartScanStepper> [([this] { return !actions_->startScanStepper(); })] = state<systemError>,
    state<systemInMotion> + event<eventSetupHome> [([this] { return actions_->goHome(); })] = state<systemHome>,
    state<systemInMotion> + event<eventSetupHome> [([this] { return !actions_->goHome(); })] = state<systemError>,
    state<systemInMotion> + event<eventMoveStepperToRelativePosition> [([this] { return actions_->moveCalibratedMotor(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventMoveStepperToRelativePosition> [([this] { return !actions_->moveCalibratedMotor(); })] = state<systemError>,
    state<systemInMotion> + event<eventStopMovement> [([this] { return actions_->stop(); })] = state<systemConnected>,
    state<systemInMotion> + event<eventStopMovement> [([this] { return !actions_->stop(); })] = state<systemError>,
    // From System Home
    state<systemHome> + event<eventDisconnect> / [this] { actions_->disconnect(); } = state<systemNotInitialized>,
    state<systemHome> + event<eventMoveStepperToRelativePosition> [([this] { return true; })] = state<systemInMotion>,
    state<systemHome> + event<eventStartScanStepper> [([this] { return true; })]  = state<systemInMotion>,
    // From System Error
    state<systemError> + event<eventInitialize> [([this] { return actions_->connect(); })] = state<systemConnected>,
    state<systemError> + event<eventInitialize> [([this] { return !actions_->connect(); })] = state<systemError>,
    state<systemError> + event<eventDisconnect> / [this] { actions_->disconnect(); } = state<systemNotInitialized>);  // end transition table
}
};

}  // namespace autocollimator
