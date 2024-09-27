/**
 * @file Crystal/Events.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains the structures that define the possible events of the Crystal device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_CRYSTAL_EVENTS_HPP_
#define MODULES_DEVICES_INCLUDE_CRYSTAL_EVENTS_HPP_

namespace crystal {

// Events
struct eventInitialize {};  /**< Event used to initialize the device. */
struct eventDisconnect {};  /**< Event used to disconnect the device. */
struct eventSetupHome {};  /**< Event used to home the device. */
struct eventStopMovement {};  /**< Event used to stop the device during a scan. */
struct eventMoveHxpToAbsolutePosition {};  /**< Event used to move the hexapod to a position. */
struct eventStartScanHxp {};  /**< Event used to start a scan of the hexapod motor. */
struct eventStartScanStepper {};  /**< Event used to start a scan of the stepper motor. */
struct eventMoveStepperToRelativePosition {};  /**< Event used to start a movement of the stepper motor. */
struct eventXAxisAlignmentCrystal {};  /**< Event used to start an alignment on the hexapod x-axis. */
struct eventZAxisAlignmentCrystal {};  /**< Event used to start an alignment on the hexapod z-axis. */
struct eventYAxisAlignmentCrystal {};  /**< Event used to start an alignment on the hexapod y-axis. */
struct eventYWAxesAlignmentCrystal {};  /**< Event used to start an alignment on the hexapod y-axis. */
struct eventSearchBraggPeakCrystal {};  /**< Event used to start a search of the Bragg's peak. */
struct eventYAxisFineAlignmentCrystal {};  /**< Event used to start a fine alignment on the hexapod y-axis. */
struct eventCheckAlignmentInFlippedOrientation {};  /**< Event used to check the hexapod alignment on the y-axis when the crystal is in flipped orientation. */
struct eventBendingAngleMeasurement {};  /**< Event used to start a bending angle measurement of the crystal. */
struct eventMiscutAngleMeasurement {};  /**< Event used to start a miscut angle measurement of the crystal. */
struct eventTorsionAngleMeasurement {};  /**< Event used to start a torsion angle measurement of the crystal. */

}  // namespace crystal

#endif  // MODULES_DEVICES_INCLUDE_CRYSTAL_EVENTS_HPP_
