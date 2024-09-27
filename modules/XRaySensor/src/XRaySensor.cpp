/**
 * @file IXRaySensor.cpp
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

#include "spdlog/spdlog.h"
#include "asio.hpp"

#include <fcntl.h>
#include <iostream>
#include <string>
#include <fstream>

#include "XRaySensor.hpp"

XRaySensor::XRaySensor(std::shared_ptr<IConfiguration> clientConfiguration) :
	clientConfiguration_(clientConfiguration) {
        spdlog::debug("CTor of Class XRaySensor\n");
}

XRaySensor::~XRaySensor() {
	spdlog::debug("DTor of Class XRaySensor\n");
	if (this->isConnected())
		this->disconnectSensor();
}

void XRaySensor::disconnectSensor() {
	spdlog::info("Method disconnectSensor of Class XRaySensor\n");
    if (chdpp_.LibUsb_isConnected) { // send and receive status
		spdlog::debug("Closing connection to default LibUsb device...");
		chdpp_.LibUsb_Close_Connection();
		spdlog::debug("DPP device connection closed.");
	}
}

bool XRaySensor::connectToSensor() {
	spdlog::info("Method connectToSensor of Class XRaySensor\n");
	spdlog::debug("Running DPP LibUsb tests from console...");
	spdlog::debug("Connecting to default LibUsb device...\n");
	if (chdpp_.LibUsb_Connect_Default_DPP()) {
		spdlog::info("LibUsb DPP device connected.\n");
		spdlog::debug("XRaySensor device connected!\n");
		LibUsb_isConnected_ = true;
		return LibUsb_isConnected_;
	} else {
		spdlog::error("LibUsb DPP device not connected.\n");
		spdlog::error("No LibUsb DPP device present.\n");
		LibUsb_isConnected_ = false;
		return LibUsb_isConnected_;
	}
}

bool XRaySensor::isConnected() {
    return LibUsb_isConnected_;
}

void XRaySensor::getSensorStatus() {
	spdlog::info("Method getSensorStatus of Class XRaySensor\n");
	if (chdpp_.LibUsb_isConnected) { // send and receive status
		cout << endl;
		spdlog::debug("Requesting Status...");
		if (chdpp_.LibUsb_SendCommand(XMTPT_SEND_STATUS)) {	// request status
			spdlog::debug("Status sent.");
			if (chdpp_.LibUsb_ReceiveData()) {
				//cout << chdpp_.DppStatusString << endl;
				bRunSpectrumTest_ = true;
				bHaveStatusResponse_ = true;
				bRunConfigurationTest_ = true;
			} else {
				spdlog::error("Error receiving status.");
			}
		} else {
			spdlog::error("Error sending status.");
		}
	}
}

void XRaySensor::readDppConfigurationFromHardware(bool bDisplayCfg) {
	CONFIG_OPTIONS CfgOptions;
	if (bHaveStatusResponse_ && bRunConfigurationTest_) {
		//test configuration functions
		// Set options for XMTPT_FULL_READ_CONFIG_PACKET
		chdpp_.CreateConfigOptions(&CfgOptions, "", chdpp_.DP5Stat, false);
		cout << endl;
		spdlog::debug("Requesting Full Configuration...\n");
		chdpp_.ClearConfigReadFormatFlags();	// clear all flags, set flags only for specific readback properties
		//chdpp_.DisplayCfg = false;	// DisplayCfg format overrides general readback format
		chdpp_.CfgReadBack = true;	// requesting general readback format
		if (chdpp_.LibUsb_SendCommand_Config(XMTPT_FULL_READ_CONFIG_PACKET, CfgOptions)) {	// request full configuration
			if (chdpp_.LibUsb_ReceiveData()) {
				if (chdpp_.HwCfgReady) {		// config is ready
					bHaveConfigFromHW_ = true;
					if (bDisplayCfg) {
						cout << chdpp_.HwCfgDP5 << endl;
					} else {
						spdlog::debug("Full configuration received.\n");
					}
				}
			}
		}
	}
}

int XRaySensor::acquireKalphaRadiation(int timeOfAcquisition) {
	spdlog::info("Method acquireKalphaRadiation of Class XRaySensor\n");
	this->acquireSpectrum(timeOfAcquisition);
	return this->getCountAtDesiredRadiation(this->saveSpectrumFile(timeOfAcquisition, false)).first;
}

int XRaySensor::acquireKbetaRadiation(int timeOfAcquisition) {
	spdlog::info("Method acquireKbetaRadiation of Class XRaySensor\n");
	this->acquireSpectrum(timeOfAcquisition);
	return this->getCountAtDesiredRadiation(this->saveSpectrumFile(timeOfAcquisition, false)).second;
}

std::vector<int> XRaySensor::acquireFullSpectrumOfRadiations(int timeOfAcquisition) {
	spdlog::info("Method acquireFullSpectrumOfRadiations of Class XRaySensor\n");
	this->acquireSpectrum(timeOfAcquisition);
	return this->getCountsFullSpectrumOfRadiation(this->saveSpectrumFile(timeOfAcquisition, false));
}

void XRaySensor::acquireSpectrum(int timeOfAcquisition) {
	spdlog::info("Method acquireSpectrum of Class XRaySensor\n");
	spdlog::info("Method acquiring spectrum ...");
	bool bDisableMCA;
	this->readDppConfigurationFromHardware(false);
	this->saveSpectrumConfig();
	if (bRunSpectrumTest_) {
		chdpp_.LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS);
		chdpp_.LibUsb_SendCommand(XMTPT_SEND_CLEAR_SPECTRUM_STATUS);
		chdpp_.LibUsb_SendCommand(XMTPT_ENABLE_MCA_MCS);

		// Convert seconds to milliseconds
        ///int acquisitionTimeMs = (timeOfAcquisition > 2 ? timeOfAcquisition + 1 : timeOfAcquisition) * 1000;
		int acquisitionTimeMs = (timeOfAcquisition) * 1000;

		for (int idxSpectrum=0; idxSpectrum<=1; ++idxSpectrum) { // acquire one dataset
			if (chdpp_.LibUsb_SendCommand(XMTPT_SEND_SPECTRUM_STATUS)) { // request spectrum+status
				if (chdpp_.LibUsb_ReceiveData()) {
					bDisableMCA = true;  // aquiring data, disable mca when done
					if (idxSpectrum == 0)
						std::this_thread::sleep_for(std::chrono::milliseconds(acquisitionTimeMs));
					else
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
			} else {
				spdlog::error("Problem acquiring spectrum.\n");
				break;
			}
		}
		if (bDisableMCA) {
			chdpp_.LibUsb_SendCommand(XMTPT_DISABLE_MCA_MCS);
		}
	}
}

void XRaySensor::sendPresetAcquisitionTime(string strPRET) {
	CONFIG_OPTIONS CfgOptions;
	spdlog::debug("Setting Preset Acquisition Time...\n");
	chdpp_.CreateConfigOptions(&CfgOptions, "", chdpp_.DP5Stat, false);
	CfgOptions.HwCfgDP5Out = strPRET;
	// send PresetAcquisitionTime string, bypass any filters, read back the mode and settings
	if (chdpp_.LibUsb_SendCommand_Config(XMTPT_SEND_CONFIG_PACKET_EX, CfgOptions)) {
		this->readDppConfigurationFromHardware(false);	// read setting back
		//DisplayPresets();							// display new presets
	} else {
		spdlog::error("Preset Acquisition Time NOT SET\n");
	}
}

int XRaySensor::getCountAtDesiredRadiation(const std::string& input, int centroidIndex) {
	spdlog::info("Method getCountAtDesiredRadiation of Class XRaySensor\n");
    // Find the positions of <<DATA>> and <<END>>
    size_t dataPos = input.find("<<DATA>>");
    size_t endPos = input.find("<<END>>");
	int count;

    if (dataPos != std::string::npos && endPos != std::string::npos) {
        // Extract the substring between <<DATA>> and <<END>>
        std::string dataSubstring = input.substr(dataPos + 8, endPos - dataPos - 8);

        // Parse the numbers within the substring
		std::istringstream iss(dataSubstring);
        std::vector<int> numbers;
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }

        // Find the maximum number
        if (numbers.size() > centroidIndex) {
            count = numbers[centroidIndex];
            return count;
        } else {
            spdlog::error("Not enough numbers in the spectrum; index 28 is out of range.\n");
        }
    } else {
		spdlog::error("Boundaries not found in the input string.\n");
    }
    return -1; // Indicate failure by returning a negative number
}

std::pair<int, int> XRaySensor::getCountAtDesiredRadiation(const std::string& input) {
	spdlog::info("Method getCountAtDesiredRadiation of Class XRaySensor\n");
    // Find the positions of <<DATA>> and <<END>>
    size_t dataPos = input.find("<<DATA>>");
    size_t endPos = input.find("<<END>>");
	std::pair<int, int> countsKA_KB;

    if (dataPos != std::string::npos && endPos != std::string::npos) {
        // Extract the substring between <<DATA>> and <<END>>
        std::string dataSubstring = input.substr(dataPos + 8, endPos - dataPos - 8);

        // Parse the numbers within the substring
		std::istringstream iss(dataSubstring);
        std::vector<int> numbers;
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
		/*
        countsKA_KB.first = this->findMaxAboveIndex(numbers, clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getConfigFilename(),
																												clientConfiguration_->getPath(),
																												"X_RAY_SENSOR_SETTINGS",
																												"K_ALPHA_START")
																												);  // Kalpha*/
		countsKA_KB.first = this->computeIntegral(numbers, clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getConfigFilename(),
																												clientConfiguration_->getPath(),
																												"X_RAY_SENSOR_SETTINGS",
																												"K_ALPHA_START"),
															clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getConfigFilename(),
																													clientConfiguration_->getPath(),
																													"X_RAY_SENSOR_SETTINGS",
																													"K_ALPHA_STOP")
																													);																										
		countsKA_KB.second = this->findMaxAboveIndex(numbers, clientConfiguration_->readIntFromConfigurationFile(clientConfiguration_->getConfigFilename(),
																												 clientConfiguration_->getPath(),
																												 "X_RAY_SENSOR_SETTINGS",
																												 "K_BETA_START")
																												 );  // Kbeta
		return countsKA_KB;
    } else {
		spdlog::error("Boundaries not found in the input string.\n");
    }
	return countsKA_KB;
}

int XRaySensor::findMaxAboveIndex(const std::vector<int>& numbers, int startIndex) {
    if (numbers.size() <= startIndex) {
        throw std::out_of_range("The vector does not have more than startIndex elements.");
    }

    // Use std::max_element to find the maximum element starting from index "startIndex"
    auto max_iter = std::max_element(numbers.begin() + startIndex, numbers.end());

	//  std::cout << std::distance(numbers.begin(), max_iter) << std::endl;
    
    return *max_iter;
}

double XRaySensor::computeIntegral(const std::vector<int>& numbers, size_t start, size_t stop) {
    if (start >= stop || stop >= numbers.size()) {
        throw std::out_of_range("Invalid start or stop index");
    }
    double integral = 0.0;
    for (size_t i = start; i < stop; ++i) {
        // Apply the trapezoidal rule
        integral += (numbers[i] + numbers[i + 1]) / 2.0;
    }
    return integral;
}

std::vector<int> XRaySensor::getCountsFullSpectrumOfRadiation(const std::string& input) {
	spdlog::info("Method getCountsFullSpectrumOfRadiation of Class XRaySensor\n");
    // Find the positions of <<DATA>> and <<END>>
    size_t dataPos = input.find("<<DATA>>");
    size_t endPos = input.find("<<END>>");
	std::vector<int> numbers;

    if (dataPos != std::string::npos && endPos != std::string::npos) {
        // Extract the substring between <<DATA>> and <<END>>
        std::string dataSubstring = input.substr(dataPos + 8, endPos - dataPos - 8);

        // Parse the numbers within the substring
		std::istringstream iss(dataSubstring);
        int num;
        while (iss >> num) {
            numbers.push_back(num);
        }
    } else {
		spdlog::error("Boundaries not found in the input string.\n");
    }
    return numbers; // Indicate failure by returning a negative number
}

std::string XRaySensor::saveSpectrumFile(int timeOfAcquisition, bool saveMCAfile) {
	spdlog::info("Method saveSpectrumFile of Class XRaySensor\n");
	string strSpectrum;											// holds final spectrum file
	chdpp_.sfInfo.strSpectrumStatus = chdpp_.DppStatusString;		// save last status after acquisition
	chdpp_.sfInfo.m_iNumChan = chdpp_.mcaCH;						// number channels in spectrum
	chdpp_.sfInfo.SerialNumber = chdpp_.DP5Stat.m_DP5_Status.SerialNumber;	// dpp serial number
	chdpp_.sfInfo.strDescription = "Amptek Spectrum File";					// description
    chdpp_.sfInfo.strTag = "TestTag";										// tag
	// create spectrum file, save file to string
    strSpectrum = chdpp_.CreateMCAData(chdpp_.DP5Proto.SPECTRUM.DATA,chdpp_.sfInfo,chdpp_.DP5Stat.m_DP5_Status);
	// Create a string stream to format the date and time
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    std::stringstream dateTimeStream;
    dateTimeStream << std::put_time(localTime, "%Y-%m-%d_%H-%M-%S");
	// Create path to save spectrum
	std::filesystem::path currentPath = std::filesystem::current_path();
    std::string projectName = "XRay_Machine";
    std::filesystem::path pathToProjDirectory = clientConfiguration_->getPartialPathUntilKeyName(currentPath, projectName);
    // Additional path parts
    std::filesystem::path logFilesDirectry = "LogFiles";
	std::filesystem::path spectraFilesDirectry = "Spectra";
    // Combine the paths
    std::filesystem::path directoryPath = pathToProjDirectory / logFilesDirectry / spectraFilesDirectry;
    // Create the filename with the current date and time
    std::filesystem::path fileName = "SpectrumData_TOA_" + std::to_string(timeOfAcquisition) + "_" + dateTimeStream.str() + ".mca";
    // Combine the directory path and filename
    std::filesystem::path fullPath = directoryPath / fileName;
	if (saveMCAfile) {
		chdpp_.SaveSpectrumStringToFile(strSpectrum, fullPath.string());  // save spectrum file string to file
	}
	return strSpectrum;
}

void XRaySensor::saveSpectrumConfig() {
	string strSpectrumConfig;
	chdpp_.Dp5CmdList = chdpp_.MakeDp5CmdList();	// ascii text command list for adding comments
	strSpectrumConfig = chdpp_.CreateSpectrumConfig(chdpp_.HwCfgDP5);	// append configuration comments
	chdpp_.sfInfo.strSpectrumConfig = strSpectrumConfig;
}