/**
 * @file ConfigurationMockConfig.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Configuration Class used to set the behavior of the mock object.
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

#include <memory>

#include "ConfigurationMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

/**
 * @class ConfigurationMockConfig
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The ConfigurationMockConfig class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class ConfigurationMockConfig {
 public:
  ConfigurationMockConfig() {
    ConfigurationMock_.reset(new NiceMock<ConfigurationMock>);
  }
  ~ConfigurationMockConfig() {}
  std::shared_ptr<NiceMock<ConfigurationMock>> getMock() {
        return ConfigurationMock_;
  }
  void configureConfigurationMock() {
    ON_CALL(*ConfigurationMock_, getPartialPathUntilKeyName(_, _)).WillByDefault(Return("C:\\Users\\giricci\\Desktop\\XRay_Machine\\"));
    ON_CALL(*ConfigurationMock_, writeConfigurationFile(_, _, _, _, _)).WillByDefault(Return(1));
    ON_CALL(*ConfigurationMock_, readFloatFromConfigurationFile(_, _, _, _)).WillByDefault(Return(0.1));
    ON_CALL(*ConfigurationMock_, readIntFromConfigurationFile(_, _, _, _)).WillByDefault(Return(1));
    ON_CALL(*ConfigurationMock_, readStringFromConfigurationFile(_, _, _, _)).WillByDefault(Return("test_read_string"));
    ON_CALL(*ConfigurationMock_, readBoolFromConfigurationFile(_, _, _, _)).WillByDefault(Return(true));
    ON_CALL(*ConfigurationMock_, readFileSystemPathFromConfigurationFile(_, _, _, _)).WillByDefault(Return("test_read_systemPath"));
    ON_CALL(*ConfigurationMock_, getPathToLogFilesDirectory()).WillByDefault(Return("test_getPathToLogFilesDirectory"));
    ON_CALL(*ConfigurationMock_, getAlignmentSettingsConfigFilename()).WillByDefault(Return("test_getAlignmentSettingsConfigFilename"));
    ON_CALL(*ConfigurationMock_, getConfigFilename()).WillByDefault(Return("test_getConfigFilename"));
    ON_CALL(*ConfigurationMock_, readStepperMotorIndexAxis(_)).WillByDefault(Return("0003"));
    ON_CALL(*ConfigurationMock_, readStepperMotorCONST(_)).WillByDefault(Return(0.03));
    ON_CALL(*ConfigurationMock_, getDefaultIpAddressHxp()).WillByDefault(Return("192.168.0.3"));
    ON_CALL(*ConfigurationMock_, getDefaultIpAddressStepper()).WillByDefault(Return("192.168.0.2"));
    ON_CALL(*ConfigurationMock_, getDefaultNPortHxp()).WillByDefault(Return(5001));
  }

 private:
  std::shared_ptr<NiceMock<ConfigurationMock>> ConfigurationMock_;  /**< Shared pointer object of Class 'ConfigurationMock. '*/
};
