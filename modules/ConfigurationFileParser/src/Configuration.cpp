/**
 * @file Configuration.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'IConfiguration' used to write/read the configuration files.
 * @version 0.1
 * @date 2022-03-01
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "Configuration.hpp"

Configuration::Configuration() {
    spdlog::info("cTor2 Configuration\n");
    path_ = this->getPartialPathUntilKeyName(std::filesystem::current_path(), projectName_) / configurationFilesFolder_;
}

Configuration::~Configuration() {
    spdlog::info("dTor Configuration\n");
}

std::filesystem::path Configuration::getPartialPathUntilKeyName(
                    const std::filesystem::path pathReference,
                    const std::filesystem::path keyPathName) {
    spdlog::info("Method getPartialPathUntilKeyName of Class Configuration\n");
    spdlog::debug("Current Path: {} \n", pathReference.string());
    std::filesystem::path partialPath;
    std::filesystem::path::iterator itrPath = std::find(pathReference.begin(),
                                                        pathReference.end(),
                                                        keyPathName);
    if (itrPath != pathReference.end()) {
        std::advance(itrPath, 1);
        std::for_each(pathReference.begin(),
                      itrPath,
                      [&partialPath](std::filesystem::path nameInPath){
                          partialPath/= nameInPath;
                          });
    } else {
        spdlog::error("KeyName ({}) not Found in Path ({})",
                       keyPathName.string(),
                       pathReference.string());
    }
    spdlog::debug("Partial path: {} \n", partialPath.string());
    return partialPath;
}

int Configuration::writeConfigurationFile(std::string value,
                                          std::string section,
                                          std::string key,
                                          std::filesystem::path filename,
                                          std::filesystem::path path) {
    spdlog::info("Method writeStringConfigurationFile "
                 "of Class Configuration\n");
    path /= filename;
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    // now we can read the file
    bool readSuccess = file.read(ini);
    if (readSuccess == true) {
        // update a value or add a new entry
        ini[section][key] = value;
        // write updates to file
        file.write(ini, true);
        return 1;
    } else {
        spdlog::error("Error in writeStringConfigurationFile"
                      " of Class Configuration\n");
        return 0;
    }
}

int Configuration::createConfigurationFile(std::filesystem::path filename,
                                           std::filesystem::path path) {
    spdlog::info("Method createConfigurationFile of Class Configuration\n");
    path /= filename;
    mINI::INIFile file(path.string());
    mINI::INIStructure ini;
    bool generatesuccess = file.generate(ini, true);
    if (generatesuccess == true) {
        spdlog::debug("New .ini file created\n");
        path_ = path;
        return 1;
    } else {
        spdlog::error("Error in createConfigurationFile"
                      " of Class Configuration. "
                      "File not created.\n");
        return 0;
    }
}

int Configuration::hasFile(std::filesystem::path filename,
                           std::filesystem::path path) {
    path /= filename;
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    // now we can read the file
    bool readSuccess = file.read(ini);
    if (readSuccess == true) {
        spdlog::debug("Read Success!\n");
        return 1;
    } else {
        spdlog::error("Error in hasFile of Class Configuration\n");
        return 0;
    }
}

int Configuration::hasSection(std::string section,
                              std::filesystem::path filename,
                              std::filesystem::path path) {
    spdlog::info("Method hasSection of Class Configuration\n");
    path /= filename;
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    file.read(ini);
    bool hasSection = ini.has(section);
    if (hasSection == true) {
        return 1;
    } else {
        return 0;
    }
}

int Configuration::hasKey(std::string section,
                          std::string key,
                          std::filesystem::path filename,
                          std::filesystem::path path) {
    path /= filename;
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    file.read(ini);
    bool hasKey = ini[section].has(key);
    if (hasKey == true) {
        return 1;
    } else {
        return 0;
    }
}

int Configuration::removeConfigurationFile(std::filesystem::path filename,
                                           std::filesystem::path path) {
    spdlog::info("Method removeConfigurationFile of Class Configuration\n");
    path /= filename;
    if (std::filesystem::remove(path)) {
        return 0;
    } else {
        return 1;
    }
}

std::string Configuration::readStringFromConfigurationFile(std::filesystem::path filename,
                                                           std::filesystem::path path,
                                                           std::string section,
                                                           std::string key) {
    spdlog::info("Method readStringFromConfigurationFile of Class Configuration\n");
    path /= filename;
    std::string result;
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    // read the file
    bool readSuccess = file.read(ini);
    if (readSuccess == true) {
        result = ini.get(section).get(key);
        spdlog::info("Read string {} {} Ok!\n", key ,result);
    } else {
        spdlog::error("Error\n");
    }
    return result;
}

bool Configuration::readBoolFromConfigurationFile(std::filesystem::path filename,
                                                  std::filesystem::path path,
                                                  std::string section,
                                                  std::string key) {
    int result = this->readIntFromConfigurationFile(filename,
                                                        path,
                                                        section,
                                                        key);
    if (result == 1) {
        return true;
    } else {
        return false;
    }

}

float Configuration::readFloatFromConfigurationFile(std::filesystem::path filename,
                                                    std::filesystem::path path,
                                                    std::string section,
                                                    std::string key) {
    spdlog::info("Method readFloatFromConfigurationFile of Class Configuration\n");
    float result = std::stof(this->readStringFromConfigurationFile(filename, path, section, key));
    return result;
}

int Configuration::readIntFromConfigurationFile(std::filesystem::path filename,
                                                std::filesystem::path path,
                                                std::string section,
                                                std::string key) {
    spdlog::info("Method readIntFromConfigurationFile of Class Configuration\n");
    int result = std::stoi(this->readStringFromConfigurationFile(filename,
                                                                 path,
                                                                 section,
                                                                 key));
    return result;
}

std::filesystem::path Configuration::readFileSystemPathFromConfigurationFile(std::filesystem::path filename,
                                                                             std::filesystem::path path,
                                                                             std::string section,
                                                                             std::string key) {
    spdlog::info("Method readFileSystemPathFromConfigurationFile of Class Configuration\n");
    std::filesystem::path result = std::filesystem::u8path(this->readStringFromConfigurationFile(filename,
                                                                                                 path,
                                                                                                 section,
                                                                                                 key));
    return result;
}

std::filesystem::path Configuration::getPath() {
    return path_;
}

void Configuration::setPath(std::filesystem::path path) {
    path_ = path;
}

std::filesystem::path Configuration::getConfigFilename() {
    return configFilename_;
}

std::filesystem::path Configuration::getAlignmentSettingsConfigFilename() {
    return alignmentSettingsFilename_;
}

std::filesystem::path Configuration::getPathToLogFilesDirectory() {
    pathToLogFilesDirectory_ = this->getPartialPathUntilKeyName(std::filesystem::current_path(), projectName_) / logFilesFolder_;
    return pathToLogFilesDirectory_;
}

std::string Configuration::readStepperMotorIndexAxis(int axis) {
    spdlog::info("Method readStepperMotorIndexAxis "
                 "of Class ConfigurationStepper\n");
    std::filesystem::path path = path_ / this->getConfigFilename();
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    std::string stepperMotorIndexAxis;
    // read the file
    bool readSuccess = file.read(ini);
    if (readSuccess == true) {
        spdlog::info("Read Ok!\n");
        std::string stepper_axis = "STEPPER_AXIS_" + std::to_string(axis);
        stepperMotorIndexAxis = ini.get(stepper_axis).get("ID");
    } else {
        spdlog::error("Error\n");
    }
    return stepperMotorIndexAxis;
}

double Configuration::readStepperMotorCONST(int axis) {
    spdlog::info("Method readStepperMotorCONST of "
                 "Class ConfigurationStepper\n");
    std::filesystem::path path = path_ / this->getConfigFilename();
    // create a file instance
    mINI::INIFile file(path.string());
    // create a structure that will hold data
    mINI::INIStructure ini;
    double const_var;
    // read the file
    bool readSuccess = file.read(ini);
    if (readSuccess == true) {
        spdlog::info("Read Ok!\n");
        std::string stepper_axis = "STEPPER_AXIS_" + std::to_string(axis);
        std::string value = ini.get(stepper_axis).get("CONST");
        const_var =  std::stod(value);
    } else {
        spdlog::error("Error\n");
    }
    return const_var;
}

std::string Configuration::getDefaultIpAddressHxp() {
    return defaultConfiguration_.iPAddressHxp;
}

std::string Configuration::getDefaultIpAddressStepper() {
    return defaultConfiguration_.iPAddressStepper;
}

int Configuration::getDefaultNPortHxp() {
    return defaultConfiguration_.nPortHxp;
}