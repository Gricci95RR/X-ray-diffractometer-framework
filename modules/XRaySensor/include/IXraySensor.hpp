/**
 * @file IXRaySensor.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used for XRaySensor communication.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <string>

#include "IConfiguration.hpp"

/**
 * @class IXRaySensor
 * @brief Interface used for XRaySensor communication.
 * 
 */
class IXRaySensor {
 public:
    ~IXRaySensor() = default;
    /**
     * @brief Disconnect the device.
     */
    virtual void disconnectSensor() = 0;
    /**
     * @brief Attempts to connect to the X-ray sensor using the default LibUsb device.
     * 
     * @return bool True if the connection is successful, false otherwise.
     */
    virtual bool connectToSensor() = 0;
    /**
     * @brief Retrieves the status of the X-ray sensor by sending a status request command.
     * 
     * This method checks if the sensor is connected, sends a status request command, 
     * and processes the received status data. It updates internal flags based on the status response.
     */
    virtual void getSensorStatus() = 0;
    /**
     * @brief Acquires K-alpha radiation data.
     * 
     * This method initiates spectrum data acquisition, retrieves K-alpha radiation count,
     * and returns the count.
     * 
     * @param timeOfAcquisition Time of acquisition (seconds).
     * 
     * @return int The count at K-alpha radiation.
     */
    virtual int acquireKalphaRadiation(int timeOfAcquisition) = 0;
    /**
     * @brief Acquires K-beta radiation data.
     * 
     * This method initiates spectrum data acquisition, retrieves K-beta radiation count,
     * and returns the count.
     * 
     * @param timeOfAcquisition Time of acquisition (seconds).
     * 
     * @return int The count at K-beta radiation.
     */
    virtual int acquireKbetaRadiation(int timeOfAcquisition) = 0;
    /**
     * @brief Acquires the full spectrum of radiation counts by collecting spectrum data sets.
     * 
     * This method configures the hardware, initiates spectrum data acquisition, 
     * and retrieves the full spectrum of radiation counts.
     * 
     * @param timeOfAcquisition Time of acquisition (seconds).
     * 
     * @return std::vector<int> A vector of integers representing the full spectrum of radiation counts.
     */
    virtual std::vector<int> acquireFullSpectrumOfRadiations(int timeOfAcquisition) = 0;

    virtual double computeIntegral(const std::vector<int>& numbers, size_t start, size_t stop) = 0;
};
