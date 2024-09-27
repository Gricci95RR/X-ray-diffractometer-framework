/**
 * @file IXRaySensor.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of the interface Class @ref IXRaySensor.
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
#include <sstream>
#include <ctime>
#include <iomanip>

#include <asio.hpp>
#include "DppLibUsb.h"
#include "libusb.h"
#include "ConsoleHelper.h"

#include "IXRaySensor.hpp"
#include "Configuration.hpp"

/**
 * @class XRaySensor
 * @brief Implementation of the interface Class @ref IXRaySensor.
 * 
 */
class XRaySensor: public IXRaySensor {
 public:
    /**
     * @brief Construct a new XRaySensor object.
     */
    XRaySensor(std::shared_ptr<IConfiguration> clientConfiguration);
    /**
     * @brief Destroy the XRaySensor object.
     * 
     */
    ~XRaySensor();
    void disconnectSensor() override;
	 bool connectToSensor() override;
    void getSensorStatus() override;
    int acquireKalphaRadiation(int timeOfAcquisition) override;
    int acquireKbetaRadiation(int timeOfAcquisition) override;
    std::vector<int> acquireFullSpectrumOfRadiations(int timeOfAcquisition) override;
    /**
     * @brief Checks if the X-ray sensor is connected.
     *
     * This method returns the connection status of the X-ray sensor.
     *
     * @return True if the sensor is connected, false otherwise.
     */
    bool isConnected();
    /**
     * @brief Initiates spectrum data acquisition.
     * 
     * This method configures the hardware, sends commands for spectrum data acquisition,
     * and disables the MCA (Multi-Channel Analyzer) after data acquisition.
     * 
     * @param timeOfAcquisition Time of acquisition (seconds).
     */
    void acquireSpectrum(int timeOfAcquisition);
    /**
     * @brief Saves the spectrum data to a file and returns it as a string.
     *
     * This method saves the acquired spectrum data to a file with a timestamped filename.
     * It also logs relevant information and constructs the necessary file paths.
     *
     * @param timeOfAcquisition The time duration of the acquisition in seconds.
     * @param saveMCAfile A boolean indicating whether to save the spectrum data to a file.
     * @return A string containing the spectrum data.
     */
    std::string saveSpectrumFile(int timeOfAcquisition, bool saveMCAfile);
    /**
     * @brief Sends the preset acquisition time to the hardware.
     *
     * This method sets the preset acquisition time by sending the specified configuration string to the hardware.
     * It bypasses any filters, reads back the configuration to verify the setting, and logs the process.
     *
     * @param strPRET The string representing the preset acquisition time to be set.
     */
    void sendPresetAcquisitionTime(string strPRET);
    /**
     * @brief Saves the spectrum configuration.
     *
     * This method generates the spectrum configuration commands and appends configuration comments.
     * The resulting configuration string is stored in the `sfInfo` structure of the `chdpp_` object.
     */
    void saveSpectrumConfig();
    /**
     * @brief Reads the DPP configuration from the hardware.
     *
     * This method requests the full configuration from the hardware and, if successful,
     * updates the internal configuration status. The configuration can be optionally displayed.
     *
     * @param bDisplayCfg A boolean indicating whether to display the configuration.
     */
    void readDppConfigurationFromHardware(bool bDisplayCfg);
    /**
     * @brief Retrieves the count at a specified radiation index from the input spectrum data.
     * 
     * This method extracts the spectrum data from the input string, parses the numbers, 
     * and returns the count at the specified index (centroidIndex).
     * 
     * @param input The input string containing the spectrum data delimited by <<DATA>> and <<END>>.
     * @param centroidIndex The index in the parsed data at which the desired count is located.
     * @return int The count at the specified radiation index. Returns -1 if the index is out of range 
     *         or if the boundaries are not found in the input string.
     */
    int getCountAtDesiredRadiation(const std::string& input, int centroid);
   /**
     * @brief Retrieves the counts at desired radiation levels from the input string.
     *
     * This method searches for data within specific delimiters in the input string, parses the data,
     * and then calculates the maximum counts above specified indices for Kalpha and Kbeta radiation.
     *
     * @param input The input string containing the spectrum data.
     * @return A pair of integers representing the counts for Kalpha and Kbeta radiation.
     */
    std::pair<int, int> getCountAtDesiredRadiation(const std::string& input);
    /**
     * @brief Acquires the full spectrum of radiation counts.
     * 
     * This method initiates spectrum data acquisition and retrieves the full spectrum
     * of radiation counts from the saved spectrum file.
     * 
     * @return std::vector<int> A vector containing the full spectrum of radiation counts.
     */
    std::vector<int> getCountsFullSpectrumOfRadiation(const std::string& input);
    /**
     * @brief Finds the maximum value in a vector starting from a given index.
     *
     * This method searches for the maximum value in the vector `numbers` starting from the specified `startIndex`.
     * If the vector does not contain more elements than `startIndex`, an exception is thrown.
     *
     * @param numbers The vector of integers to search.
     * @param startIndex The index from which to start the search.
     * @return The maximum value found in the vector from `startIndex` onwards.
     * @throws std::out_of_range if the vector does not contain more than `startIndex` elements.
     */
    int findMaxAboveIndex(const std::vector<int>& numbers, int startIndex);

    double computeIntegral(const std::vector<int>& numbers, size_t start, size_t stop);

 private:
    std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
    CDppLibUsb DppLibUsb_;  /**< LibUsb communications object. */
    bool LibUsb_isConnected_;  /**< LibUsb is connected if true. */
    int  LibUsb_NumDevices_;  /**< LibUsb number of devices found. */
    CConsoleHelper chdpp_;  /**< Object of class CConsoleHelper (extern library) containing communications functions */
    bool bRunSpectrumTest_ = false;   /**< run spectrum test */
    bool bRunConfigurationTest_ = false;  /**< run configuration test */
    bool bHaveStatusResponse_ = false;  /**< have status response */
    bool bHaveConfigFromHW_ = false;  /**< have configuration from hardware */
};
