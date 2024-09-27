/**
 * @file ScanningHXPMockConfiguration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Configuration class used to set the behavior of the mock object.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "ScanningHXPMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

namespace scanning {

/**
 * @class ScanningHXPMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The ConfigurationMockConfig class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class ScanningHXPMockConfiguration {
 public:
  ScanningHXPMockConfiguration() {
    ScanningHXPMock_.reset(new NiceMock<ScanningHXPMock>);
  }
  ~ScanningHXPMockConfiguration() {}
  std::shared_ptr<NiceMock<ScanningHXPMock>> getMock() {
        return ScanningHXPMock_;
  }
  void configureScanningHXPMock() {
    ON_CALL(*ScanningHXPMock_, scan()).WillByDefault(Return(true));
    ON_CALL(*ScanningHXPMock_, checkReachingPosition(_, _)).WillByDefault(Return(true));
  }

 private:
  std::shared_ptr<NiceMock<ScanningHXPMock>> ScanningHXPMock_;  /**< Shared pointer object of Class 'ScanningHXPMock. '*/
};

}  // namespace scanning
