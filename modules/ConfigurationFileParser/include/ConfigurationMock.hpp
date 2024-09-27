/**
 * @file ConfigurationMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'IConfiguration' used to mock methods of the parent class.
 * @version 0.1
 * @date 2022-03-01
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "IConfiguration.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

/**
 * @class ConfigurationMock
 * @brief Implementation Class of 'IConfiguration' used to mock methods of the parent class.
 * 
 * The class 'ConfigurationMock' inherits from 'IConfiguration' and overrides all of its methods
 * using the MOCK_METHOD macro provided by the Google Test framework.
 * Each overridden method is prefixed with MOCK_METHOD, which takes as arguments the method name,
 * the return type, and the method parameters.
 * 
 */
class ConfigurationMock : public IConfiguration {
 public:
  MOCK_METHOD2(getPartialPathUntilKeyName, std::filesystem::path(const std::filesystem::path pathReference,
                                                                 const std::filesystem::path keyPathName));
  MOCK_METHOD5(writeConfigurationFile, int(std::string value,
                                           std::string section,
                                           std::string key,
                                           std::filesystem::path filename,
                                           std::filesystem::path path));
  MOCK_METHOD2(createConfigurationFile, int(std::filesystem::path filename,
                                            std::filesystem::path path));
  MOCK_METHOD2(hasFile, int(std::filesystem::path filename,
                            std::filesystem::path path));
  MOCK_METHOD3(hasSection, int(std::string section,
                               std::filesystem::path filename,
                               std::filesystem::path path));
  MOCK_METHOD4(hasKey, int(std::string section,
                           std::string key,
                           std::filesystem::path filename,
                           std::filesystem::path path));
  MOCK_METHOD2(removeConfigurationFile, int(std::filesystem::path filename,
                                            std::filesystem::path path));
  MOCK_METHOD4(readFloatFromConfigurationFile, float(std::filesystem::path filename,
                                                     std::filesystem::path path,
                                                     std::string section,
                                                     std::string key));
  MOCK_METHOD4(readIntFromConfigurationFile, int(std::filesystem::path filename,
                                                 std::filesystem::path path,
                                                 std::string section,
                                                 std::string key));
  MOCK_METHOD4(readStringFromConfigurationFile, std::string(std::filesystem::path filename,
                                                            std::filesystem::path path,
                                                            std::string section,
                                                            std::string key));
  MOCK_METHOD4(readBoolFromConfigurationFile, bool(std::filesystem::path filename,
                                                   std::filesystem::path path,
                                                   std::string section,
                                                   std::string key));
  MOCK_METHOD4(readFileSystemPathFromConfigurationFile, std::filesystem::path(std::filesystem::path filename,
                                                                              std::filesystem::path path,
                                                                              std::string section,
                                                                              std::string key));
  MOCK_METHOD0(getPath, std::filesystem::path());
  MOCK_METHOD1(setPath, void(std::filesystem::path path));
  MOCK_METHOD0(getConfigFilename, std::filesystem::path());
  MOCK_METHOD0(getAlignmentSettingsConfigFilename, std::filesystem::path());
  MOCK_METHOD0(getPathToLogFilesDirectory, std::filesystem::path());
  MOCK_METHOD1(readStepperMotorIndexAxis, std::string(int axis));
  MOCK_METHOD1(readStepperMotorCONST, double(int axis));
  MOCK_METHOD0(getDefaultIpAddressHxp, std::string());
  MOCK_METHOD0(getDefaultIpAddressStepper, std::string());
  MOCK_METHOD0(getDefaultNPortHxp, int());
};
