/**
 * @file IConfiguration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Inteface Class used to write/read the configuration files.
 * @version 0.1
 * @date 2022-03-01
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <filesystem>
#include <string>

/**
 * @class IConfiguration.
 * @brief Inteface Class used to write/read the configuration files.
 * 
 */
class IConfiguration{
 private:
    
 public:
    /**
     * @brief Destroy the IConfiguration object.
     * 
     */
    ~IConfiguration() = default;

    /**
     * @brief Used to get the path to project directory.
     * @param pathReference current path.
     * @param keyPathName name of the project directory.
     * @return std::filesystem::path containing the partial path to project directory.
     */
    virtual std::filesystem::path getPartialPathUntilKeyName
                        (const std::filesystem::path pathReference,
                         const std::filesystem::path keyPathName) = 0;

    /**
     * @brief Write a string value in configuration file.
     * @param value new value to be written in the configuration file.
     * @param section name of one of the section of the configuration file.
     * @param key name of one of the key of configuration file.
     * @param filename name of the file.
     * @param path path to the directory that contains the configuration file.
     * @return 1 if writing was successful 0 otherwise.
     */
    virtual int writeConfigurationFile(std::string value,
                               std::string section,
                               std::string key,
                               std::filesystem::path filename,
                               std::filesystem::path path) = 0;

    /**
     * @brief Create an empty configuration file.
     * @param filename name of the file to be created (filename with .ini extension).
     * @param path where the new configuration file is created.
     * @return 1 if writing was successful 0 otherwise.
     */
    virtual int createConfigurationFile(std::filesystem::path filename,
                                std::filesystem::path path) = 0;

    /**
     * @brief check if there is a configuration file.
     * @param filename name of the file to be searched.
     * @param path path of the file.
     * @return 1 if the file was found 0 otherwise.
     */
    virtual int hasFile(std::filesystem::path filename, std::filesystem::path path) = 0;

    /**
     * @brief check if the configuration file has a section.
     * @param section name of one of the section of the configuration file.
     * @param filename name of the file.
     * @param path path of the file to be tested.
     * @return 1 if the section was found 0 otherwise.
     */
    virtual int hasSection(std::string section,
                   std::filesystem::path filename,
                   std::filesystem::path path) = 0;

    /**
     * @brief check if the configuration file has a key.
     * @param section name of one of the section of the configuration file.
     * @param filename name of the file.
     * @param key name of one of the key of the configuration file.
     * @param path path of the file to be tested.
     * @return 1 if the key was found 0 otherwise.
     */
    virtual int hasKey(std::string section,
               std::string key,
               std::filesystem::path filename,
               std::filesystem::path path) = 0;

    /**
     * @brief remove configuration file.
     * @param filename name of the file to be removed.
     * @param path path to the directory of the file to be removed.
     * @return 1 if the file was removed 0 otherwise.
     */
    virtual int removeConfigurationFile(std::filesystem::path filename,
                                std::filesystem::path path) = 0;

    /**
     * @brief Read a float number from configuration file.
     * @param filename name of the file to be read.
     * @param path path to the directory of the file to be read.
     * @return float number read.
     */
    virtual float readFloatFromConfigurationFile(std::filesystem::path filename,
                                         std::filesystem::path path,
                                         std::string section,
                                         std::string key) = 0;
    /**
     * @brief Read a int number from configuration file.
     * @param filename name of the file to be read.
     * @param path path to the directory of the file to be read.
     * @return int number read.
     */
    virtual int readIntFromConfigurationFile(std::filesystem::path filename,
                                     std::filesystem::path path,
                                     std::string section,
                                     std::string key) = 0;
    /**
     * @brief Read a std::string number from configuration file.
     * @param filename name of the file to be read.
     * @param path path to the directory of the file to be read.
     * @return std::string number read.
     */
    virtual std::string readStringFromConfigurationFile(std::filesystem::path filename,
                                                std::filesystem::path path,
                                                std::string section,
                                                std::string key) = 0;
    /**
     * @brief Read a int number from configuration file and convert it in bool.
     * @param filename name of the file to be read.
     * @param path path to the directory of the file to be read.
     * @return bool converted.
     */
    virtual bool readBoolFromConfigurationFile(std::filesystem::path filename,
                                                std::filesystem::path path,
                                                std::string section,
                                                std::string key) = 0;

    /**
     * @brief get path to the .ini configuration file directory.
     * @return std::filesystem::path path to the configuration file directory.
     */
    virtual std::filesystem::path getPath() = 0;

    /**
     * @brief set path to the .ini configuration file directory.
     * @param path std::filesystem::path to the .ini configuration file directory.
     */
    virtual void setPath(std::filesystem::path path) = 0;

    /**
     * @brief get name of the .ini configuration file.
     * @return std::filesystem::path name of the configuration file.
     */
    virtual std::filesystem::path getConfigFilename() = 0;

    /**
     * @brief get name of the .ini alignment settings configuration file.
     * @return std::filesystem::path name of the alignment settings configuration file.
     */
    virtual std::filesystem::path getAlignmentSettingsConfigFilename() = 0;

    /**
     * @brief get path to the log files directory.
     * @return std::filesystem::path path to the log files directory.
     */
    virtual std::filesystem::path getPathToLogFilesDirectory() = 0;

    /**
     * @brief read Stepper Motor Index.
     * @param axis axis to be read.
     * @return Stepper Motor index string.
     */
    virtual std::string readStepperMotorIndexAxis(int axis) = 0;

    /**
     * @brief read Stepper Motor ratio [UNIT/STEP].
     * @param axis axis to be read.
     * @return Stepper Motor  ratio [UNIT/STEP] float.
     */
    virtual double readStepperMotorCONST(int axis) = 0;

    /**
     * @brief Read a std::string from configuration file and convert it in std::filesystem::path.
     * @param filename name of the file to be read.
     * @param path path to the directory of the file to be read.
     * @return std::filesystem::path converted.
     */
    virtual std::filesystem::path readFileSystemPathFromConfigurationFile(std::filesystem::path filename,
                                                                          std::filesystem::path path,
                                                                          std::string section,
                                                                          std::string key) = 0;

    /**
     * @brief Get the default hexapod controller Ip Address.
     * @return std::string containing the ip address to the hexapod controller.
     */
    virtual std::string getDefaultIpAddressHxp() = 0;

    /**
     * @brief Get the default stepper motor controller Ip Address.
     * @return std::string containing the ip address to the stepper motors controller.
     */
    virtual std::string getDefaultIpAddressStepper() = 0;

    /**
     * @brief Get the default hexapod controller port number.
     * @return int containing the number of the port used to establish a TCP connection with the hexapod controller.
     */
    virtual int getDefaultNPortHxp() = 0;
};
