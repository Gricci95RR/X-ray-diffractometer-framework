/**
 * @file ScanningStepperMockConfiguration.hpp
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

#include "ScanningStepperMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

namespace scanning {

/**
 * @class ScanningStepperMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The ConfigurationMockConfig class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class ScanningStepperMockConfiguration {
 public:
  ScanningStepperMockConfiguration() {
    ScanningStepperMock_.reset(new NiceMock<ScanningStepperMock>);
  }
  ~ScanningStepperMockConfiguration() {}
  std::shared_ptr<NiceMock<ScanningStepperMock>> getMock() {
        return ScanningStepperMock_;
  }
  void configureScanningStepperMock() {
    ON_CALL(*ScanningStepperMock_, setupAlignmentParameters(_, _, _, _, _, _)).WillByDefault(Return());
    ON_CALL(*ScanningStepperMock_, scan()).WillByDefault(Invoke(
      [this] {
          return true;
      }));
    ON_CALL(*ScanningStepperMock_, checkReachingPosition(_, _)).WillByDefault(Return(true));
  }

 private:
  std::shared_ptr<NiceMock<ScanningStepperMock>> ScanningStepperMock_;  /**< Shared pointer object of Class 'ScanningStepperMock'. */
};

}  // namespace scanning
