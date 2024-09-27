/**
 * @file AutocollimatorStatus.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file contains an enumeration class that defines the possible states of an Autocollimator device.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_AUTOCOLLIMATOR_AUTOCOLLIMATORSTATUS_HPP_
#define MODULES_DEVICES_INCLUDE_AUTOCOLLIMATOR_AUTOCOLLIMATORSTATUS_HPP_

namespace autocollimator {

/**
 * @enum AutocollimatorStatus
 * @brief Enumeration class that defines the possible states of an Autocollimator device.
 * 
 * The class provides a set of named constants with associated integer values starting from 0.
 * 
 */
enum class AutocollimatorStatus {
    NotDefined = 0,  /**< The status is not defined or unknown.*/

    NotInitialized = 1,  /**< The device has not been initialized or connected yet.*/

    Connected = 2,  /**<  The device is connected and ready for use.*/

    Home = 3,  /**<  The device is currently in the home position.*/

    InMotion = 4,  /**< The device is currently in motion.*/

    Error = 5  /**< An error has occurred with the device.*/
};

}  // namespace autocollimator

#endif  // MODULES_DEVICES_INCLUDE_AUTOCOLLIMATOR_AUTOCOLLIMATORSTATUS_HPP_
