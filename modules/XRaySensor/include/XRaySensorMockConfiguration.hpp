/**
 * @file XRaySensorMockConfiguration.hpp
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

#include "XRaySensorMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;


/**
 * @class XRaySensorMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The XRaySensorMockConfiguration class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class XRaySensorMockConfiguration {
 public:
  XRaySensorMockConfiguration() {
    XRaySensorMock_.reset(new NiceMock<XRaySensorMock>);
  }
  ~XRaySensorMockConfiguration() {}
  std::shared_ptr<NiceMock<XRaySensorMock>> getMock() {
        return XRaySensorMock_;
  }
  void configureXRaySensorMock() {
    ON_CALL(*XRaySensorMock_, read()).WillByDefault(Return("0000"));
    ON_CALL(*XRaySensorMock_, write(_)).WillByDefault(Return());
    ON_CALL(*XRaySensorMock_, deinitialize()).WillByDefault(Return());
  }

 private:
  std::shared_ptr<NiceMock<XRaySensorMock>> XRaySensorMock_;  /**< Shared pointer to Class XRaySensorMock. */
};
