/**
 * @file HXPMockConfiguration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Configuration class used to set the behavior of the mock object.
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "HXPMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

/**
 * @class HXPMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 */
class HXPMockConfiguration {
 public:
  HXPMockConfiguration() {
    hxpMock_.reset(new NiceMock<HXPMock>);
  }
  ~HXPMockConfiguration() {}
  std::shared_ptr<NiceMock<HXPMock>> getMock() {
        return hxpMock_;
  }
  void configureHXPMock() {
    ON_CALL(*hxpMock_, connect(_, _)).WillByDefault(Return(0));
    ON_CALL(*hxpMock_, goHome()).WillByDefault(Return(0));
    ON_CALL(*hxpMock_, disconnect()).WillByDefault(Return(0));
    ON_CALL(*hxpMock_, setPositionAbsolute(_, _, _, _, _, _)).WillByDefault(Return(0));
  }

 private:
  std::shared_ptr<NiceMock<HXPMock>> hxpMock_;  /**< Shared pointer object of Class 'HXPMock. '*/
};
