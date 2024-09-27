/**
 * @file Configuration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'IConfiguration' used to write/read the configuration files.
 * @version 0.1
 * @date 2022-03-01
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <ini.h>
#include "spdlog/spdlog.h"

#include <iostream>
#include <filesystem>
#include <string>

#include "IConfiguration.hpp"

/**
 * @class Configuration
 * @brief Implementation Class of 'IConfiguration'.
 * 
 */
class Configuration : public IConfiguration{
 private:
    std::filesystem::path path_;
    std::filesystem::path pathToLogFilesDirectory_;  /**< Path to the where the log files are stored. */
    /**
     * @brief Struct containing all the default devices parameters.
     * 
     */
    struct defaultConfigurationParameters_{
        const std::string iPAddressHxp = "192.168.0.3";
        const int nPortHxp = 5001;
        const std::string iPAddressStepper = "192.168.0.2";
        const std::string stepperMotorIndexAxis0 = "0000304C";
        const std::string stepperMotorIndexAxis1 = "0000304F";
        const std::string stepperMotorIndexAxis2 = "00003060";
        const std::string stepperMotorIndexAxis3 = "00003070";
        const std::string stepperMotorIndexAxis4 = "0000306D";
        const std::string stepperMotorIndexAxis5 = "00003051";
        const std::string stepperMotorIndexAxis6 = "0000306E";
        const std::string stepperMotorIndexAxis7 = "00003079";
        const double const0 = 0.005;
        const double const1 = 0.010000;
        const double const2 = 0.005000;
        const double const3 = 0.010000;
        const double const4 = 0.004500;
        const double const5 = 0.004500;
        const double const6 = 0.003750;
        const double const7 = 0.005;
    };
    const std::filesystem::path projectName_ = "XRay_Machine";  /**< Name of the project. */
    const std::filesystem::path configurationFilesFolder_ = "ConfigurationFiles";
    const std::filesystem::path logFilesFolder_ = "LogFiles";
    const std::filesystem::path configFilename_ = "config.ini";  /**< Name of the configuration file that stores the 'devices configurations' and 'alignment configurations'. */
    const std::filesystem::path alignmentSettingsFilename_ = "AlignmentSettings.ini"; /**< Name of the configuration file that stores the alignment settings. */
    defaultConfigurationParameters_ defaultConfiguration_;

 public:

    explicit Configuration();

    ~Configuration();

    std::filesystem::path getPartialPathUntilKeyName
                        (const std::filesystem::path pathReference,
                         const std::filesystem::path keyPathName) override;
    int writeConfigurationFile(std::string value,
                               std::string section,
                               std::string key,
                               std::filesystem::path filename,
                               std::filesystem::path path) override;
    int createConfigurationFile(std::filesystem::path filename,
                                std::filesystem::path path) override;
    int hasFile(std::filesystem::path filename, std::filesystem::path path) override;
    int hasSection(std::string section,
                   std::filesystem::path filename,
                   std::filesystem::path path) override;
    int hasKey(std::string section,
               std::string key,
               std::filesystem::path filename,
               std::filesystem::path path) override;
    int removeConfigurationFile(std::filesystem::path filename,
                                std::filesystem::path path) override;
    float readFloatFromConfigurationFile(std::filesystem::path filename,
                                         std::filesystem::path path,
                                         std::string section,
                                         std::string key) override;
    int readIntFromConfigurationFile(std::filesystem::path filename,
                                     std::filesystem::path path,
                                     std::string section,
                                     std::string key) override;
    std::string readStringFromConfigurationFile(std::filesystem::path filename,
                                                std::filesystem::path path,
                                                std::string section,
                                                std::string key) override;
    bool readBoolFromConfigurationFile(std::filesystem::path filename,
                                       std::filesystem::path path,
                                       std::string section,
                                       std::string key) override;
    std::filesystem::path getPath() override;
    void setPath(std::filesystem::path path) override;
    std::filesystem::path getConfigFilename() override;
    std::filesystem::path getAlignmentSettingsConfigFilename() override;
    std::filesystem::path getPathToLogFilesDirectory() override;
    std::string readStepperMotorIndexAxis(int axis) override;
    double readStepperMotorCONST(int axis) override;
    std::filesystem::path readFileSystemPathFromConfigurationFile(std::filesystem::path filename,
                                                                  std::filesystem::path path,
                                                                  std::string section,
                                                                  std::string key) override;
   std::string getDefaultIpAddressHxp() override;
   std::string getDefaultIpAddressStepper() override;
   int getDefaultNPortHxp() override;
};
