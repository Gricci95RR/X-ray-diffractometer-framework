/**
 * @file Slit/Events.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the structures that define the possible events of the Slit device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_SLIT_EVENTS_HPP_
#define MODULES_DEVICES_INCLUDE_SLIT_EVENTS_HPP_

namespace slit {

// Events
struct eventInitialize {};  /**< Event used to initialize the device. */
struct eventDisconnect {};  /**< Event used to disconnect the device. */
struct eventSetupHome {};  /**< Event used to home the device. */
struct eventStopMovement {};  /**< Event used to stop the motion of the device. */
struct eventStartScanStepper1Linear {};  /**< Event used to start a scan of the stepper motor that controls the linear motion of the device. */
struct eventStartScanStepper2Rotational {};  /**< Event used to start a scan of the stepper motor that controls the rotational motion of the device. */
struct eventMoveStepper1LinearToRelativePosition {};  /**< Event used to start a movement of the stepper motor that controls the linear motion of the device. */
struct eventMoveStepper2RotationalToRelativePosition {};  /**< Event used to start a movement of the stepper motor that controls the linear motion of the device. */
struct eventMoveBothMotors {};  /**< Event used to move both stepper motors of the device. */
struct eventSearchMonochromatorBraggsPeak {};
struct eventAlignSlit {};  /**< Event used to align the device. */

}  // namespace slit

#endif  // MODULES_DEVICES_INCLUDE_SLIT_EVENTS_HPP_
