/**
 * @file Monochromator/Events.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the structures that define the possible events of the Monochromator device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_MONOCHROMATOR_EVENTS_HPP_
#define MODULES_DEVICES_INCLUDE_MONOCHROMATOR_EVENTS_HPP_

namespace monochromator {

// Events
struct eventInitialize {};  /**< Event used to initialize the device. */
struct eventDisconnect {};  /**< Event used to disconnect the device. */
struct eventSetupHome {};  /**< Event used to home the device. */
struct eventStopMovement {};  /**< Event used to stop the motion of the device. */
struct eventStartScanStepper1Linear {};  /**< Event used to start a scan of the stepper motor that controls the linear motion of the device. */
struct eventStartScanStepper2Rotational {};  /**< Event used to start a scan of the stepper motor that controls the rotational motion of the device. */
struct eventMoveStepper1LinearToRelativePosition {};  /**< Event used to start a movement of the stepper motor that controls the linear motion of the device. */
struct eventMoveStepper2RotationalToRelativePosition {};  /**< Event used to start a movement of the stepper motor that controls the linear motion of the device. */
struct eventAlignSourceWithSensor {};  /**< Event used to move the device out of the beam path. */
struct eventAlignMonochromator {};  /**< Event used to align the device. */
struct eventSearchMonochromatorBraggPeak {};  /**< Event used search the Monochromator's crystal Bragg's peak. */

}  // namespace monochromator

#endif  // MODULES_DEVICES_INCLUDE_MONOCHROMATOR_EVENTS_HPP_
