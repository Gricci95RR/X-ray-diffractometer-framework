/**
 * @file StepperMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Implementation of "IMotor" used to mock the method of the parent class.
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

#include <string>

#include "IMotor.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

/**
 * @class StepperMock
 * @brief Implementation Class of 'IMotor' used to mock methods of the parent class.
 * 
 * The class 'StepperMock' inherits from 'IMotor' and overrides all of its methods
 * using the MOCK_METHOD macro provided by the Google Test framework.
 * Each overridden method is prefixed with MOCK_METHOD, which takes as arguments the method name,
 * the return type, and the method parameters.
 * 
 */
class StepperMock : public IMotor {
 public:
    MOCK_METHOD1(rad_to_user_units,
    float(float rad));
    MOCK_METHOD0(connect,
    int());
    MOCK_METHOD0(get_ipAddress,
    std::string());
    MOCK_METHOD0(get_motorIndex,
    std::string());
    MOCK_METHOD0(calibrate,
    int());
    MOCK_METHOD0(go_home,
    int());
    MOCK_METHOD1(moveCalibratedMotor,
    int(float position));
    MOCK_METHOD0(getPositionRad,
    float());
    MOCK_METHOD0(getPositionUserUnits,
    float());
    MOCK_METHOD0(softStop,
    int());
    MOCK_METHOD0(disconnect,
    int());
};
