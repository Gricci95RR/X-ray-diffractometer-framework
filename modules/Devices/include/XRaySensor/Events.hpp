/**
 * @file XRaySensor/Events.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the structures that define the possible events of the XRaySensor device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */


#ifndef MODULES_DEVICES_INCLUDE_XRAYSENSOR_EVENTS_HPP_
#define MODULES_DEVICES_INCLUDE_XRAYSENSOR_EVENTS_HPP_

namespace xRaySensor {

// Events
struct eventInitialize {};  /**< Event used to initialize the device. */
struct eventDisconnect {};  /**< Event used to disconnect the device. */
struct eventSetupHome {};  /**< Event used to home the device. */
struct eventStopMovement {};  /**< Event used to stop the device during a scan. */
struct eventStartScanStepper {};  /**< Event used to start a scan of the stepper motor. */
struct eventMoveStepperToRelativePosition {};  /**< Event used to start a movement of the stepper motor. */
}  // namespace xRaySensor

#endif  // MODULES_DEVICES_INCLUDE_XRAYSENSOR_EVENTS_HPP_
