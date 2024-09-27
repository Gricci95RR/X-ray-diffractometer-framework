/**
 * @file StepperMockConfiguration.hpp
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

#include "StepperMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

/**
 * @class StepperMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The 'StepperMockConfiguration' class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class StepperMockConfiguration {
 public:
    StepperMockConfiguration() {
        stepperMock_.reset(new NiceMock<StepperMock>);
    }
    ~StepperMockConfiguration() {}
    std::shared_ptr<NiceMock<StepperMock>> getMock() {
        return stepperMock_;
    }
    void configureStepperMock() {
        ON_CALL(*stepperMock_, connect()).WillByDefault(Return(0));
        ON_CALL(*stepperMock_, go_home()).WillByDefault(Return(0));
        ON_CALL(*stepperMock_, calibrate()).WillByDefault(Return(0));
        ON_CALL(*stepperMock_, disconnect()).WillByDefault(Return(0));
        ON_CALL(*stepperMock_, moveCalibratedMotor(_)).WillByDefault(Return(0));
        ON_CALL(*stepperMock_, getPositionUserUnits()).WillByDefault(Return(0));
    }
 private:
    std::shared_ptr<NiceMock<StepperMock>> stepperMock_;
};
