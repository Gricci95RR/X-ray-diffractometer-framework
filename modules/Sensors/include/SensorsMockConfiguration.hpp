/**
 * @file SensorMockConfiguration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Configuration class used to set the behavior of the mock object.
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

#include "SensorsMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

namespace sensors {

/**
 * @class SensorsMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The SensorsMockConfiguration class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class SensorsMockConfiguration {
 public:
  SensorsMockConfiguration() {
    SensorsMock_.reset(new NiceMock<SensorsMock>);
  }
  ~SensorsMockConfiguration() {}
  std::shared_ptr<NiceMock<SensorsMock>> getMock() {
        return SensorsMock_;
  }
  void configureSensorsMock() {
    ON_CALL(*SensorsMock_, readXRaySensor(_, _)).WillByDefault(Return("00"));
    ON_CALL(*SensorsMock_, deinitializeXRaySensor()).WillByDefault(Return());
    ON_CALL(*SensorsMock_, motionStabilizationTimer(_)).WillByDefault(Return());
  }

 private:
  std::shared_ptr<NiceMock<SensorsMock>> SensorsMock_;  /**< Shared pointer object of Class 'SensorsMock'. */
};

}  // namespace sensors
